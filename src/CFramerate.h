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



#ifndef __CFRAMERATE_H__
#define __CFRAMERATE_H__



#include "SDL/SDL.h"
#include "SDL/SDL_framerate.h"


#define P_FPS_BG	"data/pixmap/misc/fps_bg.png"


class Framerate {
	
	private:
		FPSmanager *fps_manager;
		int init_tick;
		int frames, fps;
		bool show_fps, refresh_bg;
		SDL_Rect lastRect;
		SDL_Surface *background;
		int x, y;
		
	public:
		
		Framerate();
		~Framerate();
		
		void tic();
		void reset();
		
		inline void setFPS(int fps)	{ SDL_setFramerate(fps_manager, fps); }
		inline void delay()			{ SDL_framerateDelay(fps_manager); }
		inline void toggle()		{ show_fps = !show_fps; refresh_bg = true; }
		
};


#endif
