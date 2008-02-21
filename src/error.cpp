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
#include <string>
#include "SDL/SDL.h"

#include "main.h"
#include "error.h"



void die(int type, const char *cause) {
	
	fprintf(stderr, "!!Error: ");
	
	switch(type) {
		case ERR_CUSTOM : fprintf(stderr, "%s", cause); break;
		case ERR_INIT	: fprintf(stderr, "Cannot init %s", cause); break;
		case ERR_LOAD	: fprintf(stderr, "Cannot load %s", cause); break;
		case ERR_PMUSIC	: fprintf(stderr, "Cannot play music"); break;
		case ERR_PCHUNK	: fprintf(stderr, "Cannot play sound"); break;
	}
	
	fprintf(stderr, "\n%s\n", SDL_GetError());
	
	exit(1);
	
}
