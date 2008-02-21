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

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "scfont.h"



ScFont *ScLoadFont(const char *filename) {
	
	ScFont *font = (ScFont*) malloc(sizeof(ScFont));
	
	/* Header */
	
	FILE *file = fopen(filename, "r");
	fread((void*)&font->header, sizeof(ScFontHeader), 1, file);
	
	//printf("%d %d %d %d\n", font->header.maxWidth, font->header.maxHeight, font->header.lowIndex, font->header.highIndex);
	
	/* Letters */
	
	// Offsets
	
	int n_offsets = font->header.highIndex - font->header.lowIndex;
	Uint32 *offsets = (Uint32*) malloc(sizeof(Uint32)*(n_offsets+1)); // +1 to allocate EoF
	
	fseek(file, sizeof(ScFontHeader), SEEK_SET);
	fread((void*)offsets, sizeof(Uint32), n_offsets, file);
	
	fseek(file, 0, SEEK_END);
	offsets[n_offsets] = ftell(file);  // End of file
	
	// Letters
	
	font->letters = (ScLetter*) malloc(sizeof(ScLetter)*(n_offsets+1)); // +1 to allocate 1st character, the space
	
	for (int i=1; i<=n_offsets; i++) {
		
		if (offsets[i-1]) {
			
			/* Header */
			
			fseek(file, offsets[i-1], SEEK_SET);
			fread((void*)&font->letters[i].header, sizeof(ScLetterHeader), 1, file);
			
			/*
			printf("(%d) %d: %d %d %d %d\n", offsets[i-1], i + font->header.lowIndex,
											font->letters[i].header.x,
											font->letters[i].header.y,
											font->letters[i].header.w,
											font->letters[i].header.h);
			getchar();
			*/
			
			/* Pixels */
			
			// Searching first higher offset not equal to 0
			// (Some fonts lack some letters)
			int next_offset = offsets[i];
			for (int o=0; !next_offset; o++) {
				next_offset = offsets[i+o];
			}
			
			font->letters[i].n_pixels = next_offset  - (offsets[i-1] + sizeof(ScLetterHeader)) - 1;
			font->letters[i].pixels = (Uint8*) malloc(sizeof(Uint8)*font->letters[i].n_pixels);
			
			fseek(file, (offsets[i-1] + sizeof(ScLetterHeader)), SEEK_SET);
			fread((void*)font->letters[i].pixels, sizeof(Uint8), font->letters[i].n_pixels, file);
			
		} else
			font->letters[i].n_pixels = -1;
		
	}
	
	// Space character will not be drawn, but will have a determined width
	font->letters[0].n_pixels = 0;
	font->letters[0].header.x = 0;
	font->letters[0].header.y = 0;
	font->letters[0].header.h = 0;
	font->letters[0].header.w = font->header.maxWidth/2; //font->letters[1].header.h-2;
	
	free(offsets);
	fclose(file);
	
	return font;
	
}

ScPalette *ScLoadPalette(const char *filename) {
	
	ScPalette *palette = (ScPalette*) malloc(sizeof(ScPalette));
	
	SDL_Surface *s_palette;
	if ((s_palette = SDL_DisplayFormat(IMG_Load(filename))) == NULL) {
		fprintf(stderr, "ERROR: Cannot load palette '%s'", filename);
		exit(1);
	}
	
	memcpy(*palette, s_palette->pixels, s_palette->format->BytesPerPixel*s_palette->w);
	
	/*
	for (int i=0; i<s_palette->w; i++) {
		SDL_Color c;
		SDL_GetRGB((*palette)[i], SDL_GetVideoSurface()->format, &c.r, &c.g, &c.b);
		printf("%d - %d %d %d\n", i, c.r, c.g, c.b);
	}
	getchar();
	*/
	
	SDL_FreeSurface(s_palette);
	
	return palette;
	
}


