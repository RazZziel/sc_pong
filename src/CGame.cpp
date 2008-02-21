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
#include <time.h>
#include <string.h>

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_rotozoom.h"

#include "error.h"
#include "audio.h"
#include "main.h"
#include "menus.h"
#include "CGraphics.h"
#include "CFramerate.h"
#include "CPlayer.h"
#include "CBall.h"
#include "CPhysics.h"
#include "CController.h"
#include "CControl.h"
#include "CGame.h"


extern SDL_Surface *screen;
extern Graphics *graphics;
extern Control *control;
extern Framerate *framerate;
extern Physics *physics;


Game::Game(Player *player0, Player *player1, int rounds) : round(1), rounds(rounds) {
	
	multiplayer = false;
	
	life[0] = life[1] = rect_zero;
	
	// Players
	players[0] = player0;
	players[1] = player1;
	
	// Pixmaps
	background = graphics->load_image(P_BACKGROUND, NULL);
	covercat = graphics->load_image(P_COVERCAT);
	coversnake = graphics->load_image(P_COVERSNAKE, &color_white);
	life_empty = graphics->load_image(P_LIFE_EMPTY);
	life_full = graphics->load_image(P_LIFE_FULL);
	
	
	// Music
	s_music = loadMusic(M_RAZIOZERG);
	
	life[0].h = life[1].h =  life_full->h;
	//TODO inicializar todo esto en vez de asignarlo
	
	srand(time(NULL));
	
}


Game::~Game() {
	
	SDL_FreeSurface(background);
	Mix_FreeMusic(s_music);
	
}



void Game::setup_player_stats() {
	
	life_x0 = LIFE_X0 - life_empty->w/2;
	life_x1 = LIFE_X1 - life_empty->w/2;
	
	//TODO cuando decida el formato final, blitear parte al fondo, no a screen
	
	int offset = life_empty->h/2;
	
	// Portrait
	
	graphics->apply_surface(life_x1 - players[1]->portrait->w + offset,
							LIFE_Y - (life_empty->h/2),
							players[1]->portrait, background, NULL);
	graphics->apply_surface(life_x0 + life_empty->w - offset,
							LIFE_Y - (life_empty->h/2),
							players[0]->portrait, background, NULL);
	
	// Empty life
	
	graphics->apply_surface(life_x0, LIFE_Y, life_empty, background, NULL);
	graphics->apply_surface(life_x1, LIFE_Y, life_empty, background, NULL);
	
	// Name
	
	SDL_Surface *name_l = graphics->renderTextFNT(players[1]->name, f_main_menu, &c_mm_greenh);
	SDL_Surface *name_r = graphics->renderTextFNT(players[0]->name, f_main_menu, &c_mm_greenh);
	
	graphics->apply_surface(life_x1 + offset*2,
				  LIFE_Y + life_empty->h + offset/2,
				  name_l, background);
	graphics->apply_surface(life_x0 - offset*2 + (life_empty->w),
				  LIFE_Y + life_empty->h + offset/2,
				  name_r, background, NULL, H_RIGHT);
	
	SDL_FreeSurface(name_l);
	SDL_FreeSurface(name_r);
	
}

void Game::draw_player_stats() {
	
	for (int i=0; i<2; i++) {
		if ((int)players[i]->lifegi != players[i]->lifeg) {
			int inc = (players[i]->lifeg - (int)players[i]->lifegi);
			players[i]->lifegi += ((float)inc)/((float)(abs(inc)*4));
		}
		life[i].w = LIFE_OFFSET + LIFE_STEP * (int)players[i]->lifegi;
	}
	
	life[1].x = life_full->w - life[1].w;
	
	graphics->apply_surface(life_x1 + (life_full->w) - life[1].w, LIFE_Y, life_full, screen, &life[1]);
	graphics->apply_surface(life_x0,                              LIFE_Y, life_full, screen, &life[0]);
	
	SDL_Rect *rect = (SDL_Rect*) malloc(sizeof(SDL_Rect));
	rect->x = life_x1;
	rect->y = LIFE_Y;
	rect->w = (life_x0 - life_x1) + life_full->w;
	rect->h = life_full->h;
	
	graphics->add_rect(rect);
	
}


void Game::configure_multiplayer() {
	
	multiplayer = true;
	
}


