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
#include <string.h>
#include <math.h>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#include "error.h"
#include "main.h"
#include "scfont.h"
#include "CGraphics.h"



extern SDL_Surface *screen;


SDL_Color get_pixel(SDL_Surface *surface, int x, int y) {

	char* pos = (char*) surface->pixels;
	pos += surface->pitch * y;
	pos += surface->format->BytesPerPixel * x;

	Uint32 col;
	memcpy(&col, pos, surface->format->BytesPerPixel);

	SDL_Color color;
	SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);

	return color;

}


void put_pixel(SDL_Surface *surface, int x, int y, SDL_Color color) {

	char* pos = (char*) surface->pixels ;
	pos += (surface->pitch * y);
	pos += (surface->format->BytesPerPixel * x);

	Uint32 col = SDL_MapRGB(surface->format, color.r, color.g, color.b);
	memcpy(pos, &col, surface->format->BytesPerPixel);

}




Graphics::Graphics() : refresh_fs(false), actual_font(NULL), actual_palette(NULL), font(NULL), palette(NULL) {
	
	Uint32 screen_flags = 0;
	screen_flags |= SDL_ANYFORMAT;
	screen_flags |= SDL_HWSURFACE;//TODO load config
	
	if ((screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, screen_flags)) == NULL)
		die(ERR_INIT, "Video Mode");
	SDL_WM_SetCaption(WINDOW_TITLE, NULL);
	
	if (TTF_Init() == -1) die(ERR_INIT, "fonts");
	
	SDL_ShowCursor(0);
	
	if ((ttf_sc[0] = TTF_OpenFont(ttf_starcraft.filename,  7)) == NULL) die(ERR_LOAD, ttf_starcraft.filename);
	if ((ttf_sc[1] = TTF_OpenFont(ttf_starcraft.filename, 10)) == NULL) die(ERR_LOAD, ttf_starcraft.filename);
	if ((ttf_sc[2] = TTF_OpenFont(ttf_starcraft.filename, 70)) == NULL) die(ERR_LOAD, ttf_starcraft.filename);
	if ((ttf_sc[3] = TTF_OpenFont(ttf_starcraft.filename, 90)) == NULL) die(ERR_LOAD, ttf_starcraft.filename);
	if ((ttf_sc[4] = TTF_OpenFont(ttf_starcraft.filename, 100)) == NULL) die(ERR_LOAD, ttf_starcraft.filename);
	
	//if ((fonts[1][0] = TTF_OpenFont(ttf_federation.filename,  7)) == NULL) die(ERR_LOAD, ttf_federation.filename);
	//if ((fonts[1][1] = TTF_OpenFont(ttf_federation.filename, 12)) == NULL) die(ERR_LOAD, ttf_federation.filename); // Loading
	//if ((fonts[1][2] = TTF_OpenFont(ttf_federation.filename, 17)) == NULL) die(ERR_LOAD, ttf_federation.filename); // Main menu
	//if ((fonts[1][3] = TTF_OpenFont(ttf_federation.filename, 30)) == NULL) die(ERR_LOAD, ttf_federation.filename);
	//if ((fonts[1][4] = TTF_OpenFont(ttf_federation.filename, 40)) == NULL) die(ERR_LOAD, ttf_federation.filename);
	
	//ScTestFont(ScLoadFont(font8x11.filename), ScLoadPalette(pal_main_menu.filename));//ok
	//ScTestFont(ScLoadFont(font10x13.filename), ScLoadPalette(pal_main_menu.filename));//ok
	//ScTestFont(ScLoadFont(font15x16.filename), ScLoadPalette(pal_main_menu.filename));//ok
	//ScTestFont(ScLoadFont(font17x19.filename), ScLoadPalette(pal_main_menu.filename));//ok
	//ScTestFont(ScLoadFont(font25x19b.filename), ScLoadPalette(pal_main_menu.filename));//ok
	
	//ScTestFont(ScLoadFont(font12x12.filename), ScLoadPalette(pal_main_menu.filename));//SUX
	//ScTestFont(ScLoadFont(font18x19.filename), ScLoadPalette(pal_main_menu.filename));//SUX
	//ScTestFont(ScLoadFont(font25x19.filename), ScLoadPalette(pal_main_menu.filename));//SUX
	//ScTestFont(ScLoadFont(font32x35.filename), ScLoadPalette(pal_main_menu.filename));//SUX
	//ScTestFont(ScLoadFont(font52x50.filename), ScLoadPalette(pal_main_menu.filename));//SUX
	
	//ScTestFont(ScLoadFont(font46x36.filename), ScLoadPalette(pal_main_menu.filename));//FAIL
	
}