SDL_Surface *ScRenderChar(unsigned const char car, ScFont *font, ScColor *color) {
	
	SDL_Surface *screen = SDL_GetVideoSurface();
	
	int index = car - font->header.lowIndex;
	
	if ((car < font->header.lowIndex) || (car > font->header.highIndex) || (font->letters[index].n_pixels < 0)) {
		fprintf(stderr, "ERROR (ScRenderChar): Invalid character index %d\n", car);
		return NULL;
	}
	
	SDL_Surface *letter = SDL_CreateRGBSurface(SDL_SWSURFACE,
			font->letters[index].header.w, font->letters[index].header.h,
			screen->format->BitsPerPixel, screen->format->Rmask,
			screen->format->Gmask, screen->format->Bmask,
			screen->format->Amask);
	
	if (font->letters[index].header.h > 0) {
		char *pixels = (char*) letter->pixels - letter->format->BytesPerPixel;
		
		for (int p=0; p<font->letters[index].n_pixels; p++) {
			
			Uint8 skip = font->letters[index].pixels[p] >> 3;       // 11111000
			Uint8 gamma = font->letters[index].pixels[p] & 7;       // 00000111
			
			pixels += skip * letter->format->BytesPerPixel + letter->format->BytesPerPixel;
			//printf("--%d\n", gamma);
			memcpy(pixels, *color+gamma, letter->format->BytesPerPixel);
			
		}
		
		SDL_SetColorKey(letter, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0);
		
	}
	
	SDL_Surface *final_letter = SDL_CreateRGBSurface(SDL_SWSURFACE,
			font->letters[index].header.w, font->header.maxHeight,
			screen->format->BitsPerPixel, screen->format->Rmask,
			screen->format->Gmask, screen->format->Bmask,
			screen->format->Amask);
	
	//SDL_Rect bleh = {0, 0, final_letter->w, final_letter->h};/
	//SDL_FillRect(final_letter, &bleh, 0x000000FF);
	
	SDL_Rect offset = {font->letters[index].header.x, font->letters[index].header.y, 0, 0};
	SDL_BlitSurface(letter, NULL, final_letter, &offset);
	SDL_FreeSurface(letter);
	
	SDL_SetColorKey(final_letter, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0);
	
	return final_letter;
	
}

int calculateWidth(const unsigned char *text, ScFont *font, char delim) {
	
	int width = 0;
	
	while (*text) {                                                      //  <- one exta pixel to space letters
		if (*text != delim) width += font->letters[*text - font->header.lowIndex].header.w + 1;
		text++;
	}
	
	return width-1;
	
}

SDL_Surface *ScRenderText(const char *text, ScFont *font, ScColor *color, ScColor *color_hi, char delim) {

	/*
		Renders the selected 'text' with the selected 'font'.
		By default, the text is rendered with 'color', but if a 'color_hi' and
		a 'delim'iter are specifyed, the characters surrounded by two occurences
		of 'delim' will be rendered in 'color_hi' (this is: they are highlighted).
	*/

	SDL_Surface *screen = SDL_GetVideoSurface();
	
	SDL_Surface *s_text = SDL_CreateRGBSurface(SDL_SWSURFACE,
						calculateWidth((unsigned char*)text, font, delim), font->header.maxHeight,
						screen->format->BitsPerPixel, screen->format->Rmask,
						screen->format->Gmask, screen->format->Bmask,
						screen->format->Amask);
	
	SDL_SetColorKey(s_text, SDL_SRCCOLORKEY|SDL_RLEACCEL, 0);
	
	SDL_Surface *s_char;
	
	SDL_Rect offset = {0, 0, 0, 0};
	
	if (color_hi) {
		
		ScColor *actual_color = color;
		
		while(*text) {
			
			// Color change
			if (*text == delim) {
				if (actual_color == color)
					actual_color = color_hi;
				else
					actual_color = color;
				
			// Char rendering
			} else {
				if ((s_char = ScRenderChar(*text, font, actual_color))) {
					SDL_BlitSurface(s_char, NULL, s_text, &offset);
					offset.x += s_char->w + 1;
					SDL_FreeSurface(s_char);
				}
			}
			
			text++;
			
		}
		
	} else {
		
		while(*text) {
			if (*text != delim) {
				if ((s_char = ScRenderChar(*text, font, color))) {
					SDL_BlitSurface(s_char, NULL, s_text, &offset);
					offset.x += s_char->w + 1;
					SDL_FreeSurface(s_char);
				}
			}
			text++;
		}
		
	}
	
	return s_text;
	
}

