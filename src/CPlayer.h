/***************************************************************************
 *   Copyright (C) 2006 by Ismael Barros (Raziel)                          *
 *   razielmine@gmail.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __CPAD_H__
#define __CPAD_H__


#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#ifdef __USE_OPENAL__
#include <AL/al.h>
#endif

#include "CVector.h"
#include "CController.h"
#include "CAnimation.h"
#include "CBall.h"


enum {
	RIGHT = -1,
	LEFT  = -2
};

enum {
	COMPUTER  = 0,
	HUMAN     = 1,
	NETHUMAN  = 2
};

enum {
	COMBO_DELAY  = 100,
	COMBOFLASH_V = 10
};

/* Files */

// Pixmaps

#define P_ZERG_PLAYER			"data/pixmap/player/zerg_player.png"
#define P_ZERG_PORTRAIT			"data/pixmap/player/zerg_portrait.png"
#define P_ZERG_COMBOFLASH		"data/pixmap/player/zerg_comboflash.png"
#define P_ZERG_TITLE_L			"data/pixmap/player/zerg_title_l.png"
#define P_ZERG_TITLE_R			"data/pixmap/player/zerg_title_r.png"

#define P_TERRAN_PLAYER			"data/pixmap/player/terran_player.png"
#define P_TERRAN_PORTRAIT		"data/pixmap/player/terran_portrait.png"
#define P_TERRAN_COMBOFLASH		"data/pixmap/player/terran_comboflash.png"
#define P_TERRAN_TITLE_L		"data/pixmap/player/terran_title_l.png"
#define P_TERRAN_TITLE_R		"data/pixmap/player/terran_title_r.png"

#define P_PROTOSS_PLAYER		"data/pixmap/player/protoss_player.png"
#define P_PROTOSS_PORTRAIT		"data/pixmap/player/protoss_portrait.png"
#define P_PROTOSS_COMBOFLASH		"data/pixmap/player/terran_comboflash.png"
#define P_PROTOSS_TITLE_L		"data/pixmap/player/protoss_title_l.png"
#define P_PROTOSS_TITLE_R		"data/pixmap/player/protoss_title_r.png"


// Sounds

#define S_ZERG_HIT			"data/sound/player/zerg_plr_hit.wav"
#define S_ZERG_WIN			"data/sound/player/zerg_plr_win.wav"
#define S_ZERG_WIN2			"data/sound/player/zerg_plr_win2.wav"
#define S_ZERG_LOSE			"data/sound/player/zerg_plr_lose.wav"

#define S_TERRAN_HIT			"data/sound/player/terran_plr_hit.wav"
#define S_TERRAN_WIN			"data/sound/player/terran_plr_win.wav"
#define S_TERRAN_WIN2			"data/sound/player/terran_plr_win2.wav"
#define S_TERRAN_LOSE			"data/sound/player/terran_plr_lose.wav"

#define S_PROTOSS_HIT			"data/sound/player/protoss_plr_hit.wav"
#define S_PROTOSS_WIN			"data/sound/player/protoss_plr_win.wav"
#define S_PROTOSS_WIN2			"data/sound/player/protoss_plr_win2.wav"
#define S_PROTOSS_LOSE			"data/sound/player/protoss_plr_lose.wav"


class Player {

	protected:
		SDL_Surface *sprite;
		Animation *comboflash;
		
		int race;
		int direction;
		CtrlKey plr_forward, plr_back;
		CtrlKey last_key;
		
		int combo_state;
		Uint32 combo_ticks;
		
		void updateCombo();
		
#ifdef __USE_OPENAL__
		ALfloat s3d_lst_pos[3]; // Position of the listener.
		ALfloat s3d_lst_vel[3]; // Velocity of the listener.
		// Orientation of the listener (first 3 elements are "at", second 3 are "up")
		ALfloat s3d_lst_ori[6];
		
		void initALListener();
		ALboolean updateALListener();
#endif
		
		inline void setComboState(int new_state) {
			combo_state = new_state;
			combo_ticks = SDL_GetTicks() + COMBO_DELAY;
		}
	
	public:
		SDL_Surface *portrait, *title;
		Mix_Chunk *s_hit, *s_win, *s_win2, *s_lose;
		
		const char *name;
		int n_player;
		int type;
		int life_max;
		int life;	// Life value, between 0 and life_max
		int lifeg;	// Graphical life value, each lifeg value represents a step in the life bar
		float lifegi;	// Instant-lifeg, represents the actual value drawn in screen (because of the life animation)
		int x;
		int vy;
		Vector v;
		Vector p1, p2;
		int r;
		int m;
		int wins;
		Combo combo;
	
		Player(int x, int direction, int type, const char *name, int n_player);
		void setup();
		~Player();
		
		void handle_input();
		void move(std::vector<Ball*> balls);
		void show();
		void hurt();
		void setLife(int _life);
		void reset();
		
};

class PlayerZerg : public Player {
	public:
		PlayerZerg(int x, int direction, int type, const char *name, int n_player);
};

class PlayerTerran : public Player {
	public:
		PlayerTerran(int x, int direction, int type, const char *name, int n_player);
};

class PlayerProtoss : public Player {
	public:
		PlayerProtoss(int x, int direction, int type, const char *name, int n_player);
};


#endif
