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

#include "CFramerate.h"
#include "CGraphics.h"
#include "CController.h"
#include "CCtrlKeyboard.h"
#include "CCtrlJoystick.h"
#include "CCtrlNet.h"
#include "CControl.h"



extern Graphics *graphics;
extern Framerate *framerate;


Control::Control() {
	
	n_controllers = 1 + SDL_NumJoysticks();
	
	//printf("Number of controllers: %d\n", n_controllers);
	
	controllers = new Controller* [n_controllers];
	
	n_players = 2;
	if (n_controllers < 2)
		n_players = n_controllers;
	
	players = new Controller* [n_players];
	
	controllers[0] = new CtrlKeyboard();
	
	for (int i=1; i<n_players; i++)
		controllers[i] = new CtrlJoystick(i-1);
	
	//TODO poner bien donde cuadre
	assignController(0, 0);
	
	if (n_players > 1)
		assignController(1, 1);
	
}

Control::~Control() {
	
	delete [] controllers;
	
}

bool Control::isPressed(CtrlKey key, Sint8 player, Uint32 delay) {
	
	if (player == -1) {
		for (int i=0; i<n_players; i++) {
			if (players[i]->isPressed(key, delay))
				return true;
		}
		return false;
		
	} else
		return players[player]->isPressed(key, delay);
	
}

bool Control::isPressed(Uint16 key, Sint8 player, Uint32 delay) {
	
	return players[0]->isPressed(key, delay);
	
}

void Control::updateState() {
	
	SDL_Event event;
	
	while (SDL_PollEvent(&event)) {
		
		switch (event.type) {
			
			case SDL_KEYDOWN:
				
				switch (event.key.keysym.sym) {
					
					case SDLK_F9:
						framerate->toggle();
						break;
						
					case SDLK_RETURN:
						if (SDL_GetModState() & KMOD_ALT)
							graphics->toggle_fullscreen();
						break;
						
					default:
						break;
					
				}
				break;
				
				
			case SDL_QUIT:
				exit(0);
				
			default:
				break;
				
		}
		
		for (int i=0; i<n_players; i++)
			players[i]->updateState(&event);
		
	}
	
}