ScColor *ScGetColor(const ScPalette *palette, int index) {
	
	/*
	for (int i=0; i<48; i++) {
		SDL_Color c;
		SDL_GetRGB((*palette)[i], SDL_GetVideoSurface()->format, &c.r, &c.g, &c.b);
		printf("(%d %d %d) ", c.r, c.g, c.b);
	}
	printf("\n");
	*/
	
	ScColor *color = (ScColor*) malloc(sizeof(ScColor));
	memcpy(*color, *palette + (index * MAX_GAMMA), sizeof(ScColor));
	
	/*
	printf(">%x %d\n", palette, index);
	for (int i=0; i<8; i++) {
		SDL_Color c;
		SDL_GetRGB((*color)[i], SDL_GetVideoSurface()->format, &c.r, &c.g, &c.b);
		printf("(%d %d %d) ", c.r, c.g, c.b);
	}
	printf("\n");
	getchar();
	*/
	
	return color;
	
}

void ScTestFont(ScFont *font, ScPalette *palette) {
	
#define TEST_SDL
	
	SDL_Surface *screen = SDL_GetVideoSurface();
	
	SDL_FillRect(screen, NULL, SDL_MapRGB(SDL_GetVideoSurface()->format, 255, 255, 255));
	SDL_Flip(screen);
	
#ifdef TEST_SDL
	
	SDL_Rect offset = {10, 10, 0, 0};
	SDL_BlitSurface(ScRenderText("El veloz murci�ago hind", font, ScGetColor(palette, 1)),
			NULL, screen, &offset);
	offset.y += font->header.maxHeight+2;
	SDL_BlitSurface(ScRenderText("Juiporentenderlas", font, ScGetColor(palette, 5)),
			NULL, screen, &offset);
	offset.y += font->header.maxHeight+2;
	SDL_BlitSurface(ScRenderText("_S_ingle Player", font, ScGetColor(palette, 1), ScGetColor(palette, 2), '_'),
			NULL, screen, &offset);
	offset.y += font->header.maxHeight+2;
	
#endif
	
	for (int i=font->header.lowIndex; i<=font->header.highIndex; i++) {
		
#ifdef TEST_SDL
		
		SDL_Surface *s_char = ScRenderChar(i, font, ScGetColor(palette, 2));
		
		if (s_char) {
			SDL_BlitSurface(s_char, NULL, screen, &offset);
			SDL_FreeSurface(s_char);
		}
		
		if ((offset.x += font->header.maxWidth+2) > (screen->w - font->header.maxWidth - 10)) {
			offset.x = 10;
			offset.y += font->header.maxHeight+2;
		}
		
		
#else
		int index = i - font->header.lowIndex;
		
		int x = 1;
		for (int p=0; p<font->letters[index].n_pixels; p++)
			printf("%d ", (font->letters[i].pixels[p] >> 3));
		printf("\n");
		
		for (int p=0; p<font->letters[index].n_pixels; p++) {
			
			Uint8 skip = font->letters[index].pixels[p] >> 3;	// 11111000
			Uint8 color = font->letters[index].pixels[p] & 7;	// 00000111
			
			for (int j=0; j<skip; j++) {
				printf(" ");
				if (x++ == font->letters[index].header.w) {
					printf("\n");
					x = 1;
				}
			}
			printf("%d", color);
			//printf("#");
			if (x++ == font->letters[index].header.w) {
				printf("\n");
				x = 1;
			}
			
			
		}
		
		printf("\ntest end\n");
		getchar();
		
#endif
		
	}
	
	SDL_Flip(screen);
	*screen = *SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h,
					screen->format->BitsPerPixel, screen->format->Rmask,
					screen->format->Gmask, screen->format->Bmask,
					screen->format->Amask);
	
	getchar();
	
}

