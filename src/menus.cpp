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
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include "main.h"
#include "error.h"
#include "menus.h"
#include "audio.h"
#include "widgets.h"
#include "CGraphics.h"
#include "CControl.h"
#include "CCtrlNet.h"
#include "CAnimation.h"
#include "CGame.h"


extern SDL_Surface *screen;
extern Graphics *graphics;
extern Framerate *framerate;
extern Control *control;

Mix_Chunk *s_swishin;
Mix_Chunk *s_swishout;
Mix_Chunk *s_swishlock;
Mix_Chunk *s_mouseover;
Mix_Chunk *s_mousedown2;





void changeName(char *actual_name) {
	
	char new_name[NAME_MAX_LENGTH];
	*new_name = '\0';
	printf("Change name (%s): ", actual_name);
	scanf("%s", new_name);
	printf(":)\n");
	
	if (*new_name)
		strcpy(actual_name, new_name);
	
}


void menu_options() {
	
	playSound(s_swishin);
	
	SDL_Surface *background = graphics->load_image(P_BG_OPTIONS, NULL);
	
	
	// Configuration parameters
	
	char name[NAME_MAX_LENGTH];
	strcpy(name, "Juiporentenderlas");//TODO lol
	int volm = Mix_VolumeMusic(-1)*SOUND_BAR_STEPS/MIX_MAX_VOLUME;
	int vols = Mix_Volume(-1, -1)*SOUND_BAR_STEPS/MIX_MAX_VOLUME;
	bool fs = screen->flags & SDL_FULLSCREEN;
	
	
	// Labels attached to the background
	
	const char *text[] = { "_N_ame",
		"_M_usic volume",
		"_S_ound volume",
		"_F_ullscreen",
		"_C_onfigure keys",
		"ON",
		"OFF" };
	
	
	SDL_Surface *s_text;
	
	// Option titles, blitted to background
	for (int i=0; i<5; i++) {
		s_text = graphics->renderTextFNT(text[i], f_main_menu, &c_mm_green, &c_mm_greenh, '_');
		graphics->apply_surface(LEFT_COL, POS(i), s_text, background);
		SDL_FreeSurface(s_text);
	}
	
	// Selected (highlighted) option
	SDL_Surface *s_selected = graphics->renderTextFNT(text[0], f_main_menu, &c_mm_greenh, NULL, '_');
	
	// Name
	SDL_Surface *s_name = graphics->renderTextFNT(name, f_main_menu, &c_mm_green);
	
	// Volume bars
	//    Empty bars, blitted to background
	SDL_Surface *empty_bar = graphics->load_image(P_BAR_EMPTY);
	graphics->apply_surface(RIGHT_COL, POS(1), empty_bar, background);
	graphics->apply_surface(RIGHT_COL, POS(2), empty_bar, background);
	SDL_FreeSurface(empty_bar);
	
	//    Full bars
	SDL_Surface *bar = graphics->load_image(P_BAR_FULL);
	SDL_Rect bar_m_rect = {0, 0, VOL(volm), bar->h};  // Drawing rect
	SDL_Rect bar_s_rect = {0, 0, VOL(vols), bar->h};  // Drawing recr
	SDL_Rect bar_m_rect_refresh = {RIGHT_COL + VOL(volm) - VOL_STEP, POS(1), VOL_STEP, bar->h};  // Updating rect
	SDL_Rect bar_s_rect_refresh = {RIGHT_COL + VOL(volm) - VOL_STEP, POS(2), VOL_STEP, bar->h};  // Updating recr
	
	
	// Fullscreen ON and OFF
	s_text = graphics->renderTextFNT(text[5], f_main_menu, &c_mm_green);	// ON
	graphics->apply_surface(RIGHT_COL, POS(3), s_text, background);
	SDL_FreeSurface(s_text);
	
	s_text = graphics->renderTextFNT(text[6], f_main_menu, &c_mm_green);	// OFF
	graphics->apply_surface(RIGHT_COL*5/4, POS(3), s_text, background);
	SDL_FreeSurface(s_text);
	
	// Selected fullscreen option
	int fs_x = (fs) ? RIGHT_COL : RIGHT_COL2;
	SDL_Surface *selected_fs = graphics->renderTextFNT( ((fs) ? (text[5]) : (text[6])) , f_main_menu, &c_mm_greenh);
	
	
	MenuOpt option = OPT_NAME;
	bool exit = false;
	
	control->flushEvents();
	graphics->refresh_whole_screen(false);
	
	while(!exit) {
		
		graphics->apply_surface(0, 0, background, screen);
		
		// Iniput handling
		
		control->updateState();
		
		if (control->isPressed(KEY_BACK)) {
			playSound(s_mousedown2);
			playSound(s_swishout);
			exit = true;
			
		} else if (control->isPressed(KEY_UP)) {
			playSound(s_mouseover);
			option--;
			
			s_selected = graphics->renderTextFNT(text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			
			// Refreshing the whole screen on each selection switch may be rather dirty, but keeping
			// track of actual and old selection rects is a worthless cpu and memory everhead.
			
		} else if (control->isPressed(KEY_DOWN)) {
			playSound(s_mouseover);
			option++;
			
			s_selected = graphics->renderTextFNT(text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			
		} else if (control->isPressed(SDLK_n)) {
			playSound(s_mouseover);
			option = OPT_NAME;
			
			s_selected = graphics->renderTextFNT(text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->add_rect(graphics->apply_surface(LEFT_COL, POS(option), s_selected, screen));
			graphics->refresh_screen();
			
			changeName(name);
			s_name = graphics->renderTextFNT(name, f_main_menu, &c_mm_green);
			graphics->refresh_whole_screen(false); // To prevent glitches when switchng to a shorter name
			
		} else if (control->isPressed(SDLK_m)) {
			playSound(s_mouseover);
			option = OPT_VOLM;
			
			s_selected = graphics->renderTextFNT(text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			
			
		} else if (control->isPressed(SDLK_s)) {
			playSound(s_mouseover);
			option = OPT_VOLS;
			
			s_selected = graphics->renderTextFNT(text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			
			
		} else if (control->isPressed(SDLK_f)) {
			playSound(s_mouseover);
			option = OPT_FS;
			
			s_selected = graphics->renderTextFNT(text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			
			fs = !fs;
			graphics->toggle_fullscreen();
			selected_fs = graphics->renderTextFNT( (fs) ? text[5] : text[6] , f_main_menu, &c_mm_greenh);
			fs_x = (fs) ? RIGHT_COL : RIGHT_COL2;
			
			
		} else if (control->isPressed(SDLK_c)) {
			playSound(s_mouseover);
			option = OPT_KEYB;
			
			s_selected = graphics->renderTextFNT(text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			
		}
		
		
		switch(option) {
				
			case OPT_NAME:
				if (control->isPressed(KEY_OK)) {
					playSound(s_mousedown2);
					playSound(s_swishin);
					
					changeName(name);
					s_name = graphics->renderTextFNT(name, f_main_menu, &c_mm_green);
					graphics->refresh_whole_screen(false); // To prevent glitches when switchng to a shorter name
					
				}
				break;
				
			case OPT_VOLM:
				
				if (control->isPressed(KEY_RIGHT, 0, 50)) {
					if (volm < SOUND_BAR_STEPS) {
						Mix_VolumeMusic(++volm*MIX_MAX_VOLUME/SOUND_BAR_STEPS);
						playSound(s_mouseover);
						
						bar_m_rect.w += VOL_STEP;
						bar_m_rect_refresh.x += VOL_STEP;
					}
				} else if (control->isPressed(KEY_LEFT, 0, 50)) {
					if (volm > 0) {
						Mix_VolumeMusic(--volm*MIX_MAX_VOLUME/SOUND_BAR_STEPS);
						playSound(s_mouseover);
						
						graphics->add_rect(&bar_m_rect_refresh);
						bar_m_rect.w -= VOL_STEP;
						bar_m_rect_refresh.x -= VOL_STEP;
					}
				}
				break;
				
			case OPT_VOLS:
				if (control->isPressed(KEY_RIGHT, 0, 50)) {
					if (vols < SOUND_BAR_STEPS) {
						Mix_Volume(-1, ++vols*MIX_MAX_VOLUME/SOUND_BAR_STEPS);
						playSound(s_mouseover);
						
						bar_s_rect.w += VOL_STEP;
						bar_s_rect_refresh.x += VOL_STEP;
					}
				} else if (control->isPressed(KEY_LEFT, 0, 50)) {
					if (vols > 0) {
						Mix_Volume(-1, --vols*MIX_MAX_VOLUME/SOUND_BAR_STEPS);
						playSound(s_mouseover);
						
						graphics->add_rect(&bar_s_rect_refresh);
						bar_s_rect.w -= VOL_STEP;
						bar_s_rect_refresh.x -= VOL_STEP;
					}
				}
				break;
				
			case OPT_FS:
				if (control->isPressed(KEY_RIGHT)) {
					if (fs) {
						playSound(s_mouseover);
						
						fs = false;
						graphics->toggle_fullscreen();
						selected_fs = graphics->renderTextFNT(text[6], f_main_menu, &c_mm_greenh);
						fs_x = RIGHT_COL2;
					}
				} else if (control->isPressed(KEY_LEFT)) {
					if (!fs) {
						playSound(s_mouseover);
						
						fs = true;
						graphics->toggle_fullscreen();
						selected_fs = graphics->renderTextFNT(text[5], f_main_menu, &c_mm_greenh);
						fs_x = RIGHT_COL;
					}
				} else if (control->isPressed(KEY_OK)) {
					playSound(s_mouseover);
					
					fs = !fs;
					graphics->toggle_fullscreen();
					selected_fs = graphics->renderTextFNT( (fs) ? text[5] : text[6] , f_main_menu, &c_mm_greenh);
					fs_x = (fs) ? RIGHT_COL : RIGHT_COL2;
					
				}
				break;
				
			case OPT_KEYB:
				if (control->isPressed(KEY_OK)) {
					playSound(s_mousedown2);
					playSound(s_swishin);
					
					printf("Configure keys\n");
				}
				break;
				
			default:
				break;
			
		}
		
		/* Redraw */
		
		// Background
		graphics->apply_surface(0, 0, background, screen);
		
		// Drawing selected (highlighted) option
		graphics->add_rect(graphics->apply_surface(LEFT_COL, POS(option), s_selected, screen));
		
		// Name
		graphics->add_rect(graphics->apply_surface(RIGHT_COL, POS(0), s_name, screen));
		
		// Volumes
		graphics->add_rect(graphics->apply_surface(RIGHT_COL, POS(1), bar, screen, &bar_m_rect));
		graphics->add_rect(graphics->apply_surface(RIGHT_COL, POS(2), bar, screen, &bar_s_rect));
		
		// Fullscreen (selected option)
		graphics->add_rect(graphics->apply_surface(fs_x, POS(3), selected_fs, screen));
		
		
		framerate->tic();
		graphics->refresh_screen();
		framerate->delay();
		
	}
	
	
	// Cleanup
	SDL_FreeSurface(background);
	SDL_FreeSurface(bar);
	SDL_FreeSurface(selected_fs);
	SDL_FreeSurface(s_name);
	SDL_FreeSurface(s_selected);
	
}


int menu_multiplayer() {
	
	return 0;
	
}


int menu_race(Animation **races) {
	
	SDL_Surface *background = graphics->load_image(P_BG_RACE_MENU, NULL);
	
	SDL_Surface *fade_black = graphics->copySurface(screen, false);
	SDL_FillRect(fade_black, 0, 0x00000000);
	SDL_SetAlpha(fade_black, SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT);
	
	SDL_Surface *s_arm_l = graphics->load_image(P_WDG_ARML);
	SDL_Surface *s_arm_r = graphics->load_image(P_WDG_ARMR);
	dot r_arm_l = {-s_arm_l->w, 362};
	dot r_arm_r = {SCREEN_WIDTH, 380};
	
	//s_text = graphics->renderTextFNT(rm_text[i], f_main_menu, &c_mm_green, &c_mm_greenh, '_');
	
	
	const char *rm_text[] = {
		"_Z_erg",
		"_T_erran",
		"_P_rotoss",
		"Say _m_oo",
		"Do _n_othing",
		"_C_ancel"
	};
	dot rm_text_pos[] = {
		{89, 329},
		{283, 240},
		{497, 323},
		{343, 16},
		{98, 36},
		{319, 36}
	};
	
	
	// Text blitted to background
	SDL_Surface *s_rm_text[3];
	for (int i=0; i<3; i++) {
		s_rm_text[i] = graphics->renderTextFNT(rm_text[i], f_main_menu, &c_mm_green, &c_mm_greenh, '_');
		graphics->apply_surface(rm_text_pos[i].x, rm_text_pos[i].y, s_rm_text[i], background);
	}
	
	// Text blitted to the arms
	SDL_Surface *s_text;
	s_text = graphics->renderTextFNT(rm_text[3], f_main_menu, &c_mm_green, &c_mm_greenh, '_');
	graphics->apply_surface(rm_text_pos[3].x, rm_text_pos[3].y, s_text, s_arm_l, NULL, H_CENTER);
	SDL_FreeSurface(s_text);
	s_text = graphics->renderTextFNT(rm_text[4], f_main_menu, &c_mm_green, &c_mm_greenh, '_');
	graphics->apply_surface(rm_text_pos[4].x, rm_text_pos[4].y, s_text, s_arm_r, NULL, H_CENTER);
	SDL_FreeSurface(s_text);
	s_text = graphics->renderTextFNT(rm_text[5], f_main_menu, &c_mm_green, &c_mm_greenh, '_');
	graphics->apply_surface(rm_text_pos[5].x, rm_text_pos[5].y, s_text, s_arm_r, NULL, H_CENTER);
	SDL_FreeSurface(s_text);
	
	RaceSelection option = R_TERRAN;
	RaceSelection R_last;
	
	SDL_Surface *selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
	
	Mix_Chunk *s_moo = NULL;
	
	playSound(s_swishin);
	graphics->refresh_whole_screen(false);
	
	float increment = 4;
	bool exit = false;
	bool selected = false;
	
	while(!exit) {
		
		control->updateState();
		
		if (control->isPressed(KEY_OK)) {
			
			switch(option) {
				case R_ZERG:
				case R_TERRAN:
				case R_PROTOSS:
					selected = true;
					playSound(s_mousedown2);
					playSound(s_swishout);
					break;
				case R_MOO:
					Mix_FreeChunk(s_moo);
					switch(rand()%3) {
						case 2: s_moo = loadSound(S_MOO3); break;
						case 1: s_moo = loadSound(S_MOO2); break;
						default: s_moo = loadSound(S_MOO); break;
					}
					playSound(s_moo);
					break;
				case R_NOTHING:
					break;
				case R_CANCEL:
					selected = true;
					option = R_EXIT;
					playSound(s_swishout);
					break;
					
				default: break;
			}
			
		} else if (control->isPressed(KEY_BACK)) {
			
			selected = true;
			option = R_EXIT;
			playSound(s_swishout);
			
		} else if (control->isPressed(KEY_RIGHT)) {
			
			switch (option) {
				case R_ZERG:
				case R_TERRAN:
				case R_PROTOSS:
					playSound(s_mouseover);
					
					option++;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case R_MOO:
				case R_NOTHING:
					playSound(s_mouseover);
					
					option = R_CANCEL;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
					
				default: break;
			}
			
		} else if (control->isPressed(KEY_LEFT)) {
			
			switch (option) {
				case R_ZERG:
				case R_TERRAN:
				case R_PROTOSS:
					playSound(s_mouseover);
					
					option--;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case R_CANCEL:
					playSound(s_mouseover);
					
					option = R_NOTHING;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
					
				default: break;
			}
			
		} else if (control->isPressed(KEY_UP)) {
			
			switch (option) {
				case R_CANCEL:
					playSound(s_mouseover);
					
					option = R_PROTOSS;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					
					break;
				case R_MOO:
					playSound(s_mouseover);
					
					option = R_last;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					
					break;
				case R_NOTHING:
					playSound(s_mouseover);
					
					option = R_MOO;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					
					break;
					
				default: break;
			}
			
			
		} else if (control->isPressed(KEY_DOWN)) {
			
			switch (option) {
				case R_ZERG:
				case R_TERRAN:
					playSound(s_mouseover);
					
					R_last = option;
					option = R_MOO;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					
					break;
				case R_PROTOSS:
					playSound(s_mouseover);
					
					R_last = R_TERRAN;
					option = R_CANCEL;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					
					break;
				case R_MOO:
					playSound(s_mouseover);
					
					option = R_NOTHING;
					
					SDL_FreeSurface(selected_option);
					selected_option = graphics->renderTextFNT(rm_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					
					break;
					
				default: break;
			}
			
		}
		
		if (option != R_ZERG)
			races[R_ZERG]->setAlpha(ALPHA_FLAGS, subAlpha(races[R_ZERG]->getAlpha()) );
		if (option != R_TERRAN)
			races[R_TERRAN]->setAlpha(ALPHA_FLAGS, subAlpha(races[R_TERRAN]->getAlpha()) );
		if (option != R_PROTOSS)
			races[R_PROTOSS]->setAlpha(ALPHA_FLAGS, subAlpha(races[R_PROTOSS]->getAlpha()) );
		
		if ((option == R_ZERG) || (option == R_TERRAN) || (option == R_PROTOSS))
			races[option]->setAlpha(ALPHA_FLAGS, addAlpha(races[option]->getAlpha()) );
		
		
		/* Redrawing */
		
		graphics->apply_surface(0, 0, background, screen);
		
		
		// Race over animations
		switch (option) {
			case R_ZERG:
				races[3]->drawFrame(5, 41, ANI_STATIC);
				break;
				
			case R_TERRAN:
				races[4]->drawFrame(162, 4, ANI_STATIC);
				break;
				
			case R_PROTOSS:
				races[5]->drawFrame(391, 7, ANI_STATIC);
				break;
				
			default: break;
		}
		
		
		//Disks
		races[6]->drawFrame(33, 278, ANI_STATIC | ANI_NO_ANIMATE);		// Zerg
		races[6]->drawFrame(240, 192, ANI_STATIC | ANI_NO_ANIMATE);		// Terran
		races[6]->drawFrame(462, 277, ANI_STATIC);						// Protoss
		
		// Race animations
		races[0]->drawFrame(16, 98, ANI_STATIC);						// Zerg
		races[1]->drawFrame(162, 34, ANI_STATIC);						// Terran
		races[2]->drawFrame(449, 109, ANI_STATIC);						// Protoss
		
		// Arms
		int old_xl = r_arm_l.x;
		int old_xr = r_arm_r.x;
		//   In
		if (!selected) {
			if (r_arm_l.x < 0) {
				increment *= 1.1;
				if ((r_arm_l.x + s_arm_l->w/increment) >= 0) {
					r_arm_l.x = 0;
					r_arm_r.x = SCREEN_WIDTH - s_arm_r->w;
				} else {
					r_arm_l.x += (int) (s_arm_l->w/increment);
					r_arm_r.x -= (int) (s_arm_r->w/increment);
				}
				
			}
			
		//   Out
		} else {
			if ((r_arm_l.x + s_arm_l->w) > 0) {
				increment /= 1.1;
				if ((r_arm_l.x - s_arm_l->w/increment) <= -s_arm_l->w) {
					r_arm_l.x = -s_arm_l->w;
					r_arm_r.x = SCREEN_WIDTH;
				} else {
					r_arm_l.x -= (int) (s_arm_l->w/increment);
					r_arm_r.x += (int) (s_arm_r->w/increment);
				}
			}
				
			if (fade_black->format->alpha != 255) {
				SDL_SetAlpha(fade_black, SDL_SRCALPHA, fade_black->format->alpha+85);
				graphics->refresh_whole_screen(false);
				
			} else
				exit = true;
			
		}
		graphics->add_rect(graphics->apply_surface(r_arm_l.x, r_arm_l.y, s_arm_l, screen, NULL, H_ANIM, old_xl, 0));
		graphics->add_rect(graphics->apply_surface(r_arm_r.x, r_arm_r.y, s_arm_r, screen, NULL, H_ANIM, old_xr, 0));
		
		
		// Text
		for (int i=0; i<3; i++)
			graphics->apply_surface(rm_text_pos[i].x, rm_text_pos[i].y, s_rm_text[i], screen);
		// Selected option
		if (option < R_MOO)
			graphics->add_rect(graphics->apply_surface(rm_text_pos[option].x, rm_text_pos[option].y, selected_option, screen));
		else if (option == R_MOO)
			graphics->add_rect(graphics->apply_surface(rm_text_pos[option].x, r_arm_l.y + rm_text_pos[option].y,
													   selected_option, screen, NULL, H_CENTER));
		else
			graphics->add_rect(graphics->apply_surface(r_arm_r.x + rm_text_pos[option].x, r_arm_r.y + rm_text_pos[option].y,
													   selected_option, screen, NULL, H_CENTER));
		
		graphics->apply_surface(0, 0, fade_black, screen);
		
		framerate->tic();
		graphics->refresh_screen();
		framerate->delay();
		
	}
	
	
	SDL_FreeSurface(background);
	SDL_FreeSurface(s_arm_l);
	SDL_FreeSurface(s_arm_r);
	SDL_FreeSurface(selected_option);
	Mix_FreeChunk(s_moo);
	
	return option;
	
}


void credits(/*We just have a*/ void /*better system.*/) {
	
//	Now this is a story, all about how my life got flipped, turned upside-down, And I'd like
//	to take a minute, So just sit right there, I'll tell you how I became the prince of a town
//	called Bel-Air. In West Philadelphia, born and raised, On the playground is where I spent
//	most of my days Chillin out, maxin', relaxin' all cool, And all shootin some b-ball outside
//	of the school When a couple of guys, who were up to no good, Started makin' trouble in my
//	neighborhood, I got in one little fight and my mom got scared She said 'You're movin with your
//	auntie and uncle in Bel-Air!' I whistled for a cab and when it came near The license plate
//	said 'Fresh' And it had dice in the mirror If anything I could say that this cab was rare But I
//	thought'Nah, forget it - Yo, home to Bel-Air!' I pulled up to the house about seven or eight, I
//	yelled to the cabbie 'Yo home, smell ya later!' Looked at my kingdom, I was finally there To
//	settle my throne as the prince of Bel-Air!
	
	Mix_FadeOutMusic(500);
	Mix_Music *m_credits;
	m_credits = loadMusic(M_CREDITS);
	playMusic(m_credits);
	SDL_Surface *background, *text;
	
	goto a90; /* WIN */
	
	a00:
			background = graphics->load_image(P_BG_CRED_10, NULL);
			graphics->apply_surface(0, 0, background, screen);
			text = graphics->renderTextFNT("No pixels were harmed during the making of this game...", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(SCREEN_WIDTH/2, 174, text, screen, NULL, H_CENTER);
			SDL_FreeSurface(text);
			graphics->refresh_whole_screen();
			a03:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto a05;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a03;
	a05:
			text = graphics->renderTextFNT("...to the best of our knownledge...", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(SCREEN_WIDTH/2, 315, text, background, NULL, H_CENTER);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a07:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto aFTW;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a07;
	a10:
			background = graphics->load_image(P_BG_CRED_09, NULL);
			text = graphics->renderTextFNT("Public Relations", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(576, 134, text, background, NULL, H_RIGHT);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("xchat-2", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(576, 178, text, background, NULL, H_RIGHT);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a15:
							control->updateState();
					if (control->isPressed(KEY_OK)) goto a00;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a15;
	a20:
			background = graphics->load_image(P_BG_CRED_08, NULL);
			text = graphics->renderTextFNT("Technical Support", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 134, text, background);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("Red Bull", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 178, text, background);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a25:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto a10;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a25;
	a30:
			background = graphics->load_image(P_BG_CRED_07, NULL);
			text = graphics->renderTextFNT("Director of Operations", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(576, 222, text, background, NULL, H_RIGHT);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("Raziel", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(576, 266, text, background, NULL, H_RIGHT);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a35:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto a20;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a35;
	a40:
			background = graphics->load_image(P_BG_CRED_06, NULL);
			text = graphics->renderTextFNT("Strike Team", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 134, text, background);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("Raziel", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 178, text, background);
			graphics->apply_surface(60, 200, text, background);
			graphics->apply_surface(60, 222, text, background);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("...", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 244, text, background);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a45:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto a30;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a45;
	a50:
			background = graphics->load_image(P_BG_CRED_05, NULL);
			text = graphics->renderTextFNT("Audio Producers", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(576, 222, text, background, NULL, H_RIGHT);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("Raziel", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(576, 266, text, background, NULL, H_RIGHT);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a55:
					control->updateState(); // OMFG POSTING IN STICKY
					if (control->isPressed(KEY_OK)) goto a40;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a55;
	a60:
			background = graphics->load_image(P_BG_CRED_04, NULL);
			text = graphics->renderTextFNT("Director of Blizzard Film", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 222, text, background);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("Grinman (whom lieks to grin)", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 266, text, background);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a65:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto a50;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a65;
	a70:
			background = graphics->load_image(P_BG_CRED_03, NULL);
			text = graphics->renderTextFNT("Art Directors", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(576, 211, text, background, NULL, H_RIGHT);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("Raziel", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(576, 255, text, background, NULL, H_RIGHT);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a75:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto a60;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a75;
	a80:
			background = graphics->load_image(P_BG_CRED_02, NULL);
			text = graphics->renderTextFNT("Lead Programmer", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 222, text, background);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("Raziel", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(60, 266, text, background);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a85:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto a70;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a85;
	a90:
			background = graphics->load_image(P_BG_CRED_01, NULL);
			text = graphics->renderTextFNT("Game Design", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(SCREEN_WIDTH/2, 222, text, background, NULL, H_CENTER);
			SDL_FreeSurface(text);
			text = graphics->renderTextFNT("Raziel", f_main_menu, &c_mm_greenh);
			graphics->apply_surface(SCREEN_WIDTH/2, 266, text, background, NULL, H_CENTER);
			SDL_FreeSurface(text);
			graphics->apply_surface(0, 0, background, screen);
			SDL_FreeSurface(background);
			graphics->refresh_whole_screen();
			a95:
					control->updateState();
					if (control->isPressed(KEY_OK)) goto a80;
					if (control->isPressed(KEY_BACK)) goto aFTW;
					SDL_Delay(1);
					goto a95;
	aFTW:
			SDL_FillRect(screen, NULL, 0);
			graphics->refresh_whole_screen();
			Mix_FadeOutMusic(500);
			Mix_FreeMusic(m_credits);
	
//	WRYYYYYYYYYYYyyyyyyYYYYYYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYYYyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYyyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYyyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYyyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYYyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYYYYyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYYYYYyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYYYyyyyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYY
//	YYYYYYYYYYyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYyyyyYYYY
//	YYYYYYYYYYyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYyyyyyyyyyyyyyYY
//	YYYYYYYYYyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyYY
//	YYYYYYYYYyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyYYYYY
//	YYYYYYYYYyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYYY
//	YYyyYYYYYYYyyyyyyyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYYYYY
//	YyyyyyyyyyYYYyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYYYYY
//	yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYYYY
//	YYyyyyyyyyyyyyyyyyyyyyyYyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYY
//	YYYYYYYYyyyyyyyyyyyyyyYYyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYY
//	YYYYYYYYYYYYYyyyyyyyYYYYyyyyyyyyyyyyyyyyyyyyyyyYYYYYYY
//	YYYYYYYYYYYYYYYYYYYYYYYyyyyyyyyYYYYyyyyyyyyyyyYYYYY
//	YYYYYYYYYYYYYYYYYYYYYYYyyyyyyyyYYYYYYyyyyyyyyyyyYYY
//	YYYYYYYYYYYYYYYYYYYYYYYyyyyyyyyYYYYYYYYyyyyyyyyyyYY
//	YYYYYYYYYYYYYYYYYYYYYYYyyyyyyyyYYYYYYYYYyyyyyyyyyyY
//	YYYYYYYYYYYYYYYYYYYYYYYyyyyyyyyYYYYYYYYYYYyyyyyyyyy
//	YYYYYYYYYYYYYYYYYYYYYYYyyyyyyyyYYYYYYYYYYYyyyyyyyyy
//	YYYYYYYYYYYYYYYYYYYYYYYyyyyyyyYYYYYYYYYYYYyyyyyyyy
//	YYYYYYYYYYYYYYYYYYYYYYyyyyyyyyyYYYYYYYYYYYyyyyyyyYY
//	YYYYYYYYYYYYYYYYYYYYyyyyyyyyyyyyYYYYYYYYYYyyyyyyyyYY
//	YYYYYYYYYYYYYYYYYYyyyyyyyyyyyyyYYYYYYYYYYYyyyyyyyyYY
//	YYYYYYYYYYYYYYYYyyyyyyyyyyyyYYYYYYYYYYYYYyyyyyyyyYYY
//	YYYYYYYYYYYYYYyyyyyyyyyyyyYYYYYYYYYYYYYYYyyyyyyyyYYY
//	YYYYYYYYYYYYyyyyyyyyyyyyYYYYYYYYYYYYYYYYYyyyyyyyyYYY
//	YYYYYYYYYYYYyyyyyyyyyyYYYYYYYYYYYYYYYYYYyyyyyyyyYYY
//	YYYYYYYYYYYYyyyyyyyYYYYYYYYYYYYYYYYYYYYYyyyyyyYYYY
	
}


int menu_main(Animation **animations) {
	
	SDL_Surface *background = graphics->load_image(P_BG_MAIN_MENU, NULL);
	
	SDL_Surface *fade_black = graphics->copySurface(screen, false);
	SDL_FillRect(fade_black, 0, 0x00000000);
	SDL_SetAlpha(fade_black, SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT);
	
	SDL_Surface *mptail = graphics->load_image(P_ANI_MAIN_MPTAIL);
	graphics->apply_surface(224, 346, mptail, background);
	SDL_FreeSurface(mptail);
	
	SDL_Surface *version = graphics->renderTextFNT(VERSION, &font10x13, &c_mm_blue);
	graphics->apply_surface(SCREEN_WIDTH-6, SCREEN_HEIGHT-1, version, background, NULL, (H_RIGHT | V_DOWN));
	SDL_FreeSurface(version);
	
	
	const char *mm_label_text[] = {
		"_S_ingle Player",		// (150, 111)
		"_M_ultiplayer",		// (78, 190)
		"_O_ptions",			// (375, 104)
		"E_x_it",				// (490, 320)
		"Show C_r_edits"		// (40, 430)
	};
	
	SDL_Surface *labels[5];
	
	for (int i=0; i<5; i++)
		labels[i] = graphics->renderTextFNT(mm_label_text[i], f_main_menu, &c_mm_green, &c_mm_greenh, '_');
	
	// Blitting all the labels to background, to prevent update rect overhead
	for (int i=0; i<5; i++) {
		graphics->apply_surface(pos[i].x, pos[i].y, labels[i], background);
	}
	
	
	graphics->refresh_whole_screen(false);
	
	bool exit = false;
	bool fade = false;
	
	MainMenuOpt option = MAIN_SP;
	SDL_Surface *s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
	
	while(!exit) {
		
		// Iniput handling
		
		control->updateState();
		
		
		if (control->isPressed(KEY_OK)) {
			
			playSound(s_mousedown2);
			fade = true;
			
		} else if (control->isPressed(KEY_BACK)) {
			
			playSound(s_mousedown2);
			option = MAIN_EXIT;
			exit = true;
			
		} else if (control->isPressed(KEY_UP)) {
			
			switch (option) {
				case MAIN_MP:
					playSound(s_mouseover);
					option = MAIN_SP;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case MAIN_EXIT:
					playSound(s_mouseover);
					option = MAIN_OPT;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case MAIN_CRED:
					playSound(s_mouseover);
					option = MAIN_MP;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				default: break;
			}
			
		} else if (control->isPressed(KEY_DOWN)) {
			
			switch (option) {
				case MAIN_SP:
					playSound(s_mouseover);
					option = MAIN_MP;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case MAIN_OPT:
					playSound(s_mouseover);
					option = MAIN_EXIT;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case MAIN_MP:
					playSound(s_mouseover);
					option = MAIN_CRED;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				default: break;
			}
			
		} else if (control->isPressed(KEY_RIGHT)) {
			
			switch (option) {
				case MAIN_SP:
					playSound(s_mouseover);
					option = MAIN_OPT;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case MAIN_MP:
					playSound(s_mouseover);
					option = MAIN_EXIT;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case MAIN_CRED:
					playSound(s_mouseover);
					option = MAIN_MP;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				default: break;
			}
			
		} else if (control->isPressed(KEY_LEFT)) {
			
			switch (option) {
				case MAIN_OPT:
					playSound(s_mouseover);
					option = MAIN_SP;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case MAIN_EXIT:
					playSound(s_mouseover);
					option = MAIN_MP;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				case MAIN_MP:
					playSound(s_mouseover);
					option = MAIN_CRED;
					SDL_FreeSurface(s_selected);
					s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
					graphics->refresh_whole_screen(false);
					break;
				default: break;
			}
			
		} else if (control->isPressed(SDLK_s)) {
			option = MAIN_SP;
			SDL_FreeSurface(s_selected);
			s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			playSound(s_mousedown2);
			fade = true;
		} else if (control->isPressed(SDLK_m)) {
			option = MAIN_MP;
			SDL_FreeSurface(s_selected);
			s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			playSound(s_mousedown2);
			fade = true;
		} else if (control->isPressed(SDLK_o)) {
			option = MAIN_OPT;
			SDL_FreeSurface(s_selected);
			s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			playSound(s_mousedown2);
			fade = true;
		} else if (control->isPressed(SDLK_x)) {
			option = MAIN_EXIT;
			SDL_FreeSurface(s_selected);
			s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			playSound(s_mousedown2);
			fade = true;
		} else if (control->isPressed(SDLK_r)) {
			option = MAIN_CRED;
			SDL_FreeSurface(s_selected);
			s_selected = graphics->renderTextFNT(mm_label_text[option], f_main_menu, &c_mm_greenh, NULL, '_');
			graphics->refresh_whole_screen(false);
			playSound(s_mousedown2);
			fade = true;
		}
		
		
		/* Redrawing */
		
		
		graphics->apply_surface(0, 0, background, screen);
		
		
		// Over Animations (1)
		if (option == MAIN_SP)
			animations[4]->drawFrame(pos_ani_on[0].x, pos_ani_on[0].y, ANI_STATIC);				// SP
		
		// Animations
		for (int i=0; i<4; i++) {
			animations[i]->drawFrame(pos_ani[i].x, pos_ani[i].y, ANI_STATIC);
		}
		
		// Over Animations (2)
		switch (option) {
			
			case MAIN_MP:
				animations[5]->drawFrame(pos_ani_on[1].x, pos_ani_on[1].y, ANI_STATIC); break;	// MP
				
			case MAIN_OPT:
				animations[6]->drawFrame(pos_ani_on[2].x, pos_ani_on[2].y, ANI_STATIC); break;	// OP
				
			case MAIN_EXIT:
				animations[7]->drawFrame(pos_ani_on[3].x, pos_ani_on[3].y, ANI_STATIC); break;	// EX
				
			default: break;
			
		}
		
		
		for (int i=0; i<4; i++) {
			graphics->apply_surface(pos[i].x, pos[i].y, labels[i], screen);
		}
		
		graphics->apply_surface(pos[option].x, pos[option].y, s_selected, screen);
		
		
		if (fade) {
			SDL_SetAlpha(fade_black, SDL_SRCALPHA, fade_black->format->alpha+85);
			exit = (fade_black->format->alpha == 255);
			
			graphics->apply_surface(0, 0, fade_black, screen);
			graphics->refresh_whole_screen(false);
		}
		
		framerate->tic();
		graphics->refresh_screen();
		framerate->delay();
		
	}
	
	
	// Cleanup
	SDL_FreeSurface(background);
	SDL_FreeSurface(s_selected);
	for (int i=0; i<5; i++) {
		SDL_FreeSurface(labels[i]);
	}
	
	return option;
	
}


Animation **preload_races() {
	
	Animation **animations = new Animation* [7];
	
	
	animations[0] = new Animation(P_ANI_ZERG, 70, 9, 1, &defaultColorkey);
	animations[1] = new Animation(P_ANI_TERRAN, 78, 10, 1, &defaultColorkey);
	animations[2] = new Animation(P_ANI_PROTOSS, 70, 9, 1, &defaultColorkey);
	
	animations[3] = new Animation(P_ANI_ZERGON, 20, 5, 1, &defaultColorkey);
	animations[4] = new Animation(P_ANI_TERRANON, 20, 5, 1, &defaultColorkey);
	animations[5] = new Animation(P_ANI_PROTOSSON, 31, 7, 1, &defaultColorkey);
	
	animations[6] = new Animation(P_ANI_DISK, 36, 6, 1, &defaultColorkey);
	
	
	animations[ZERG]->setAlpha(ALPHA_FLAGS, UNSELECTED_ALPHA);
	animations[TERRAN]->setAlpha(ALPHA_FLAGS, UNSELECTED_ALPHA);
	animations[PROTOSS]->setAlpha(ALPHA_FLAGS, UNSELECTED_ALPHA);
	
	
	return animations;
	
}


Animation **preload_menu_main() {
	
	Animation **animations = new Animation* [8];
	
	animations[0] = new Animation(P_ANI_MAIN_SP, 35, 7, 1, &defaultColorkey);
	animations[1] = new Animation(P_ANI_MAIN_MP, 50, 8, 1, &defaultColorkey);
	animations[2] = new Animation(P_ANI_MAIN_OPT, 85, 10, 1, &defaultColorkey);
	animations[3] = new Animation(P_ANI_MAIN_EXIT, 50, 8, 1, &defaultColorkey);
	
	animations[4] = new Animation(P_ANI_MAIN_SPON, 60, 9, 1, &defaultColorkey);
	animations[5] = new Animation(P_ANI_MAIN_MPON, 20, 5, 1, &defaultColorkey);
	animations[6] = new Animation(P_ANI_MAIN_OPTON, 20, 5, 1, &defaultColorkey);
	animations[7] = new Animation(P_ANI_MAIN_EXITON, 30, 6, 1, &defaultColorkey);
	
	return animations;
	
}


void menu() {
	
	
	// Audio
	
	Mix_Music *m_main_menu;
	m_main_menu = loadMusic(M_MAIN_MENU);
	playMusic(m_main_menu);
	
	s_swishin = loadSound(S_SWISHIN);
	s_swishout = loadSound(S_SWISHOUT);
	s_swishlock = loadSound(S_SWISHLOCK);
	s_mouseover = loadSound(S_MOUSEOVER);
	s_mousedown2 = loadSound(S_MOUSEDOWN2);
	
	
	// Loading screen
	
	SDL_Surface *background = graphics->load_image(P_BG_LOADING, NULL);
	
	SDL_Surface *s_text;
	s_text = graphics->renderTextFNT("Loading", &font17x19, &c_tit_blue);
	graphics->apply_surface(SCREEN_WIDTH/2, SCREEN_HEIGHT*5/6, s_text, background, NULL, H_CENTER);
	SDL_FreeSurface(s_text);
	
	s_text = graphics->renderTextFNT("Copyright  1998 Blizzard Entertainment. All rights reserved.",
			&font10x13, &c_tit_blue);
	graphics->apply_surface(SCREEN_WIDTH/2, SCREEN_HEIGHT*5/6+31, s_text, background, NULL, H_CENTER);
	SDL_FreeSurface(s_text);
	
	s_text = graphics->renderTextFNT("StarCraft and Blizzard Entertainment are trademarks or registered",
			&font10x13, &c_tit_blue);
	graphics->apply_surface(SCREEN_WIDTH/2, SCREEN_HEIGHT*5/6+31+16, s_text, background, NULL, H_CENTER);
	SDL_FreeSurface(s_text);
	
	s_text = graphics->renderTextFNT("trademarks of Blizzard Entertainment, Inc. in the U.S. and/or other countries.",
			 &font10x13, &c_tit_blue);
	graphics->apply_surface(SCREEN_WIDTH/2, SCREEN_HEIGHT*5/6+31+16+16, s_text, background, NULL, H_CENTER);
	SDL_FreeSurface(s_text);
	
	graphics->apply_surface(0, 0, background, screen);
	graphics->refresh_whole_screen();
	
	// Preload
	
	Animation **m_ani = preload_menu_main();
	Animation **r_ani = preload_races();
	
	
	// Main menu
	
	bool exit = false;
	
	while (!exit) {
		
		framerate->setFPS(MENU_FPS);
		
		if (!Mix_PlayingMusic())
			playMusic(m_main_menu);
		
		switch(menu_main(m_ani)) {
			
			case MAIN_SP: {
				
				int player_race = menu_race(r_ani);
				
				if (player_race != R_EXIT) {
					
					Player *player0; // Right player (default for human)
					Player *player1; // Left player (default for computer)
					
					switch (player_race){
						case ZERG:
							player0 = new PlayerZerg(RIGHT, LEFT, HUMAN, "Zarovich", 0);
							break;
						case TERRAN:
							player0 = new PlayerTerran(RIGHT, LEFT, HUMAN, "Zarovich", 0);
							break;
						case PROTOSS:
							player0 = new PlayerProtoss(RIGHT, LEFT, HUMAN, "Zarovich", 0);
							break;
							
						default: break;
					}
					
					player1 = new PlayerProtoss(LEFT, RIGHT, COMPUTER, "Thorn", 1);
					
					Game game = Game(player0, player1, 2);
					game.start();
					
				}
				
				break;
				
			}
			
			case MAIN_MP:
				
				playSound(s_swishlock);
				menu_multiplayer();
				break;
				
			case MAIN_OPT:
				
				playSound(s_swishlock);
				menu_options();
				break;
				
			case MAIN_EXIT:
				
				exit = true;
				break;
				
			case MAIN_CRED:
				
				credits();
				break;
				
			default:
				
				die(ERR_CUSTOM, "menu(): Invalid option");
				
		}
		
	}
	
	
	SDL_FreeSurface(screen);
	SDL_FreeSurface(background);
	Mix_FreeChunk(s_swishin);
	Mix_FreeChunk(s_swishout);
	Mix_FreeChunk(s_mouseover);
	Mix_FreeChunk(s_swishlock);
	Mix_FreeChunk(s_mousedown2);
	Mix_FreeMusic(m_main_menu);
	
}


bool game_menu(Game *game) {
	
	SDL_Surface *background = graphics->copySurface(screen);
	
	const char *options[] = { "Return to Game (_Esc_)",
				  "_O_ptions",
				  "_H_elp",
				  "_E_nd Mission",
				  NULL };
	GameMenuWindow menu = GameMenuWindow(SCREEN_WIDTH/2, SCREEN_HEIGHT/15, 33, 36, "Game Menu", options);
	
	const char *lines[] = { "When your life reaches 0, you die",
				"...",
				"lol.",
				"",
				"_1_ - Weak Attack",
				"_2_ - Hard Attack",
				"",
				"_+_ - Increase Game Speed",
				"_-_ - Decrease Game Speed",
				"",
				"#Multiplayer Features:",
				"_Enter_ - Messaging Prompt",
				NULL };
	HelpWindow help = HelpWindow(SCREEN_WIDTH/2, SCREEN_HEIGHT/15, 39, 39, "Pongcraft Help", lines);
	
	Window *actual_window = &menu;
	
	graphics->apply_surface(0, 0, background, screen);
	
	bool exit = false; // exit from menu
	bool gtfo = false; // exit from game
	
	while (!exit) {
		
		if (actual_window == &menu) {
			
			switch (menu.handleInput()) {
				case GOPT_OPT:
					menu_options();
					graphics->refresh_whole_screen(false);
					break;
				case GOPT_HELP:
					menu.hide();
					actual_window = &help;
					break;
					
				case GOPT_EXIT:		gtfo = true;
				case GOPT_RETURN:	exit = true;
					break;
					
				case GOPT_NONE:
					break;
				
				default: break;
			}
			
		} else if (actual_window == &help) {
			
			switch (help.handleInput()) {
				case GOPT_RETURN:
					help.hide();
					actual_window = &menu;
					break;
					
				case GOPT_NONE:
					break;
				
				default: break;
			}
			
		}
		
		graphics->apply_surface(0, 0, background, screen);
		
		for (Uint8 i=0; i<game->balls.size(); i++) {
			game->balls[i]->show((int)&menu);
		}
		
		actual_window->draw();
		
		framerate->tic();
		graphics->refresh_whole_screen();
		framerate->delay();
		
	}
	
	menu.hide();
	
	//delete &help;//FIXME
	//delete &menu;//FIXME
	return gtfo;
	
}
