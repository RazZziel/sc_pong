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



#ifndef __CBALL_H__
#define __CBALL_H__

#ifdef __USE_OPENAL__
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#endif

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

//#include "widgets.h" // WHAT THE FUCK.
#include "CVector.h"
#include "CAnimation.h"


enum {
	POS_CENTER   = -1,
	S3D_NBUFFERS = 2
};

/* Files */

// Pixmaps

#define P_BALL_ZERG			"data/pixmap/ball/zerg_ball_spin.png"
#define P_BALL_BIRTH_ZERG		"data/pixmap/ball/zerg_ball_birth.png"
#define P_BALL_HIDE_ZERG		"data/pixmap/ball/zerg_ball_hide.png"
#define P_BALL_UNHIDE_ZERG		"data/pixmap/ball/zerg_ball_unhide.png"

#define P_BALL_TERRAN			"data/pixmap/ball/terran_ball_spin.png"
#define P_BALL_BIRTH_TERRAN		"data/pixmap/ball/terran_ball_birth.png"

#define P_BALL_PROTOSS			"data/pixmap/ball/protoss_ball_spin.png"
#define P_BALL_BIRTH_PROTOSS		"data/pixmap/ball/protoss_ball_birth.png"


// Sounds

#define S_ZERG_START			"data/sound/ball/zerg_ball_start.wav"
#define S_ZERG_READY			"data/sound/ball/zerg_ball_ready.wav"
#define S_ZERG_BIRTH			"data/sound/ball/zerg_ball_birth.wav"
#define S_ZERG_BOUNCE			"data/sound/ball/zerg_ball_bounce.wav"
#define S_ZERG_LOOP			"data/sound/ball/zerg_ball_loop.wav"

#define S_TERRAN_START			"data/sound/ball/terran_ball_start.wav"
#define S_TERRAN_READY			"data/sound/ball/terran_ball_ready.wav"
#define S_TERRAN_BIRTH			"data/sound/ball/terran_ball_birth.wav"
#define S_TERRAN_BOUNCE			"data/sound/ball/terran_ball_bounce.wav"

#define S_PROTOSS_START			"data/sound/ball/protoss_ball_start.wav"
#define S_PROTOSS_READY			"data/sound/ball/protoss_ball_ready.wav"
#define S_PROTOSS_BIRTH			"data/sound/ball/protoss_ball_birth.wav"
#define S_PROTOSS_BOUNCE 		"data/sound/ball/protoss_ball_bounce.wav"


typedef enum {
	NORMAL,
	EGG,
	
	HIDDING,
	HIDDEN,
	UNHIDDING
} BallStatus;


typedef enum {
	CMB_NULL,
	
	CMB_NEUT_1, // Square
	CMB_NEUT_2, // Reverse square
	CMB_NEUT_3, // Sin
	CMB_NEUT_4, // Exp
	
	CMB_HIDE,   // (bola zerg) Lurk moar (protoss con torretas inmune)
		    // (bola terr) Cloak (se ve un poco, pero no se descubre al final) (protoss con torretas inmune)
		    // (bola toss) yay
	
	CMB_TURBO,  // (plyr terr) Jaco
		    // (plyr toss) Teleport-o
	
	CMB_ZERG_1, // (bola zerg) Naruto rush
	CMB_ZERG_2, // (plyr zerg) Infest Terran (only against Terran) (las bolas que lanza el terran le vienen de vuelta)
		                                         // (tarda tiempo en ser casteado y en hacer efecto, ver en juego)
	
	CMB_TERR_1, // (plyr terr) Lockdown (paraliza oponente (no hasta el final de todo))
	CMB_TERR_2, // (plyr terr) Nuke
	CMB_TERR_3,
	
	CMB_TOSS_1, // (plyr toss) Torretas
	CMB_TOSS_2  // (plyr toss) Carrier (lanza bichos que desvían bola aleatoriamente)
} Combo;


class BallData {
	
	public:
		AnimationData *ani_spin_data, *ani_birth_data, *ani_hide_data, *ani_unhide_data;
		Mix_Chunk *s_birth, *s_bounce, *s_start, *s_ready;
		
		BallData();
		~BallData();
		
#ifdef __USE_OPENAL__
		ALuint s3d_buffers[S3D_NBUFFERS];
		ALboolean loadALData();
#endif
	
};

class BallDataZerg : public BallData {
	public:
		BallDataZerg();
		~BallDataZerg() {printf("data zerg destructor\n");};
};
class BallDataTerran : public BallData {
	public:
		BallDataTerran();
		~BallDataTerran() {printf("data terran destructor\n");};
};
class BallDataProtoss : public BallData {
	public:
		BallDataProtoss();
		~BallDataProtoss() {printf("data protoss destructor\n");};
};

class Ball {

	protected:
		class Game *game;
		Animation *ani_spin, *ani_birth, *ani_hide, *ani_unhide; // TODO special 1, special2...?
		
#ifdef __USE_OPENAL__
		ALuint s3d_sources[2];
		ALfloat s3d_src_pos[3]; // Position of the source sound.
		ALfloat s3d_src_vel[3]; // Velocity of the source sound.
#endif
		
		int race;
		bool birth_sound;
		BallStatus status;
		
		// Combo stuff
		Combo effect;
		int effect_way;
		float steps;
		float y0;
		Vector or_p;
		float def_vx;
		
		int cmb_level;
		int cmb_level_reached;
		int cmb_key_counter;
		int cmb_key_pressed;
		int cmb_boost;
		class Player *cmb_player;
		
#ifdef __USE_OPENAL__
		ALboolean initALSources();
#endif
	
	public:
		Vector p;		// Position vector
		Vector v;		// Speed vector
		dot way;		// Unitary speed vector
		int r;			// Radius
		int m;			// Mass
		bool mainBall;		// Is this the main ball of the game or a cloned ball?
		BallData *data;
		
	
		Ball(float x, float y, float vx, float vy, BallStatus state, class Game *game);
		virtual ~Ball();
		
		void show(/*Window */int window=NULL);
		void sayStart();
		void sayReady();
		bool checkCollisions(/*Window */int window);
		
		void setEffect(Combo effect, int effect_way, Player *player = NULL);
		
		inline void birth()			{ birth_sound = true; effect = CMB_NULL; status = EGG; }
		inline void resetPosition()		{ p.x = or_p.x, p.y = or_p.y; }
		inline void setPosition(int x, int y)	{ p = Vector(x, y); }
		inline void setVelocity(int vx, int vy)	{ v = Vector(vx, vy); }
		inline void accelerate(int ax, int ay)	{ v += Vector(ax, ay); }
		
		virtual void initCombo()=0;
		virtual void handleCombo()=0;
		
};

class BallZerg : public Ball {
	public:
		BallZerg(float x, float y, float vx, float vy, BallStatus state, class Game *game);
		~BallZerg() {printf("zerg destructor\n");};
		void initCombo();
		void handleCombo();
};

class BallTerran : public Ball {
	public:
		BallTerran(float x, float y, float vx, float vy, BallStatus state, class Game *game);
		~BallTerran() {printf("terran destructor\n");};
		void initCombo();
		void handleCombo();
};
class BallProtoss : public Ball {
	public:
		BallProtoss(float x, float y, float vx, float vy, BallStatus state, class Game *game);
		~BallProtoss() {printf("toss destructor\n");};
		void initCombo();
		void handleCombo();
};


#endif
