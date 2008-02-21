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
#include <math.h>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"

#ifdef __USE_OPENAL__
#include <AL/al.h>
#endif

#include "main.h"
#include "audio.h"
#include "error.h"
#include "CBall.h"
#include "CGame.h"
#include "CGraphics.h"
#include "CController.h"
#include "CControl.h"
#include "CAnimation.h"
#include "CPlayer.h"


extern SDL_Surface *screen;
extern Control *control;


PlayerZerg::PlayerZerg(int x, int direction, int type, const char *name, int n_player)
	: Player(x, direction, type, name, n_player) {
	
	sprite = graphics->load_image(P_ZERG_PLAYER);
	portrait = graphics->load_image(P_ZERG_PORTRAIT);
	comboflash = new Animation(P_ZERG_COMBOFLASH, 2, 2, COMBOFLASH_V);
	s_hit = loadSound(S_ZERG_HIT);
	s_win = loadSound(S_ZERG_WIN);
	s_win2 = loadSound(S_ZERG_WIN2);
	s_lose = loadSound(S_ZERG_LOSE);
	m = 110;
	
	if (direction == RIGHT)
		title = graphics->load_image(P_ZERG_TITLE_L);
	else
		title = graphics->load_image(P_ZERG_TITLE_R);
		
	setup();
	
}

PlayerTerran::PlayerTerran(int x, int direction, int type, const char *name, int n_player)
	: Player(x, direction, type, name, n_player) {
	
	sprite = graphics->load_image(P_TERRAN_PLAYER);
	portrait = graphics->load_image(P_TERRAN_PORTRAIT);
	comboflash = new Animation(P_TERRAN_COMBOFLASH, 2, 2, COMBOFLASH_V);
	s_hit = loadSound(S_TERRAN_HIT);
	s_win = loadSound(S_TERRAN_WIN);
	s_win2 = loadSound(S_TERRAN_WIN2);
	s_lose = loadSound(S_TERRAN_LOSE);
	m = 100;
	
	if (direction == RIGHT)
		title = graphics->load_image(P_TERRAN_TITLE_L);
	else
		title = graphics->load_image(P_TERRAN_TITLE_R);
	
	setup();
	
}

PlayerProtoss::PlayerProtoss(int x, int direction, int type, const char *name, int n_player)
	: Player(x, direction, type, name, n_player) {
	
	sprite = graphics->load_image(P_PROTOSS_PLAYER);
	portrait = graphics->load_image(P_PROTOSS_PORTRAIT);
	comboflash = new Animation(P_PROTOSS_COMBOFLASH, 2, 2, COMBOFLASH_V);
	s_hit = loadSound(S_PROTOSS_HIT);
	s_win = loadSound(S_PROTOSS_WIN);
	s_win2 = loadSound(S_PROTOSS_WIN2);
	s_lose = loadSound(S_PROTOSS_LOSE);
	m = 90;
	
	if (direction == RIGHT)
		title = graphics->load_image(P_PROTOSS_TITLE_L);
	else
		title = graphics->load_image(P_PROTOSS_TITLE_R);
	setup();
}

Player::Player(int x, int direction, int type, const char *name, int n_player)
	: direction(direction),
	last_key(KEY_NULL),
	name(name),
	n_player(n_player),
	type(type),
	life_max(100), //TODO modos de vida
	x(x), vy(3),
	v(Vector(0, 0)),
	wins(0) {
	
	if (direction == RIGHT) {
		plr_forward = KEY_RIGHT;
		plr_back = KEY_LEFT;
	} else {
		plr_forward = KEY_LEFT;
		plr_back = KEY_RIGHT;
	}
	
}

void Player::setup() {
	
	if (direction == RIGHT) {
		sprite = graphics->flip(sprite, FLIP_HORIZONTAL);
		portrait = graphics->flip(portrait, FLIP_HORIZONTAL);
	}
	
	//graphics->setTransparent(sprite, &color_magenta);
	graphics->setTransparent(sprite);
	
	r = sprite->w/2;
	p1.y = (SCREEN_HEIGHT - sprite->h)/2 + r;
	p2.y = (SCREEN_HEIGHT + sprite->h)/2 - r;
	
	switch (x) {
		case LEFT:
			p1.x = p2.x = r;
			break;
			
		case RIGHT:
			p1.x = p2.x = SCREEN_WIDTH - r;
			break;
		
		default:
			p1.x = p2.x = x;
	}
	
#ifdef __USE_OPENAL__
	if (type != COMPUTER)
		initALListener();
#endif
	reset();
	
}

#ifdef __USE_OPENAL__

void Player::initALListener() {
	s3d_lst_pos[1] =  0.0;
	s3d_lst_vel[1] =  0.0;
	s3d_lst_ori[0] = -1.0;
	s3d_lst_ori[1] =  0.0;
	s3d_lst_ori[2] =  0.0;
	s3d_lst_ori[3] =  0.0;
	s3d_lst_ori[4] =  1.0;
	s3d_lst_ori[5] =  0.0;
	
	alListenerfv(AL_ORIENTATION, s3d_lst_ori);
	
}

