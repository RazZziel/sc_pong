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



#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__


#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include "scfont.h"

enum {
	FLIP_HORIZONTAL	= 1,
	FLIP_VERTICAL	= 2
};

enum {
	H_CENTER		= 1,
	H_RIGHT			= 2,
	V_CENTER		= 4,
	V_DOWN			= 8,
	H_ANIM			= 16,
	V_ANIM			= 32,
	NO_BLIT			= 64
};


/* TTF Fonts */

typedef struct {
	const char *filename;
	const int index;
} IndexedFont;

IndexedFont const ttf_starcraft		= { "data/font/Starcraft.ttf",  0 };
IndexedFont const ttf_federation	= { "data/font/Federation.ttf", 1 };

/* Palettes and Colors */

SDL_Color const color_black		= {0, 0, 0};
SDL_Color const color_white		= {255, 255, 255};
SDL_Color const color_red		= {255, 0, 0};
SDL_Color const color_magenta	= {255, 0, 255};
SDL_Color const color_green		= {84, 188, 44};
SDL_Color const color_greenb	= {172, 244, 108};
SDL_Color const color_blue		= {168, 200, 224};

SDL_Color const defaultColorkey	= {0, 0, 0};
SDL_Color const defaultColor	= color_green;
SDL_Color const defaultHColor	= color_greenb;

/* Starcraft FNT fonts */

typedef struct {
	const char *filename;
	const int index;
} ScIndexedFont;

ScIndexedFont const font8x11	= { "data/font/font8.fnt",    0 };//
ScIndexedFont const font10x13	= { "data/font/font10.fnt",   1 };//		Version, Copyright	space = 6 [0 3 2 8]
ScIndexedFont const font12x12	= { "data/font/font12.fnt",   2 };// SUX
ScIndexedFont const font15x16	= { "data/font/font14.fnt",   3 };
ScIndexedFont const font18x19	= { "data/font/font16.fnt",   4 };// SUX
ScIndexedFont const font17x19	= { "data/font/font16_.fnt",  5 };//		Title				space = ?
ScIndexedFont const font25x19	= { "data/font/font16x.fnt",  6 };// SUX
ScIndexedFont const font25x19b	= { "data/font/font16x_.fnt", 7 };//		Main Menu			space = 13 [0 3 3 13]
ScIndexedFont const font46x36	= { "data/font/font32.fnt",   8 };// FAIL
ScIndexedFont const font32x35	= { "data/font/font32_.fnt",  9 };// SUX
ScIndexedFont const font52x50	= { "data/font/font50.fnt",  10 };// SUX

ScIndexedFont const * const f_main_menu = &font25x19b;


typedef struct {
	const char *filename;
} ScIndexedPalette;

ScIndexedPalette const pal_title		= { "data/font/tfont_title.png" };
ScIndexedPalette const pal_main_menu	= { "data/font/tfont_mainmenu.png" };
ScIndexedPalette const pal_game			= { "data/font/tfont_game.png" };


typedef struct {
	const ScIndexedPalette *palette;
	const int index;
} ScIndexedColor;

ScIndexedColor const c_tit_blue		= { &pal_title, 0 };
ScIndexedColor const c_tit_yellow	= { &pal_title, 1 };
ScIndexedColor const c_tit_white	= { &pal_title, 2 };
ScIndexedColor const c_tit_gray		= { &pal_title, 3 };
ScIndexedColor const c_tit_brown	= { &pal_title, 4 };

ScIndexedColor const c_mm_blue		= { &pal_main_menu, 0 };
ScIndexedColor const c_mm_green		= { &pal_main_menu, 1 };
ScIndexedColor const c_mm_greenh	= { &pal_main_menu, 2 };
ScIndexedColor const c_mm_gray		= { &pal_main_menu, 3 };
ScIndexedColor const c_mm_white		= { &pal_main_menu, 4 };
ScIndexedColor const c_mm_red		= { &pal_main_menu, 5 };

ScIndexedColor const c_gam_blue		= { &pal_game, 0 };
ScIndexedColor const c_gam_yellow	= { &pal_game, 1 };
ScIndexedColor const c_gam_white	= { &pal_game, 2 };
ScIndexedColor const c_gam_gray		= { &pal_game, 3 };
ScIndexedColor const c_gam_red		= { &pal_game, 4 };
ScIndexedColor const c_gam_green	= { &pal_game, 5 };



typedef struct {
	int x, y;
} dot;


inline int min(int x, int y) { return (x > y) ? y : x; }
inline int max(int x, int y) { return (x < y) ? y : x; }

const SDL_Rect rect_zero = {0, 0, 0, 0};

extern SDL_Surface *screen;


SDL_Color get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, SDL_Color color);


class Graphics {
	
	private:
		
		std::vector<SDL_Rect> rects;
		TTF_Font *ttf_sc[5];
		
		bool refresh_fs;
		
		ScIndexedFont *actual_font;
		ScIndexedPalette *actual_palette;
		ScFont *font;
		ScPalette *palette;
		
	public:
		
		Graphics();
		~Graphics();
		
		void toggle_fullscreen();
		
		SDL_Surface *copySurface(SDL_Surface *source, bool blit = true);
		
		SDL_Surface *load_image(const char *filename, const SDL_Color *colorkey = &defaultColorkey);
		void setTransparent(SDL_Surface *sprite, const SDL_Color *colorkey = &defaultColorkey);
		SDL_Surface *flip(SDL_Surface *surface, int flags);
		
		SDL_Rect *apply_surface(int x, int y,
						SDL_Surface *source, SDL_Surface *destination = screen,
						SDL_Rect *clip = NULL, int flags = 0, int old_x = 0, int old_y = 0);
		
		
		SDL_Surface *renderTextTTF(const char *txt,
						const IndexedFont *fnt, int size,
						const SDL_Color *fg_color	= &defaultColor,
						const SDL_Color *bg_color	= &defaultColorkey,
						const SDL_Color *fg_hi_color	= &defaultHColor,
						int style = TTF_STYLE_NORMAL);
		SDL_Surface *renderTextFNT(const char *txt,
						const ScIndexedFont *font,
						const ScIndexedColor *color,
						const ScIndexedColor *color_hi = NULL,
						const char delim = '\0');
		
		ScColor *getColor(const ScIndexedColor *color);
		ScFont *getFont(const ScIndexedFont *font);
		
		
		void refresh_screen();
		void refresh_whole_screen(bool refresh_inmediatly = true);
		
		inline void add_rect(SDL_Rect *rect) {
			//SDL_FillRect(screen, rect, 0x00FF00FF);
			//refresh_whole_screen(false);
			rects.push_back(*rect);
		}
		
};

#endif
