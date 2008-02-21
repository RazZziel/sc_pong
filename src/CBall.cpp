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


#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __USE_OPENAL__
#include <AL/al.h>
#include <AL/alut.h>
#endif

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"

#include "main.h"
#include "audio.h"
#include "error.h"
#include "CGraphics.h"
#include "CAnimation.h"
#include "CControl.h"
#include "CBall.h"
#include "CPlayer.h"
#include "CGame.h"


extern Control *control;

BallDataZerg *ball_data_zerg = NULL;
BallDataTerran *ball_data_terran = NULL;
BallDataProtoss *ball_data_protoss = NULL;

/* BallData */


BallData::BallData() {
#ifdef __USE_OPENAL__
	loadALData();
#endif
}
BallData::~BallData() {
#ifdef __USE_OPENAL__
	alDeleteBuffers(S3D_NBUFFERS, this->s3d_buffers);
#endif
}
Ball::~Ball() {
#ifdef __USE_OPENAL__
	alDeleteSources(S3D_NBUFFERS, this->s3d_sources);
#endif
}
BallDataZerg::BallDataZerg() {
	ani_spin_data = new AnimationData(P_BALL_ZERG, 16, 4, &defaultColorkey);
	ani_birth_data = new AnimationData(P_BALL_BIRTH_ZERG, 31, 7, &defaultColorkey);
	ani_hide_data = new AnimationData(P_BALL_HIDE_ZERG, 5, 3);
	ani_unhide_data = new AnimationData(P_BALL_UNHIDE_ZERG, 5, 3);
	
	s_birth = loadSound(S_ZERG_BIRTH);
	s_start = loadSound(S_ZERG_START);
	s_ready = loadSound(S_ZERG_READY);
	s_bounce = loadSound(S_ZERG_BOUNCE);
}
BallDataTerran::BallDataTerran() {
	ani_spin_data = new AnimationData(P_BALL_TERRAN, 16, 4, &defaultColorkey);
	ani_birth_data = new AnimationData(P_BALL_BIRTH_TERRAN, 7, 96, &defaultColorkey);
	//ani_hide_data = new AnimationData(P_BALL_HIDE_TERRAN, 31, 4, 4);
	//ani_unhide_data = new AnimationData(P_BALL_UNHIDE_TERRAN, 31, 4, 4);
	
	s_birth = loadSound(S_TERRAN_BIRTH);
	s_start = loadSound(S_TERRAN_START);
	s_ready = loadSound(S_TERRAN_READY);
	s_bounce = loadSound(S_TERRAN_BOUNCE);
}
BallDataProtoss::BallDataProtoss() {
	ani_spin_data = new AnimationData(P_BALL_PROTOSS, 16, 4, &defaultColorkey);
	ani_birth_data = new AnimationData(P_BALL_BIRTH_PROTOSS, 26, 7,  &defaultColorkey);
	//ani_hide_data = new AnimationData(P_BALL_HIDE_PROTOSS, 31, 4, 4);
	//ani_unhide_data = new AnimationData(P_BALL_UNHIDE_PROTOSS, 31, 4, 4);
	
	s_birth = loadSound(S_PROTOSS_BIRTH);
	s_start = loadSound(S_PROTOSS_START);
	s_ready = loadSound(S_PROTOSS_READY);
	s_bounce = loadSound(S_PROTOSS_BOUNCE);
}

