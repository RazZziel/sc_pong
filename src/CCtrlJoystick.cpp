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

#include "error.h"
#include "CController.h"
#include "CCtrlJoystick.h"


CtrlJoystick::CtrlJoystick(int _index) {
	
	index = _index;
	
	if ((stick = SDL_JoystickOpen(_index)) == NULL)
		die (ERR_INIT, "Joystick");
	
	n_buttons = SDL_JoystickNumButtons(stick);
	
	//printf("Joystick #%d fonud: %s\n", _index, SDL_JoystickName(_index));
	//printf("Number of Axes: %d\n", SDL_JoystickNumAxes(stick));
	//printf("Number of Buttons: %d\n", n_buttons);
	
	
	keys[KEY_UP]	= 1;
	keys[KEY_DOWN]	= 1;
	keys[KEY_LEFT]	= 0;
	keys[KEY_RIGHT]	= 0;
	
	keys[KEY_OK]	= 1;
	keys[KEY_BACK]	= 9;//TODO usar otro KEY_BACK para los menus
	
	keys[KEY_B1]	= 0;
	keys[KEY_B2]	= 1;
	keys[KEY_B3]	= 2;
	keys[KEY_B4]	= 3;
	
	avail_ticks = SDL_GetTicks();
	lastPressed = 0;
	
}

CtrlJoystick::~CtrlJoystick() {
	
	SDL_JoystickClose(stick);
	
}


bool CtrlJoystick::isPressed(CtrlKey key, Uint32 delay) {
	
	if (key < (CtrlKey)SDLK_a) {
		
		if ((lastPressed != keys[key])
				|| ((lastPressed == keys[key]) && (SDL_GetTicks() >= avail_ticks))) {
			
			
#if (__PC__)
		
			Sint16 axis_state = SDL_JoystickGetAxis(stick, keys[key]);
			
			if ((((key == KEY_UP) || (key == KEY_LEFT)) && (axis_state < -TOLERANCE))
						|| (((key == KEY_DOWN) || (key == KEY_RIGHT)) && (axis_state > TOLERANCE)) ) {
				lastPressed = keys[key];
				avail_ticks = SDL_GetTicks() + delay;
				return true;
			}
			
#endif
			
			if(SDL_JoystickGetButton(stick, keys[key])) {
				lastPressed = keys[key];
				avail_ticks = SDL_GetTicks() + delay;
				return true;
			}
			
			
		}
		
	}
	
	return false;
	
}

void CtrlJoystick::updateState(SDL_Event *event) {}
