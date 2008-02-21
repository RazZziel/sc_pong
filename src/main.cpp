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
#include "SDL/SDL_mixer.h"

#ifdef __USE_OPENAL__
#include <AL/al.h>
#endif

#include "error.h"
#include "menus.h"
#include "main.h"
#include "CControl.h"
#include "CPhysics.h"
#include "CFramerate.h"
#include "CGame.h"
#include "CGraphics.h"


SDL_Surface *screen;
Control *control;

Graphics *graphics;
Framerate *framerate;
Physics *physics;


void clean_up() {
	
	delete control;
	delete framerate;
	delete graphics;
	
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

#ifdef __USE_OPENAL__
	alutExit();
#endif
	
}

void init(int argc, char *argv[]) {
	
	atexit(clean_up);
	
	// SDL
	Uint32 sdl_flags = 0;
	sdl_flags |= SDL_INIT_NOPARACHUTE;
	sdl_flags |= SDL_INIT_VIDEO;
	sdl_flags |= SDL_INIT_AUDIO;
	sdl_flags |= SDL_INIT_JOYSTICK;
	if (SDL_Init(sdl_flags) == -1) die(ERR_INIT, "SDL");
	
	// Video‘watexit’ 
	graphics = new Graphics();
	framerate = new Framerate();
	
	
	// Audio
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) die(ERR_INIT, "audio");
	Mix_VolumeMusic(20);
	Mix_Volume(-1, 20);
	
#ifdef __USE_OPENAL__
	// OpenAL
	alutInit(&argc, argv);
	alGetError();
	alDopplerFactor(1.0f);
	alDopplerVelocity(10.0f);
#endif
	
	// Control
	control = new Control();
	
	
	// Physics
	physics = new Physics();
	
}


int main(int argc, char *argv[]) {
	
	init(argc, argv);
	menu();
	return 0;
	
}