void Game::start() {
	
	framerate->setFPS(GAME_FPS);
	Mix_FadeOutMusic(500);
	
	setup_player_stats();
	balls.push_back(new BallZerg(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 5, 0, EGG, this));
	//balls.push_back(new BallZerg(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.2, 2, EGG, this)); // OpenAL test
	balls[0]->mainBall = true;
	
	bool exit = false;
	
	intro();
	
	//playMusic(s_music);//DEBUG
	
	while ((!exit) && (round <= rounds)) {
		
		round_title();
		
		exit = game_loop();
		
		if (!exit) {
			
			++(winner->wins);
			
			playSound(loser->s_lose);
			
			if (round == rounds) {
				if (players[0]->wins == players[1]->wins) {
					playSound(winner->s_win);
					rounds++;
				} else {
					playSound(winner->s_win2);
					end();
				}
				
			} else
				playSound(winner->s_win);
			
			
			round++;
			
			players[0]->reset();
			players[1]->reset();
			
		}
		
	}
	
	Mix_FadeOutMusic(1000);
	
}

void Game::intro() {
	
	Mix_Chunk *s_expl = loadSound(S_FLASH1);
	
	SDL_Surface *fade_white = graphics->copySurface(screen, false);
	SDL_FillRect(fade_white, NULL, 0x00FFFFFF);
	
	float m = -479.0/160.0;
	int vx = 20;
	
	int x1 = (int) (SCREEN_HEIGHT/m);
	int y1 = SCREEN_HEIGHT;
	int x2 = (int) (-SCREEN_HEIGHT/m);
	int y2 = -SCREEN_HEIGHT;
	
	bool exit = false;
	bool flash = true;
	bool flash_sound = true;
	
	playSound(balls[0]->data->s_start);
	
	while(!exit) {
		
		control->updateState();
		if (control->isPressed(KEY_BACK) || control->isPressed(KEY_OK))
			exit = true;
		
		SDL_FillRect(screen, NULL, 0x00000000);
		graphics->apply_surface(x1, y1, players[0]->title, screen);
		graphics->apply_surface(x2, y2, players[1]->title, screen);
		
		if (x1 != x2) {
			
			x1 += vx;
			x2 -= vx;
			y1 = (int) roundf(m*x1);
			y2 = (int) roundf(m*x2);
			
		} else {
			
			if (flash) {
				
				if (flash_sound) {
					playSound(s_expl);
					flash_sound = false;
				}
				
				graphics->apply_surface(0, 0, fade_white, screen);
				SDL_SetAlpha(fade_white, SDL_SRCALPHA, fade_white->format->alpha-5);
				flash = (fade_white->format->alpha > 0);
				
			}
			
			if (!Mix_Playing(-1))
				exit = true;
			
		}
		
		
		framerate->tic();
		graphics->refresh_whole_screen();
		framerate->delay();
		
	}
	
	Mix_FadeOutChannel(-1, 500);
	
}

void Game::round_title() {
	
	SDL_Surface *s_txt, *s_txt2, *s_txt2_resize;
	char round_no[2];
	
	if ((round == rounds) && (rounds%2 != 0)) {
		
		s_txt = graphics->renderTextTTF("FINAL", &ttf_starcraft, 3, &color_red);
		s_txt2 = graphics->renderTextTTF("ROUND", &ttf_starcraft, 3, &color_red);
		
	} else {
		
		s_txt = graphics->renderTextTTF("ROUND", &ttf_starcraft, 3, &color_red);
		sprintf(round_no, "%d", round);
		s_txt2 = graphics->renderTextTTF(round_no, &ttf_starcraft, 3, &color_red);
		
	}
	
	double zoom_factor = 1.1;
	int max_resize = s_txt2->w * 3;
	int txt_x = SCREEN_WIDTH*2;
	int txt_vx = SCREEN_WIDTH/32; // 20 pixels
	
	if (round == 1);
		playSound(balls[0]->data->s_ready);
	
	bool exit = false;
	
	while ((txt_x - s_txt->w/2) > SCREEN_WIDTH + 32) { txt_x -= txt_vx; }
	
	while (!exit) {
		
		control->updateState();
		
		if (control->isPressed(KEY_BACK) || control->isPressed(KEY_OK))
			exit = true;
			
		else {
			
			graphics->apply_surface(0, 0, background, screen);
			draw_player_stats();
			
			players[0]->show();
			players[1]->show();
			draw_player_stats();
			
			
			txt_x -= txt_vx;
			graphics->apply_surface(txt_x, (SCREEN_HEIGHT)/2, s_txt, screen, NULL, (H_CENTER|V_CENTER));
			
			if ((txt_x < (SCREEN_WIDTH/2))) {
				
				graphics->apply_surface(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, s_txt2, screen, NULL, (H_CENTER|V_CENTER));
				
				if (s_txt2->w < max_resize) {
					s_txt2_resize = zoomSurface(s_txt2, zoom_factor, zoom_factor, SMOOTHING_OFF);
					SDL_FreeSurface(s_txt2);
					s_txt2 = s_txt2_resize;
				}
				
			}
			
			if ((txt_x + s_txt->w/2) <= 0)
				exit = true;
			
		}
		
		framerate->tic();
		graphics->refresh_whole_screen();
		framerate->delay();
		
	}
	
	Mix_FadeOutChannel(-1, 500);
	SDL_FreeSurface(s_txt);
	SDL_FreeSurface(s_txt2);
	
}


