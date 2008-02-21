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
#include "SDL/SDL.h"
#include "SDL/SDL_framerate.h"

#include "main.h"
#include "error.h"
#include "CGraphics.h"
#include "CFramerate.h"


extern Graphics *graphics;

Framerate::Framerate() {

	fps_manager = (FPSmanager*) malloc(sizeof(FPSmanager));
	SDL_initFramerate(fps_manager);
	init_tick = SDL_GetTicks();
	
	frames = 0;
	fps = 0;
	show_fps = false;
	refresh_bg = false;
	
	x = (SCREEN_WIDTH - 86);
	y = 40;
	
	lastRect = rect_zero;
	lastRect.y = y;
	
	background = graphics->load_image(P_FPS_BG, &color_magenta);
	
	
}

Framerate::~Framerate() {
	free(fps_manager);
}


void Framerate::tic() {
	
	frames++;
	
	// Each second we refresh the fps value
	if ((SDL_GetTicks() - init_tick) >= 1000) {
		
		fps = frames;
		init_tick = SDL_GetTicks();
		frames = 0;
		
	}
	
	
	SDL_Rect *rect = (SDL_Rect*) malloc(sizeof(SDL_Rect));
	*rect = rect_zero;
	
	
	if (show_fps) {
		
		if (fps > 999) die(ERR_CUSTOM, "Invalid FPS");
		
		
		char txt[9];
		sprintf(txt, "FPS: %d", fps);
		
		graphics->apply_surface(SCREEN_WIDTH, 0, background, screen, NULL, H_RIGHT);
		
		SDL_Surface *s_txt = graphics->renderTextFNT(txt, &font15x16, &c_mm_white);
		rect = graphics->apply_surface(x, y, s_txt, screen, NULL, (H_CENTER | V_CENTER));
		SDL_FreeSurface(s_txt);
		
		
		// When fps_show is turned on, we must refresh the whole fps box
		if (refresh_bg) {
		
			refresh_bg = false;
			
			rect->x = SCREEN_WIDTH - background->w;
			rect->y = 0;
			rect->w = background->w;
			rect->h = background->h;
		
		} else {
		
			// Checking if the text in the last iterraction was bigger than.
			// the last drawn text, in order to refresh screen properly
			
			if (rect->w < lastRect.w) {
				int tmp_x = rect->x; 
				int tmp_w = rect->w;
				
				rect->x = lastRect.x;
				rect->w = lastRect.w;
				
				lastRect.x = tmp_x;
				lastRect.w = tmp_w;
				
			} else {
				lastRect.x = rect->x;
				lastRect.w = rect->w;
				
			}
			
			if (rect->h < lastRect.h) {
				int tmp_h = rect->h;
				
				rect->h = lastRect.h;
				
				lastRect.h = tmp_h;
				
			} else {
				lastRect.h = rect->h;
				
			}
			
		}
		
		graphics->add_rect(rect);//FIXME esto pierde memoria
		
	} else if (refresh_bg) {
		
		// We have turned off fps drawing, but we must clear
		// the portion of screen where we drew the last iteraction
		
		refresh_bg = false;
		
		rect->x = SCREEN_WIDTH - background->w;
		rect->y = 0;
		rect->w = background->w;
		rect->h = background->h;
		
		graphics->add_rect(rect);
		
	}
	
}