Graphics::~Graphics() {
	
	for (int i=0; i<5; i++) {
		TTF_CloseFont(ttf_sc[i]);
	}
	
	SDL_ShowCursor(1);
	
}

void Graphics::toggle_fullscreen() {
	
	Uint32 flags = screen->flags;
	flags ^= SDL_FULLSCREEN; // XOR for president.
	if ((screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, flags)) == NULL)
		die(ERR_INIT, "Video Mode");
	
	refresh_fs = true;
	
}






SDL_Surface *Graphics::load_image(const char *filename, const SDL_Color *colorkey) {

	SDL_Surface * loaded_img;
	SDL_Surface * optim_img;

	if ((loaded_img = IMG_Load(filename)) == NULL) die(ERR_LOAD, filename); 
	if ((optim_img = SDL_DisplayFormat(loaded_img)) == NULL ) die(ERR_LOAD, filename); 

	SDL_FreeSurface(loaded_img);
	
	if (colorkey != NULL) setTransparent(optim_img, colorkey);
	
	return optim_img;
	
}

SDL_Surface *Graphics::copySurface(SDL_Surface *source, bool blit) {
	
	SDL_Surface *destination = SDL_CreateRGBSurface(SDL_SWSURFACE, source->w, source->h,
													source->format->BitsPerPixel,
													source->format->Rmask,
													source->format->Gmask,
													source->format->Bmask,
													source->format->Amask);
	
	if (blit) SDL_BlitSurface(source, NULL, destination, NULL);
	
	return destination;
	
}


SDL_Surface *Graphics::flip(SDL_Surface *surface, int flags) {

	SDL_Surface *new_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, \
			 										surface->format->BitsPerPixel, surface->format->Rmask,\
													surface->format->Gmask, surface->format->Bmask, \
													surface->format->Amask);

	if(SDL_MUSTLOCK(surface)) 		{ SDL_LockSurface(surface); }
	if(SDL_MUSTLOCK(new_surface))	{ SDL_LockSurface(new_surface); }


	for (int y=0; y<surface->h; y++) {
			for (int x=0; x<surface->w; x++) {
			
				if ((flags & FLIP_HORIZONTAL) && (flags & FLIP_VERTICAL)) {
					put_pixel(new_surface, surface->w -1 -x, surface->h -1 -y, get_pixel(surface, x, y));

				} else if (flags & FLIP_HORIZONTAL) {
					put_pixel(new_surface, surface->w -1 -x, y, get_pixel(surface, x, y));
				
				} else if (flags & FLIP_VERTICAL) {
					put_pixel(new_surface, x, surface->h -1 -y, get_pixel(surface, x, y));
				
				}
				
			}
		}
	

	if(SDL_MUSTLOCK(surface)) 		{ SDL_UnlockSurface(surface); }
	if(SDL_MUSTLOCK(new_surface))	{ SDL_UnlockSurface(new_surface); }

	return new_surface;

}


void Graphics::setTransparent(SDL_Surface *surface, const SDL_Color *colorkey) {
	
	Uint32 transparentColor = SDL_MapRGB(surface->format, colorkey->r, colorkey->g, colorkey->b);
	SDL_SetColorKey(surface, (SDL_SRCCOLORKEY|SDL_RLEACCEL), transparentColor);
	// FIXME ver si RLE no la jode 
	
}