ALboolean Player::updateALListener() {
	s3d_lst_pos[0] = p1.x;
	s3d_lst_pos[2] = p1.y;
	
	s3d_lst_vel[0] = v.x;
	s3d_lst_vel[2] = v.y;
	
	//printf("player: %f %f %f\n", s3d_lst_pos[0], s3d_lst_pos[1], s3d_lst_pos[2]);
	
	alListenerfv(AL_POSITION,    s3d_lst_pos);
	alListenerfv(AL_VELOCITY,    s3d_lst_vel);
	
	if (alGetError() != AL_NO_ERROR) return AL_FALSE;
	return AL_TRUE;
}
#endif

Player::~Player() {
	
	SDL_FreeSurface(sprite);
	Mix_FreeChunk(s_hit);
	Mix_FreeChunk(s_win);
	Mix_FreeChunk(s_lose);
	
}

void Player::updateCombo() {
	
	if (SDL_GetTicks() >= combo_ticks)
		combo_state = 0;
	
	/* (C-FSM) Combo - Finite State Machine */
	/*     ^- no, that's not Flying Spaghetti Monster. */
	
	//TODO: script that takes a readable strings definig the combos and generates spaguetti-code
	
	switch (combo_state) {
		
		case 0: /* No input */
			
			if ( control->isPressed(KEY_DOWN, n_player, 0) ) {
				setComboState(1);
			} else
			break;
			
		case 1: /* D */
			
			if ( control->isPressed(KEY_DOWN, n_player, 0) &&
				control->isPressed(plr_forward, n_player, 0)) {
				setComboState(2);
			} else
			if ( control->isPressed(KEY_DOWN, n_player, 0) &&
				 control->isPressed(plr_back, n_player, 0)) {
				setComboState(3);
			}
			break;
			
		case 2: /* D, D+R */
			
			if (!control->isPressed(KEY_DOWN, n_player, 0) &&
				control->isPressed(plr_forward, n_player, 0)) {
				combo = CMB_NEUT_1;
				setComboState(0);
			}
			break;
			
		case 3: /* D, D+L */
			
			if (!control->isPressed(KEY_DOWN, n_player, 0) &&
				 control->isPressed(plr_back, n_player, 0)) {
				
				combo = CMB_NEUT_2;
				setComboState(0);
			}
			break;
			
		
		default: die(ERR_CUSTOM, "Player::updateCombo: Invalid state");
	}
	
}

void Player::handle_input() {
	
	updateCombo();
	
	// Updating player velocity
	if (control->isPressed(KEY_UP, n_player, 0)) {
		v.y = -vy;
	} else if (control->isPressed(KEY_DOWN, n_player, 0)) {
		v.y = vy;
	} else
		v.y = 0;
	
}


void Player::move(std::vector<Ball*> balls) {

	int screen_mid_y = SCREEN_HEIGHT/2;
	
	/* If the ball is headed to the player, and the distance
	 * between the ball and the player is less than SCREEN_WIDTH/2,
	 * the player tries to catch the ball. */
	float player_center = (p1.y + p2.y)/2.0;
	float ai_v = 2.0f/SCREEN_HEIGHT;
	if ((balls[0]->v.x < 0) && (fabs(balls[0]->p.x - p1.x) < screen_mid_y))
		v.y = vy                               // Base velocity
			* ai_v                             // Artificial Inteligente speed
			* (balls[0]->p.y - player_center)  // Way and distance to the ball
			* balls[0]->v.module();            // Ball speed
	
	/* If the ball is headed to the opponent, the player
	 * goes back to the middle of the screen. */
	else if ((p1.y > screen_mid_y) || (p2.y < screen_mid_y))
		v.y = vy * ai_v * (screen_mid_y - player_center);
	
	else
		v.y = 0.0f;
	
}


void Player::show() {
	
	int old_y = (int)round(p1.y) - r;
	
	if (((v.y < 0) && ((p1.y-r) > 0))
			 || ((v.y > 0) && ((p2.y+r) < SCREEN_HEIGHT))) {
		p1.y += v.y;
		p2.y += v.y;
	}
	
	if (combo == CMB_NULL)
		// Player
		graphics->add_rect(graphics->apply_surface((int)round(p1.x) - r, (int)round(p1.y) - r,
						   sprite, screen, NULL, V_ANIM, 0, old_y));
	
	else {
		// Background animation
		comboflash->drawFrame((int)round(p1.x) - r, (int)round(p1.y) - r);
		
		// Player
		graphics->add_rect(graphics->apply_surface((int)round(p1.x) - r, (int)round(p1.y) - r,
						   sprite, screen, NULL, V_ANIM, 0, old_y));
		
	}
	
#ifdef __USE_OPENAL__
	if (type != COMPUTER)
		updateALListener();
#endif
	
}


void Player::hurt() {
	
	//setLife(life-10);
	
}


void Player::setLife(int _life) {
	
	life = _life;
	lifeg = (life * LIFE_MAX) / life_max;
	
}


void Player::reset() {
	
	setLife(life_max);
	lifegi = 0;
	setComboState(0);
	combo = CMB_NULL;
	
#ifdef __USE_OPENAL__
	if (type != COMPUTER)
		updateALListener();
#endif
	
}
