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



#ifndef __CCONTROL_H__
#define __CCONTROL_H__



#include "SDL/SDL.h"
#include "CController.h"
#include "CCtrlJoystick.h"



#define DELAY	300


class Control {
	
	private:
		Controller **controllers; // Number of controllers available on the machine
		Controller **players;     // Number of players playing on the same machine
		int n_controllers, n_players;
		
	public:
		Control();
		~Control();
		bool isPressed(CtrlKey key, Sint8 player = -1, Uint32 delay = DELAY);
		bool isPressed(Uint16 key, Sint8 player = -1, Uint32 delay = DELAY);
		void updateState();
		
		inline void assignController(int player, int control)
			{ players[player] = controllers[control]; }
		
		inline void flushEvents()
			{ SDL_Event event; while (SDL_PollEvent(&event)); }
		
};


#endif