SDL_Surface *Graphics::renderTextTTF(const char *txt, const IndexedFont *fnt, int size,
					const SDL_Color *fg_color,
					const SDL_Color *bg_color,
					const SDL_Color *fg_hi_color,
					int style) {
	
	SDL_Surface *s_text;
	TTF_Font *font;
	
	switch(fnt->index) {
		case 0: font = ttf_sc[size]; break;
		case 1: font = ttf_sc[size]; break;
		default: break;
	}
	
	if (style != TTF_STYLE_NORMAL)
		TTF_SetFontStyle(font, style);
	
	
	const char *tok = strchr(txt, '_');
	
	/* Text has a highlighted letter */
	if(tok) {
		
		int width = 0;
		SDL_Surface *s_pre, *s_hi, *s_post;
		
		// Prefix
		if (tok > txt) {
			char *pre = (char*) malloc(sizeof(char)*(tok-txt+1));
			strncpy(pre, txt, (tok-txt));
			pre[tok-txt] = '\0';
			
			//printf("> %s\n", pre);
			if (((s_pre = TTF_RenderText_Blended(font, pre, *fg_color)) == NULL))
				die(ERR_CUSTOM, "Cannot render text");
			width += s_pre->w;
			free(pre);
		} else
			s_pre = NULL;
		
		// Highlighted letter
		char hi[2];
		hi[0] = *(tok+1);
		hi[1] = '\0';
		//printf("> %s\n", hi);
		//TTF_SetFontStyle(font, TTF_STYLE_BOLD);//TODO queda bien
		if ((s_hi = TTF_RenderText_Blended(font, hi, *fg_hi_color)) == NULL)
			die(ERR_CUSTOM, "Cannot render text");
		//TTF_SetFontStyle(font, TTF_STYLE_NORMAL);//TODO pero queda mal.
		width += s_hi->w;
		
		// Postfix
		if(*(tok+2) != 0) {
			//printf("> %s\n", tok+2);
			if ((s_post = TTF_RenderText_Blended(font, tok+2, *fg_color)) == NULL)
				die(ERR_CUSTOM, "Cannot render text");
			width += s_post->w;
		} else
			s_post = NULL;
		
		// Blit
		s_text = SDL_CreateRGBSurface(SDL_SWSURFACE, width, s_hi->h,
									  screen->format->BitsPerPixel, screen->format->Rmask,
									  screen->format->Gmask, screen->format->Bmask,
									  screen->format->Amask);
		setTransparent(s_text, &color_black);
		int offset;
		if (s_pre != NULL) {
			apply_surface(0, 0, s_pre, s_text);
			offset = s_pre->w;
			SDL_FreeSurface(s_pre);
		} else
			offset = 0;
		apply_surface(offset, 0, s_hi, s_text);
		offset += s_hi->w;
		SDL_FreeSurface(s_hi);
		if(s_post != NULL) {
			apply_surface(offset, 0, s_post, s_text);
			SDL_FreeSurface(s_post);
		}
		
		
	/* Normal render, without highlighted letter */
	} else {
		
		//if ((text = TTF_RenderText_Solid(font, txt, fg_color)) == NULL)
		//if ((text = TTF_RenderText_Shaded(font, txt, fg_color, bg_color)) == NULL)
		if ((s_text = TTF_RenderText_Blended(font, txt, *fg_color)) == NULL)
			die(ERR_CUSTOM, "Cannot render text");
		
		//setTransparent(text);
		
	}
	
	if (style != TTF_STYLE_NORMAL)
		TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	
	return s_text;
	
}


SDL_Surface *Graphics::renderTextFNT(const char *txt, const ScIndexedFont *font,
					const ScIndexedColor *color, const ScIndexedColor *color_hi,
					const char delim) {
	//printf("%s\n", font->filename);
	ScColor *sc_color = getColor(color);
	ScColor *sc_color_hi = getColor(color_hi);
	return ScRenderText(txt, getFont(font), sc_color, sc_color_hi, delim);
	free(sc_color);
	free(sc_color_hi);
	
}

