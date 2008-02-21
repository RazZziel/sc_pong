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



#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include "CGraphics.h"
#include "CControl.h"
#include "audio.h"
#include "widgets.h"


extern SDL_Surface *screen;
extern Graphics *graphics;
extern Control *control;


/* Window */

Window::Window(int x, int y, int w, int h, const char *title) {
	
	snd_button = loadSound(S_BUTTON);
	
	buildBackground(w, h, title);
	win_rect.x = x - background->w/2;
	win_rect.y = y;
	win_rect.w = background->w;
	win_rect.h = background->h;
	
	loadButtonTiles();
	
}

Window::~Window() {
	
	SDL_FreeSurface(background);
	
}


void Window::loadButtonTiles() {
	
	s_button_tiles = graphics->load_image(P_BUTTON, NULL);
	
	for (int i=0; i<12; i++) {
		r_button_tiles[i].x = i*BUT_W;
		r_button_tiles[i].y = 0;
		r_button_tiles[i].w = BUT_W;
		r_button_tiles[i].h = BUT_H;
	}
	
}

void Window::buildBackground(int w, int h, const char *title) {
	
	SDL_Surface *s_window = graphics->load_image(P_WINDOW);
	SDL_Rect r_window[9];
	
	// Load window tiles
	
	Uint8 index = 0;
	for (int j=0; j<3; j++) {
		for (int i=0; i<3; i++) {
			r_window[index].x = i*WIN_W;
			r_window[index].y = j*WIN_H;
			r_window[index].w = WIN_W;
			r_window[index].h = WIN_H;
			index++;
		}
	}
	
	
	background = SDL_CreateRGBSurface(SDL_SWSURFACE,
									  w*WIN_W, h*WIN_H,                   // Wii.
									  s_window->format->BitsPerPixel,
									  0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	
	
	/* Build Window */
	
	
	SDL_Rect offset = {0, 0, 0, 0};
	SDL_BlitSurface(s_window, &r_window[0], background, &offset); // UpLeft
	offset.x = (w-1)*WIN_W;
	SDL_BlitSurface(s_window, &r_window[2], background, &offset); // UpRight
	offset.y = (h-1)*WIN_H;
	SDL_BlitSurface(s_window, &r_window[8], background, &offset); // DownRight
	offset.x = 0;
	SDL_BlitSurface(s_window, &r_window[6], background, &offset); // DownLeft
	
	offset.y = 0;
	for (int i=1; i<(w-1); i++) {
		offset.x = i*WIN_W;
		SDL_BlitSurface(s_window, &r_window[1], background, &offset); // Up
	}
	offset.y = (h-1)*WIN_H;
	for (int i=1; i<(w-1); i++) {
		offset.x = i*WIN_W;
		SDL_BlitSurface(s_window, &r_window[7], background, &offset); // Down
	}
	offset.x = 0;
	for (int j=1; j<(h-1); j++) {
		offset.y = j*WIN_H;
		SDL_BlitSurface(s_window, &r_window[3], background, &offset); // Left
	}
	offset.x = (w-1)*WIN_W;
	for (int j=1; j<(h-1); j++) {
		offset.y = j*WIN_H;
		SDL_BlitSurface(s_window, &r_window[5], background, &offset); // Right
	}
	
	for (int j=1; j<(h-1); j++) {
		offset.y = j*WIN_H;
		for (int i=1; i<(w-1); i++) {
			offset.x = i*WIN_W;
			SDL_BlitSurface(s_window, &r_window[4], background, &offset); // Center
		}
	}
	
	SDL_FreeSurface(s_window);
	
	
	// Make it semi-transparent
	
	Uint32 *buf = (Uint32*) background->pixels;
	
	register Uint32 fill_color = buf[(background->w*background->h)/2+1];
	register Uint32 fill_transparent = fill_color & 0xD0FFFFFF; // 0x80 = 128 = 50% transparent
	
	for (register int j=0; j<background->h; j++) {
		for (register int i=0; i<background->w; i++, buf++) {
			if (*buf == fill_color)
				*buf = fill_transparent;
			else if (*buf == 0xFF000000)
				*buf = 0x00000000;
		}
	}
	
	/* Window title */
	
	if (title != NULL) {
		
		SDL_Surface *s_title = graphics->renderTextFNT(title, &font17x19, &c_gam_blue);
		graphics->apply_surface(background->w/2, 12, s_title, background, NULL, H_CENTER);
		SDL_FreeSurface(s_title);
		
	}
	
	
}

void Window::hide() {
	
	graphics->add_rect(&win_rect);
	
}

SDL_Surface *Window::createButton(int w, const char *text, ButtonType type) {
	
	SDL_Surface *button = SDL_CreateRGBSurface(SDL_SWSURFACE, w*BUT_W, BUT_H,
											   s_button_tiles->format->BitsPerPixel,
											   s_button_tiles->format->Rmask,
											   s_button_tiles->format->Gmask,
											   s_button_tiles->format->Bmask,
											   s_button_tiles->format->Amask);
	
	SDL_Rect offset = {0, 0, 0, 0};
	
	SDL_BlitSurface(s_button_tiles, &r_button_tiles[0+type], button, &offset); // Left
	for (int i=1; i<(w-1); i++) {
		offset.x += BUT_W;
		SDL_BlitSurface(s_button_tiles, &r_button_tiles[1+type], button, &offset); // Center
	}
	offset.x += BUT_W;
	SDL_BlitSurface(s_button_tiles, &r_button_tiles[2+type], button, &offset); // Right
	
	graphics->setTransparent(button);
	
	if (text != NULL) {
		SDL_Surface *s_text = graphics->renderTextFNT(text, &font17x19, &c_gam_yellow, &c_gam_white, '_');
		graphics->apply_surface(button->w/2, button->h/2, s_text, button, NULL, H_CENTER | V_CENTER);
		SDL_FreeSurface(s_text);
	}
	
	return button;
	
}

SDL_Surface *Window::createScrollBar(int h) {
	
	SDL_Surface *s_sbar_tiles = graphics->load_image(P_SCROLL);
	SDL_Rect r_sbar_tiles[6];
	
	// Load window tiles
	
	for (int j=0; j<6; j++) {
		r_sbar_tiles[j].x = 0;
		r_sbar_tiles[j].y = j*BAR_W;
		r_sbar_tiles[j].w = BAR_W;
		r_sbar_tiles[j].h = BAR_W;
	}
	
	SDL_Surface *s_bar = SDL_CreateRGBSurface(SDL_SWSURFACE, BAR_W, h*BAR_H + 2*BAR_H + 4,
											  s_sbar_tiles->format->BitsPerPixel,
											  s_sbar_tiles->format->Rmask,
											  s_sbar_tiles->format->Gmask,
											  s_sbar_tiles->format->Bmask,
											  s_sbar_tiles->format->Amask);
	
	SDL_Rect offset = {0, 0, 0, 0};
	
	SDL_BlitSurface(s_sbar_tiles, &r_sbar_tiles[3], s_bar, &offset); // Up arrow
	offset.y += BAR_H + 2;
	SDL_BlitSurface(s_sbar_tiles, &r_sbar_tiles[0], s_bar, &offset); // Up
	for (int j=1; j<(h-1); j++) {
		offset.y += BAR_H;
		SDL_BlitSurface(s_sbar_tiles, &r_sbar_tiles[1], s_bar, &offset); // Center
	}
	offset.y += BAR_H;
	SDL_BlitSurface(s_sbar_tiles, &r_sbar_tiles[2], s_bar, &offset); // Down
	offset.y += BAR_H + 2;
	SDL_BlitSurface(s_sbar_tiles, &r_sbar_tiles[4], s_bar, &offset); // Down arrow
	
	graphics->setTransparent(s_bar);
	
	SDL_FreeSurface(s_sbar_tiles);
	
	return s_bar;
	
}



/*
 * Game Menu Window
 */



GameMenuWindow::GameMenuWindow(int x, int y, int w, int h, const char *title, const char **options)
	: Window(x, y, w, h, title),
	option(GOPT_RETURN),
	options(options) {
	
	
	/* Build option buttons */
	
	if (options != NULL) {
		
		SDL_Surface *s_option;
		int opt_y = 36;
		
		// (Return|Ok) button
		s_option = createButton(BUT_W2, *options);
		graphics->apply_surface(background->w/2, background->h - 8 - BUT_H, s_option, background, NULL, H_CENTER);
		button_rect.x = x - s_option->w/2;//FIXME wtf
		button_rect.y = y - s_option->h/2;//FIXME wtf
		button_rect.w = s_option->w;
		button_rect.h = s_option->h;
		SDL_FreeSurface(s_option);
		
		options++;
		
		// Other buttons
		while (*options) {
			
			s_option = createButton(BUT_W2, *options);
			graphics->apply_surface(background->w/2, opt_y, s_option, background, NULL, H_CENTER);
			SDL_FreeSurface(s_option);
			
			opt_y += 34;
			options++;
			
		}
		
		// Default selected button
		s_button = createButton(BUT_W2, options[option], bt_2);
		
	}
	
}

GameMenuWindow::~GameMenuWindow() {
	
	
}

void GameMenuWindow::draw() {
	
	graphics->add_rect(graphics->apply_surface(win_rect.x, win_rect.y, background, screen));
	button_rect = *graphics->apply_surface(win_rect.x + background->w/2, win_rect.y + button_y(option), s_button, screen, NULL, H_CENTER);
	graphics->add_rect(&button_rect);
	
}

GameMenuOpt GameMenuWindow::handleInput() {
	
	control->updateState();
	
	if (control->isPressed(KEY_BACK)) {
		playSound(snd_button);
		return GOPT_RETURN;
		
	} else if (control->isPressed(KEY_OK)) {
		SDL_Surface *s_button2 = createButton(BUT_W2, options[option], bt_3);
		graphics->apply_surface(0, 0, s_button2, s_button);
		SDL_FreeSurface(s_button2);
		playSound(snd_button);
		return option;
		
	} else if (control->isPressed(KEY_UP)) {
		option--;
		
		graphics->add_rect(&button_rect);
		SDL_FreeSurface(s_button);
		s_button = createButton(BUT_W2, options[option], bt_2);
		
		return GOPT_NONE;
		
	} else if (control->isPressed(KEY_DOWN)) {
		option++;
		
		graphics->add_rect(&button_rect);
		SDL_FreeSurface(s_button);
		s_button = createButton(BUT_W2, options[option], bt_2);
		
		return GOPT_NONE;
		
	}
	
	return GOPT_NONE;
	
}


/*
 * Help Window
 */


HelpWindow::HelpWindow(int x, int y, int w, int h, const char *title, const char **lines) : Window(x, y, w, h, title),
																				scroll_y(0) {
	
	
	s_text = SDL_CreateRGBSurface(SDL_SWSURFACE, scroll_rect.w, scroll_rect.h,
								  background->format->BitsPerPixel,
								  background->format->Rmask,
								  background->format->Gmask,
								  background->format->Bmask,
								  background->format->Amask);
	
	/* Scroll bar */
	
	SDL_Surface *s_bar = createScrollBar(BAR_H1);
	graphics->apply_surface(sbar_rect.x, sbar_rect.y - 18, s_bar, background);
	SDL_FreeSurface(s_bar);
	
	/* Print help lines */
	
	if (lines != NULL) {
		
		SDL_Surface *s_line;
		
		// Ok button
		s_line = createButton(BUT_W1, "_O_k");
		button_rect.x = (background->w - s_line->w)/2;
		button_rect.y = background->h - 8 - BUT_H;
		button_rect.w = s_line->w;
		button_rect.h = s_line->h;
		graphics->apply_surface(button_rect.x, button_rect.y, s_line, background, NULL);
		SDL_FreeSurface(s_line);
		
		// Help lines
		int line_y = 0;
		while (*lines) {
			
			
			
			if (strcmp(*lines, "")) {
				
				if (**lines == '#')
					s_line = graphics->renderTextFNT(*lines, &font10x13, &c_gam_yellow, NULL, '#');
				else
					s_line = graphics->renderTextFNT(*lines, &font10x13, &c_gam_blue, &c_gam_white, '_');
				
				graphics->apply_surface(0, line_y, s_line, s_text);
				SDL_FreeSurface(s_line);
				
			}
			
			line_y += 16;
			lines++;
			
		}
		
		// Default selected button
		s_button = createButton(BUT_W1, "_O_k", bt_2);
		
	}
	
}


HelpWindow::~HelpWindow() {
	
	
	
}

void HelpWindow::draw() {
	
	graphics->add_rect(graphics->apply_surface(win_rect.x, win_rect.y, background, screen));
	graphics->apply_surface(win_rect.x + scroll_rect.x, win_rect.y + scroll_rect.y, s_text, screen);
	
}

GameMenuOpt HelpWindow::handleInput() {
	
	control->updateState();
	
	
	if (control->isPressed(KEY_BACK) || control->isPressed(KEY_OK)) {
		playSound(snd_button);
		return GOPT_RETURN;
		
	} else if (control->isPressed(KEY_UP)) {
		
		
		return GOPT_NONE;
		
	} else if (control->isPressed(KEY_DOWN)) {
		
		
		return GOPT_NONE;
		
	}
	
	return GOPT_NONE;
	
}
