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



#ifndef __CCONTROLLER_H__
#define __CCONTROLLER_H__


#include <vector>


#include "SDL/SDL.h"


typedef enum {
	KEY_NULL,
	
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	
	KEY_B1,
	KEY_B2,
	KEY_B3,
	KEY_B4,
	
	KEY_OK,
	KEY_BACK,
	KEY_START
} CtrlKey;

class Controller {
	
	protected:
		Uint16 keys[16];
		
	public:
		Uint32 avail_ticks;
		
		virtual ~Controller() {};
		virtual void updateState(SDL_Event *event)=0;
		virtual bool isPressed(CtrlKey key, Uint32 delay)=0;
		virtual bool isPressed(Uint16 key, Uint32 delay)=0;
		
};


#endif