void Game::end() {
	
	SDL_Surface *s_text;
	
	// U WIN
	if ((winner->type == HUMAN) && ((loser->type == COMPUTER) || (loser->type == NETHUMAN))) {
		
		s_text = graphics->renderTextTTF("U WIN", &ttf_starcraft, 2);
		
	// U FAIL
	} else if ((loser->type == HUMAN) && ((winner->type == COMPUTER) || (winner->type == NETHUMAN))) {
		
		s_text = graphics->renderTextTTF("U FAIL", &ttf_starcraft, 2);
		
	// SOMEBODY WINS
	} else if (winner->type == loser->type) {
		
		char *text = (char*) malloc(sizeof(char)*(6 + strlen(winner->name)));
		sprintf(text, "%s WINS", winner->name);
		s_text = graphics->renderTextTTF(text, &ttf_starcraft, 2);
		
	}
	
	graphics->add_rect(graphics->apply_surface(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,
					   s_text, screen, NULL, (H_CENTER | V_CENTER)));
	SDL_FreeSurface(s_text);
	
	graphics->refresh_screen();
	
	SDL_Delay(3000);
	
}


bool Game::game_loop() {
	
	graphics->apply_surface(0, 0, background, screen);
	graphics->refresh_whole_screen();
	
	bool exit = false;
	winner = NULL;
	loser = NULL;
	
	while((!exit) && (!winner)) {
		
		/* Input handling */
		
		
		control->updateState();
		
		if ((control->isPressed(KEY_BACK)) && (game_menu(this))) {
			exit = true;
			
		} else {
			
			
			if (players[0]->type != COMPUTER)	players[0]->handle_input();
			else					players[0]->move(balls);
			
			if (players[1]->type != COMPUTER)	players[1]->handle_input();
			else					players[1]->move(balls);
			
			
			/* Collision detection */
			
			
			physics->checkPlayerCollisions(&balls, players, 2);
			physics->checkBallCollisions(&balls);
			//printf("%f %f\n", balls[0]->v.module(), balls[1]->v.module());
			
			
			/* Screen redraw */
			
			
			graphics->apply_surface(0, 0, background, screen);
			
			draw_player_stats();
			
			
			// Players
			
			players[0]->show();
			players[1]->show();
			
			/* Checking if there is a winner and drawing balls */
			
			for (Uint8 i=0; i<balls.size(); i++) {
				
				if ((balls[i]->p.x + balls[i]->r) <= 0) { // Left
					
					players[0]->hurt();
					players[0]->combo = CMB_NULL;
					
					if (players[0]->life <= 0) {
						winner = players[1];
						loser = players[0];
					}
					
					graphics->refresh_whole_screen(false);
					if (i == 0) {
						balls[i]->resetPosition();
						balls[i]->setVelocity(-5, 0);
						balls[i]->birth();
					} else {
						delete balls[i];
						balls.erase(balls.begin()+i);
					}
					
				} else if ((balls[i]->p.x - balls[i]->r) >= SCREEN_WIDTH) { // Right
					
					players[1]->hurt();
					players[1]->combo = CMB_NULL;
					
					if (players[1]->life <= 0) {
						winner = players[0];
						loser = players[1];
					}
					
					graphics->refresh_whole_screen(false);
					if (i == 0) {
						balls[i]->resetPosition();
						balls[i]->setVelocity(5, 0);
						balls[i]->birth();
					} else {
						delete balls[i];
						balls.erase(balls.begin()+i);
					}
					
				} else
					balls[i]->show();
				
			}
			
			framerate->tic();
			graphics->refresh_screen();
			framerate->delay();
			
		}
		
	}
	
	for (Uint8 i=0; i<balls.size(); i++) {
		delete balls[i];
	}
	
	return exit;
	
}
