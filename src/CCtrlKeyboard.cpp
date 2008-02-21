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



#include "SDL/SDL.h"

#include "CGraphics.h"
#include "CController.h"
#include "CCtrlKeyboard.h"



extern SDL_Surface *screen;


CtrlKeyboard::CtrlKeyboard() {
	
	//printf("Initializing keyboard\n");
	
	// TODO read from standard config file
	keys[KEY_UP]	= SDLK_UP;
	keys[KEY_DOWN]	= SDLK_DOWN;
	keys[KEY_LEFT]	= SDLK_LEFT;
	keys[KEY_RIGHT]	= SDLK_RIGHT;
	
	keys[KEY_OK]	= SDLK_RETURN;
	keys[KEY_BACK]	= SDLK_ESCAPE;
	
	keys[KEY_B1]	= SDLK_a;
	keys[KEY_B2]	= SDLK_s;
	keys[KEY_B3]	= SDLK_d;
	keys[KEY_B4]	= SDLK_w;
	
	keystate = SDL_GetKeyState(NULL);
	lastPressed = 0;
	avail_ticks = SDL_GetTicks();
	
}

bool CtrlKeyboard::isPressed(Uint16 key, Uint32 delay) {
	
	if ((key >= 'a') && (key <= 'z') && (keystate[key])
			&& (((key != lastPressed))
			|| ((key == lastPressed) && (SDL_GetTicks() >= avail_ticks)))) {
		lastPressed =  key;
		avail_ticks = SDL_GetTicks() + delay;
		return true;
	}
	
	return false;
	
}

bool CtrlKeyboard::isPressed(CtrlKey key, Uint32 delay) {
	
	if ((keystate[keys[key]]) // FIXME
			&& (((keys[key] != lastPressed))
			|| ((keys[key] == lastPressed) && (SDL_GetTicks() >= avail_ticks)))) {
		lastPressed = keys[key];
		avail_ticks = SDL_GetTicks() + delay;
		return true;
	}
	
	return false;
	
}

void CtrlKeyboard::updateState(SDL_Event *event) {
	
	switch (event->type) {
		case SDL_KEYUP:
			avail_ticks = SDL_GetTicks();
			break;
		
		default: break;
	}
	
	keystate = SDL_GetKeyState(NULL);
	
}
