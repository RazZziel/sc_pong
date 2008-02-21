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

#include "CAnimation.h"
#include "SDL/SDL.h"

#include "main.h"
#include "CGraphics.h"
#include "CAnimation.h"



extern SDL_Surface *screen;

/* AnimationData */

AnimationData::AnimationData(const char *filename, int n_frames, int frames_h, const SDL_Color *colorkey)
	: n_frames(n_frames) {
	
	sprites = graphics->load_image(filename);
	frames = mapAnimation(sprites, n_frames, frames_h);
	
	if (colorkey != NULL)
		graphics->setTransparent(sprites,  colorkey);
	
}

AnimationData::~AnimationData() {
	SDL_FreeSurface(sprites);
	free(frames);
}

/* Animation */

Animation::Animation(const char *filename, int n_frames, int frames_h, int v, const SDL_Color *colorkey)
	: frame(0), old_x(-1), old_y(-1), v(v) {
	this->data = new AnimationData(filename, n_frames, frames_h, colorkey);
	Animation(new AnimationData(filename, n_frames, frames_h, colorkey), v);
}

Animation::Animation(AnimationData *data, int v)
	: frame(0), old_x(-1), old_y(-1), v(v), data(data) {}


/*
 * Applies current frame and moves to next frame
 * Returned value is the rectangle to update
 */

void Animation::drawFrame(int x, int y, Uint8 flags) {
	
	int act_frame = frame/v;
	
	if (flags & ANI_CENTERED) {
		x -= data->frames[act_frame].w/2;
		y -= data->frames[act_frame].h/2;
	}
	
	if (flags & ANI_STATIC) {
		graphics->add_rect(graphics->apply_surface(x, y, data->sprites, screen, &data->frames[act_frame]));
		
	} else {
		
		if (((old_x + data->frames[act_frame].w) < x)
					|| ((x + data->frames[act_frame].w) < old_x)
					|| ((old_y + data->frames[act_frame].h) < y)
					|| ((y + data->frames[act_frame].h) < old_y)) {
			// Old position must be cleared
			graphics->add_rect(graphics->apply_surface(old_x, old_y, data->sprites, screen, &data->frames[act_frame], NO_BLIT));
			// Drawing sprite in new position
			graphics->add_rect(graphics->apply_surface(x, y, data->sprites, screen, &data->frames[act_frame]));
		} else
			graphics->add_rect(graphics->apply_surface(x, y, data->sprites, screen, &data->frames[act_frame], (H_ANIM|V_ANIM), old_x, old_y));
		
		old_x = x;
		old_y = y;
		
	}
	
	if (!(flags & ANI_NO_ANIMATE)) {
		if (++frame == data->n_frames*v) frame = 0;
	}
	//printf("%d %d %d %d\n", rect->x, rect->y, rect->w, rect->h);
	
}

SDL_Rect *mapAnimation(SDL_Surface *source, int frames, int frames_h) {

	/* Extracts the sprite map of an animation given in the following format:
	 *
	 *
	 *    /----fpl----/ 	<-- frames_h = 4
	 *     __ __ __ __
	 *    |1 |2 |3 |4 |
	 *    |__|__|__|__|
	 *    |5 |6 |7 |8 |
	 *    |__|__|__|__|
	 *    |9 |10|  |  | 	<- frames = 10
	 *    |__|__|__|__|
	 *
	 */
	
	SDL_Rect *map = (SDL_Rect*) malloc(frames*sizeof(SDL_Rect));
	
	int w = source->w/frames_h;
	int h = source->h/((int)ceil((float)frames/frames_h));
	
	int row = 0;
	int col = 0;
	
	//printf(">>>%d %d %d %d\n", source->w, source->h, w, h);
	
	for (int i=0; i<frames; i++) {
	
		if (col == frames_h) {
			row++;
			col = 0;
		}
		
		map[i].x = col*w;
		map[i].y = row*h;
		map[i].w = w;
		map[i].h = h;
		
		//printf("%d: x %d\t y %d\t w %d\t h %d\n", i, map[i].x, map[i].y, map[i].w, map[i].h);
		
		col++;
		
	}
	
	return map;
	
}
