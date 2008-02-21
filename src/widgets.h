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



#ifndef __WIDGETS_H__
#define __WIDGETS_H__


#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"


#include "menus.h"

#define P_WINDOW	"data/pixmap/menu/widgets/window.png"
#define P_BUTTON	"data/pixmap/menu/widgets/button.png"
#define P_SCROLL	"data/pixmap/menu/widgets/scrollbar.png"


enum {
	WIN_W	= 8,
	WIN_H	= 8,
	
	BUT_W	= 8,
	BUT_H	= 28,
	BUT_W1	= 104/BUT_W,
	BUT_W2	= 224/BUT_W,
	
	BAR_W	= 16,
	BAR_H	= 16,
	BAR_H1	= 176/BAR_H
};

typedef enum {
	bt_0 = 0,
	bt_1 = 3,
	bt_2 = 6,
	bt_3 = 9
} ButtonType;


class Window {
	
	protected:
		SDL_Surface *background;
		SDL_Rect win_rect;
		
		SDL_Surface *s_button;
		SDL_Rect button_rect;
		
		SDL_Surface *s_button_tiles;
		SDL_Rect r_button_tiles[12];
		
		Mix_Chunk *snd_button;
		
	public:
		
		void buildBackground(int w, int h, const char *title);
		void loadButtonTiles();
		SDL_Surface *createButton(int w, const char *text, ButtonType type = bt_0);
		SDL_Surface *createScrollBar(int h);
		void hide();
		
		inline int button_y(int option)
			{ return (option == 0) ? (background->h - 8 - BUT_H) : (36 + 34*(option-1)); }
		
		Window(int x, int y, int w, int h, const char *title);
		virtual ~Window();
		virtual void draw()=0;
		virtual GameMenuOpt handleInput()=0;
	
};

class GameMenuWindow : public Window {
	private:
		GameMenuOpt option;
		const char **options;
		
	public:
		GameMenuWindow(int x, int y, int w, int h, const char *title, const char **options);
		~GameMenuWindow();
		
		void draw();
		GameMenuOpt handleInput();
		
};

const SDL_Rect sbar_rect = { 280, 58, 16, 176 };
const SDL_Rect scroll_rect = { 18, 43, 262, 200 };

class HelpWindow : public Window {
	private:
		int scroll_y;
		
		SDL_Surface *s_text;
		SDL_Surface scroll_dot;
		
	public:
		HelpWindow(int x, int y, int w, int h, const char *title, const char **lines);
		~HelpWindow();
		
		void draw();
		GameMenuOpt handleInput();
};


#endif
