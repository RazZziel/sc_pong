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



#ifndef __FONT_H__
#define __FONT_H__



#include <stdlib.h>

#include "SDL/SDL.h"


/* ScFont */

/* Thanks to Stormcoast Fortress for the fnt spec.       */
/* http://www.stormcoast-fortress.net/cntt/formats/font/ */

typedef struct {
	char	name[4];	//	Always is "FONT"
	Uint8	lowIndex;	//	Index of first letter in file
	Uint8	highIndex;	//	Index of the last letter in file
	Uint8	maxWidth;	//	Maximum width
	Uint8	maxHeight;	//	Maximum height
	Uint32	unk1;		//	Unknown
} ScFontHeader;

typedef struct {
	Uint8	w;			//	Width of the letter
	Uint8	h;			//	Height of the letter
	Uint8	x;			//	X Offset for the topleft corner of the letter.
	Uint8	y;			//	Y Offset for the topleft corner of the letter.
} ScLetterHeader;

typedef struct {
	ScLetterHeader header;
	int n_pixels;
	Uint8 *pixels;
} ScLetter;

typedef struct {
	ScFontHeader header;
	ScLetter *letters;
} ScFont;


/* ScColor */

enum {
	MAX_COLORS	= 6,
	MAX_GAMMA	= 8
};


typedef Uint32 ScColor[MAX_GAMMA];
typedef Uint32 ScPalette[MAX_COLORS*MAX_GAMMA];


SDL_Surface *ScRenderChar(unsigned const char car, ScFont *font, ScColor *color);
SDL_Surface *ScRenderText(const char *text, ScFont *font, ScColor *color, ScColor *color_hi = NULL, char delim = '\0');

ScFont *ScLoadFont(const char *filename);
ScPalette *ScLoadPalette(const char *filename);
ScColor *ScGetColor(const ScPalette *palette, int index);

void ScTestFont(ScFont *font, ScPalette *palette);


#endif
