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



#ifndef __CCTRLJOYSTICK_H__
#define __CCTRLJOYSTICK_H__


#include "SDL/SDL.h"

#include "CController.h"


#define TOLERANCE	8000


class CtrlJoystick : public Controller {
	private:
		int index;
		SDL_Joystick *stick;
		int n_buttons;
		Uint8 lastPressed;
		
	public:
		CtrlJoystick(int _index);
		~CtrlJoystick();
		
		bool isPressed(CtrlKey key, Uint32 delay);
		bool isPressed(Uint16 key, Uint32 delay) { return false; }
		void updateState(SDL_Event *event);
		
};

#endif