ScColor *Graphics::getColor(const ScIndexedColor *color) {
	
	/* We only have to select a color to draw the text. *
	 * Palettes are loaded and erased automatically.    */
	
	if (color) {
		
		if (color->palette != actual_palette) {
			actual_palette = (ScIndexedPalette*) color->palette;
			if (palette) free(palette);
			palette = ScLoadPalette(actual_palette->filename);
		}
		
		return ScGetColor(palette, color->index);
		
	} else
		
		return NULL;
	
}

ScFont *Graphics::getFont(const ScIndexedFont *font) {
	
	if (font != actual_font) {
		actual_font = (ScIndexedFont*) font;
		this->font = ScLoadFont(font->filename);
	}
	
	return this->font;
	
}

SDL_Rect *Graphics::apply_surface(int x, int y,
								  SDL_Surface *source, SDL_Surface *destination,
								  SDL_Rect *clip, int flags, int old_x, int old_y) {
	
	if (flags & H_RIGHT)		x -= source->w;
	else if (flags & H_CENTER)	x -= (source->w)/2;
	if (flags & V_DOWN)			y -= source->h;
	else if (flags & V_CENTER)	y -= (source->h)/2;
	
	SDL_Rect offset = {x, y, 0, 0};
	
	if (!(flags & NO_BLIT))
		SDL_BlitSurface(source, clip, destination, &offset);
	
	SDL_Rect *rect = (SDL_Rect*) malloc(sizeof(SDL_Rect));
	rect->x = x;
	rect->y = y;
	rect->w = source->w;
	rect->h = source->h;
	
	// Checking if we are drawing only a portion of the surface
	if (clip) {
		rect->w = clip->w;
		rect->h = clip->h;
	}
	
	// Checking if the surface is moving across the screen,
	// so we must clear the blank space it leaves behind
	if (flags & H_ANIM) {
		rect->x = min(x, old_x);
		rect->w += abs(old_x - x);
	}
	if (flags & V_ANIM) {
		rect->y = min(y, old_y);
		rect->h += abs(old_y - y);
	}
	
	// Checking bounds with screen
	if (rect->x < 0)					{ rect->w += rect->x; rect->x = 0; }
	else if (rect->x > SCREEN_WIDTH)	rect->x = SCREEN_WIDTH;
	if (rect->y < 0)					{ rect->h += rect->y; rect->y = 0; }
	else if (rect->y > SCREEN_HEIGHT)	rect->y = SCREEN_HEIGHT;
	
	if ((rect->x + rect->w) > SCREEN_WIDTH)		rect->w = SCREEN_WIDTH - rect->x;
	if ((rect->y + rect->h) > SCREEN_HEIGHT)	rect->h = SCREEN_HEIGHT - rect->y;
	
	return rect;
	
}

void Graphics::refresh_whole_screen(bool refresh_inmediatly) {
	
	// Sometimes, instead of refreshing inmediatly the whole screen, we
	// want to refresh it after blitting the background in a bussy loop,
	// so we only mark it to be refreshed.
	
	refresh_fs = true;
	if (refresh_inmediatly) refresh_screen();
}

void Graphics::refresh_screen() {
	
	if (refresh_fs) {
		
		refresh_fs = false;
		if (SDL_Flip(screen)==-1) die(ERR_CUSTOM, "Cannot refresh screen");
	
	} else {
		
		if (rects.size() > 0)
			SDL_UpdateRects(screen, rects.size(), &rects[0]);
		
	}
	
	//TODO liberar memoria
	// se puede hacer que addRect deje elegir entre añadir el rect o no
	// a un vector de direcciones que se deben liberar,
	// o alocatear todos los rects dinamicamente
	rects.clear();
	
}