#ifdef __USE_OPENAL__
ALboolean BallData::loadALData() {
	
	// Variables to load into.
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;
	
	// Load wav data into a buffer.
	alGenBuffers(S3D_NBUFFERS, this->s3d_buffers);
	
	if (alGetError() != AL_NO_ERROR) return AL_FALSE;
	
	alutLoadWAVFile(S_ZERG_LOOP, &format, &data, &size, &freq, &loop);
	alBufferData(this->s3d_buffers[0], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	
	alutLoadWAVFile(S_ZERG_BOUNCE, &format, &data, &size, &freq, &loop);
	alBufferData(this->s3d_buffers[1], format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	
	if (alGetError() != AL_NO_ERROR) return AL_FALSE;
	return AL_TRUE;
}
#endif

/* Ball */

Ball::Ball(float x, float y, float vx, float vy, BallStatus state, Game *game) : game(game),
										 birth_sound(true),
										 status(state),
										 effect(CMB_NULL),
										 effect_way(0),
										 steps(0),
										 y0(y),
										 or_p(Vector(x, y)),
										 def_vx(vx),
										 p(or_p),
										 v(Vector(vx, vy)),
										 mainBall(false) {
}


BallZerg::BallZerg(float x, float y, float vx, float vy, BallStatus state, Game *game)
	: Ball(x, y, vx, vy, state, game) {
	r = 20;
	m = 10;
	if (ball_data_zerg == NULL)
		ball_data_zerg = new BallDataZerg();
	data = ball_data_zerg;
	
	ani_spin = new Animation(data->ani_spin_data, 4);
	ani_birth = new Animation(data->ani_birth_data, 4);
	ani_hide = new Animation(data->ani_hide_data, 4);
	ani_unhide = new Animation(data->ani_unhide_data, 7);
#ifdef __USE_OPENAL__
	initALSources();
#endif
}

BallTerran::BallTerran(float x, float y, float vx, float vy, BallStatus state, Game *game)
	: Ball(x, y, vx, vy, state, game) {
	r = 20;
	m = 30;
	if (ball_data_terran == NULL)
		ball_data_terran = new BallDataTerran();
	data = ball_data_terran;
	
	ani_spin = new Animation(data->ani_spin_data, 4);
	ani_birth = new Animation(data->ani_birth_data, 96);
	//ani_hide = new Animation(data->ani_hide_data, 4);
	//ani_unhide = new Animation(data->ani_unhide_data, 4);
}

BallProtoss::BallProtoss(float x, float y, float vx, float vy, BallStatus state, Game *game)
	: Ball(x, y, vx, vy, state, game) {
	r = 20;
	m = 5;
	if (ball_data_protoss == NULL)
		ball_data_protoss = new BallDataProtoss();
	data = ball_data_protoss;
	
	ani_spin = new Animation(data->ani_spin_data, 4);
	ani_birth = new Animation(data->ani_birth_data, 24);
	//ani_hide = new Animation(data->ani_hide_data, 4);
	//ani_unhide = new Animation(data->ani_unhide_data, 4);
}


#ifdef __USE_OPENAL__
ALboolean Ball::initALSources() {
	
	// Bind buffer with a source.
	alGenSources(S3D_NBUFFERS, this->s3d_sources);
	
	for (int i=0; i<S3D_NBUFFERS; i++) {
		alSourcei(this->s3d_sources[i], AL_BUFFER, this->data->s3d_buffers[i]);
		alSourcef(this->s3d_sources[i], AL_PITCH,  1.5f);
		if (alGetError() != AL_NO_ERROR) return AL_FALSE;
	}
	
	alSourcef(this->s3d_sources[0], AL_GAIN,     100.0f);
	alSourcef(this->s3d_sources[1], AL_GAIN,     50.0f);
	alSourcei(this->s3d_sources[0], AL_LOOPING,  AL_TRUE);
	alSourcei(this->s3d_sources[1], AL_LOOPING,  AL_FALSE);
	
	if (alGetError() != AL_NO_ERROR) return AL_FALSE;
	return AL_TRUE;
}
#endif


void Ball::setEffect(Combo effect, int effect_way, Player *player) {
	
	y0 = p.y;
	steps = 0;
	this->effect = effect;
	this->effect_way = (effect_way == 0) ? 1 : effect_way;
	cmb_player = player;
	
	switch (effect) {
		case CMB_NEUT_4:
			v.y = 0;
			cmb_level = rand() % (SCREEN_WIDTH/10);
			break;
			
		default:
			break;
	}
	
	initCombo();
	
}


bool Ball::checkCollisions(/*Window */int window) {
	if (!window) {
		return ((p.y - r) <= 0) || ((p.y + r) >= SCREEN_HEIGHT);
	} else {
		return true;
	}
}


void Ball::show(/*Window */int window) {
	
	// Advance
	
	way.x = (int) ((v.x+1)/fabs(v.x+1));
	way.y = (int) ((v.y+1)/fabs(v.y+1));
	
	//printf("ball->v.x\t %d\t ball->v.y\t %d\n", v.x, v.y);
	
	// Neutral Combos
	
	switch (effect) {
		
		case CMB_NULL:
			if (status == NORMAL)
				p += v;
			break;
			
			
		case CMB_NEUT_1:
			p.x += v.x;
			steps += roundf(fabs(v.x));
			
			p.y += -effect_way * (steps - SCREEN_WIDTH/2)/40;
			break;
			
		case CMB_NEUT_2:
			p.x += v.x;
			steps += roundf(fabs(v.x));
			
			//p.y += (effect/2)*(int)((pow((steps-(SCREEN_WIDTH/2)), 2) - 48000)/10000);
			//p.y = y0 - (effect/2)*(int)((pow((steps-(SCREEN_WIDTH/2)), 2)/800 - 128));
			p.y = y0 + effect_way * (int)((pow((steps-(SCREEN_WIDTH/2)), 2)/400 - 256));
			//TODO comparar cada nuevo y con el anterior para actualizar vy, por si hay que chocar
			break;
			
		case CMB_NEUT_3:
			p.x += v.x;
			steps += fabs(v.x);
			
			p.y = y0 + effect_way
					* (int)(sin(((float)steps/(float)SCREEN_WIDTH)*(2*M_PI))*(SCREEN_HEIGHT/4));
			break;
			
		case CMB_NEUT_4:
			p.x += v.x;
			steps += fabs(v.x);
			if (!(((int)roundf(steps) > (SCREEN_WIDTH*5/8 + cmb_level)))) {
				p.y = y0 + effect_way * exp(0.01*(steps+(SCREEN_WIDTH/4))) * sin(0.1*steps);//TODO derivar para tratar V, conocer P
			} else {
				setEffect(CMB_NEUT_1, rand()%2-1);
			}
			break;
			
		default:
			break;
			
	}
	
#ifdef __USE_OPENAL__
	// 3D sound possition and velocity update
	
	s3d_src_pos[0] = p.x;
	s3d_src_pos[1] = 0.0;
	s3d_src_pos[2] = p.y;
	
	s3d_src_vel[0] = v.x;
	s3d_src_vel[1] = 0.0;
	s3d_src_vel[2] = v.y;
	//printf("ball: %f %f %f\n", s3d_src_pos[0], s3d_src_pos[1], s3d_src_pos[2]);
	//printf("ball: %f %f %f\n", s3d_src_vel[0], s3d_src_vel[1], s3d_src_vel[2]);
	for (int i=0; i<S3D_NBUFFERS; i++) {
		alSourcefv(this->s3d_sources[i], AL_POSITION, s3d_src_pos);
		alSourcefv(this->s3d_sources[i], AL_VELOCITY, s3d_src_vel);
	}
#endif
	
	// Race specific combos
	
	handleCombo();
	
	
	// Detecting if ball collides with floor/ceiling
	
	if (this->checkCollisions(window)) {
#ifdef __USE_OPENAL__
	alSourcePlay(this->s3d_sources[1]);
#else
	playSound(data->s_bounce);
#endif
		
		v.y = -v.y;
		p.y += v.y;//FIXME cuidado con esto mientras no se actualiza vy en los efectos
		effect_way = -effect_way;
		
		y0 = 2*p.y - y0;
		
	}
	
	// Deciding how to draw the ball, depending on its state
	
	switch (status) {
		
		case NORMAL:
			ani_spin->drawFrame((int)roundf(p.x), (int)roundf(p.y), ANI_CENTERED);
			break;
		
		case EGG:
			ani_birth->drawFrame((int)roundf(p.x), (int)roundf(p.y), ANI_STATIC | ANI_CENTERED);
			if ((ani_birth->getFrame() == 15) && (birth_sound)) {
				playSound(data->s_birth);
				birth_sound = false;
			} else if (ani_birth->animationCompleted()) {
				status = NORMAL;
#ifdef __USE_OPENAL__
				alSourcePlay(this->s3d_sources[0]);
#endif
			}
			break;
			
		case HIDDING:
			ani_hide->drawFrame((int)roundf(p.x), (int)roundf(p.y), ANI_STATIC | ANI_CENTERED);
			if (ani_hide->animationCompleted())
				status = HIDDEN;
			break;
			
		case UNHIDDING:
			ani_unhide->drawFrame((int)roundf(p.x), (int)roundf(p.y), ANI_STATIC | ANI_CENTERED);
			if (ani_unhide->animationCompleted())
				status = NORMAL;
			break;
			
		default:
			break;
		
	}
	
}

void BallZerg::initCombo() {
	
	switch (effect) {
		
		case CMB_HIDE:
			// Level is used to tell when the ball is hidden
			cmb_level = 0;
			
		case CMB_ZERG_1:
			// Level is used to know when to add balls,
			// depending on the player's key striking speed
			cmb_level = 0;
			// Level_reached is the level the player
			// has reached striking the key
			cmb_level_reached = 0;
			// Key_counter counts how many times the player
			// strikes the hit button
			cmb_key_counter = 0;
			cmb_key_pressed = false;
			// Cmb_boost is used when when the player has
			// enough m1cro to strike the damn key (4-1)*4=12 times
			cmb_boost = 0;
			
			v.y = 0;
			v.x = def_vx * way.x;
			break;
			
		default:
			break;
	}
	
}

void BallTerran::initCombo() {
	
	
}

void BallProtoss::initCombo() {
	
	
}

void BallZerg::handleCombo() {
	
	switch (effect) {
			
		case CMB_HIDE:
			
			switch (cmb_level) {
				
				case 0: // Normal
					p.x += v.x;
					steps += roundf(fabs(v.x));
					if (steps > (SCREEN_WIDTH/20)) {
						status = HIDDING;
						++cmb_level;
					}
					break;
					
				case 1: // Hidding
					if (ani_hide->animationCompleted()) {
						p.y = 1 + (rand() % (SCREEN_HEIGHT-2*r));
						v.x = def_vx * way.x;
						v.y = 0;
						++cmb_level;
					}
					break;
					
				case 2: // Hidden
					p.x += v.x;
					steps += roundf(fabs(v.x));
					if(steps > (SCREEN_WIDTH/2)) {
						status = UNHIDDING;
						++cmb_level;
					}
					break;
					
				case 3: // Unhidding
					if (ani_unhide->animationCompleted()) {
						v.x *= 1.5;
						effect = CMB_NULL;
						++cmb_level;
					}
					break;
					
				default:
					break;
			}
			
			break;
			
		case CMB_ZERG_1:
			
			p += v;
			
			// Incrementing cmb_key_counter
			if ((!control->isPressed(KEY_B1, cmb_player->n_player, 0)) && cmb_key_pressed) {
				++cmb_key_counter;
				cmb_key_pressed = false;
			} else if (control->isPressed(KEY_B1, cmb_player->n_player, 0)) {
				cmb_key_pressed = true;
			}
			
			// Incrementing cmb_level
			if ((cmb_level_reached = 1+(cmb_key_counter/4)) == 4) {
				cmb_boost = true;
				cmb_key_counter = -cmb_key_counter;
			}
			
			// Generating two new balls if the m1cro is enough
			if (cmb_level < cmb_level_reached) {
				cmb_level = cmb_level_reached;
				game->balls.push_back(new BallZerg(p.x + (((SCREEN_WIDTH/100)*r*way.x)/cmb_level),
												   p.y - (cmb_level*2*r),
												   v.x, v.y, UNHIDDING, game));
				game->balls.push_back(new BallZerg(p.x + (((SCREEN_WIDTH/100)*r*way.x)/cmb_level),
												   p.y + (cmb_level*2*r),
												   v.x, v.y, UNHIDDING, game));
			} else if (cmb_boost) {
				game->balls.push_back(new BallZerg(SCREEN_WIDTH/2 - (way.x*(SCREEN_WIDTH/4)),
												SCREEN_HEIGHT/2,
												way.x*def_vx, 0, EGG, game));
				// TODO unleash teh beast
				cmb_boost = false;
			}
			break;
			
		default:
			break;
		
		//TODO y=(200./(1+(x.^2)/640))-200; plot(x,y); axis([0, 640, -240, 240]);
	}
	
}

void BallTerran::handleCombo() {
	
	switch (effect) {
			
		
			
		default:
			break;
		
	}
	
}

void BallProtoss::handleCombo() {
	
	switch (effect) {
			
		
			
		default:
			break;
		
	}
	
}
