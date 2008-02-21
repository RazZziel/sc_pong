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



#ifndef __CGAME_H__
#define __CGAME_H__


#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include "CFramerate.h"
#include "CBall.h"
#include "CPlayer.h"



#define P_BACKGROUND	"data/pixmap/game/bg_zerg.png"

#define P_LIFE_EMPTY	"data/pixmap/game/life_empt.png"
#define P_LIFE_FULL	"data/pixmap/game/life_full.png"

enum {
	LIFE_OFFSET	= 5,
	LIFE_STEP	= 6,
	LIFE_MAX	= 34,
	LIFE_Y		= SCREEN_HEIGHT/15,
	LIFE_X0		= SCREEN_WIDTH*29/40,
	LIFE_X1		= SCREEN_WIDTH*11/40
};

#define P_COVERCAT	"data/pixmap/game/covercat.png"
#define P_COVERSNAKE	"data/pixmap/game/coversnake.png"

/* Sounds */

#define S_FLASH1	"data/sound/game/intro_flash1.wav"
#define S_FLASH2	"data/sound/game/intro_flash2.wav"

#define M_RAZIOZERG	"data/sound/music/radiofreezerg.ogg"

enum {
	GAME_FPS = 60
};

class Game {
	
	private:
		Player *players[2], *winner, *loser;
		SDL_Surface *background, *life_empty, *life_full;
		SDL_Surface *covercat, *coversnake;
		Mix_Music *s_music;
		bool multiplayer;
		
		int life_x0, life_x1;
		int round, rounds;
		
		SDL_Rect life[2];
		
		void draw_player_stats();
		void setup_player_stats();
		void intro();
		void round_title();
		bool game_loop();
		void end();
		
	public:
		std::vector<Ball*> balls;
		
		Game(Player *player0, Player *player1, int rounds);
		~Game();
		
		void configure_multiplayer();
		void start();
		
};


#endif
