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



#ifndef _CANIMATION__H__
#define _CANIMATION__H__

#include "CGraphics.h"


enum {
	ANI_STATIC	= 1,
	ANI_NO_ANIMATE	= 2,
	ANI_CENTERED	= 4
};


extern Graphics *graphics;

SDL_Rect *mapAnimation(SDL_Surface *source, int frames, int frames_h);


class AnimationData {
	public:
		SDL_Surface *sprites;
		SDL_Rect *frames;
		int n_frames;
		
		AnimationData(const char *filename, int n_frames, int frames_h, const SDL_Color *colorkey = NULL);
		~AnimationData();
};

class Animation {

	private:
		int frame;
		int old_x, old_y;
		int v;
		
	public:
		AnimationData *data;
		
		Animation(const char *filename, int n_frames, int frames_h, int v = 1, const SDL_Color *colorkey = NULL);
		Animation(AnimationData *data, int v = 1);
		~Animation() {};
		
		void drawFrame(int x, int y, Uint8 flags = 0);
		
		inline int getFrame()
			{ return frame/v; }
		
		inline bool animationCompleted()
			{ return (frame == 0); }
		
		inline void setAlpha(Uint32 flags, Uint8 alpha)
			{ SDL_SetAlpha(data->sprites, flags, alpha); }
		
		inline Uint8 getAlpha()
			{ return data->sprites->format->alpha; }
	
};


#endif
