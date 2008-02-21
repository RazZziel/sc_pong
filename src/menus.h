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



#ifndef __MENUS_H__
#define __MENUS_H__


#include "main.h"
#include "CGame.h"
#include "CGraphics.h"

#define M_MAIN_MENU 			"data/sound/music/title.ogg"
#define P_BG_LOADING			"data/pixmap/menu/loading.png"

#define P_BOK				"data/pixmap/misc/pOk.png"
#define P_BCANCEL			"data/pixmap/misc/pCancel.png"


/* Main */

#define P_BG_MAIN_MENU			"data/pixmap/menu/main/background.png"

#define P_ANI_MAIN_SP			"data/pixmap/menu/main/single.png"
#define P_ANI_MAIN_SPON			"data/pixmap/menu/main/singleon.png"

#define P_ANI_MAIN_MP			"data/pixmap/menu/main/multi.png"
#define P_ANI_MAIN_MPON			"data/pixmap/menu/main/multion.png"
#define P_ANI_MAIN_MPTAIL		"data/pixmap/menu/main/etail.png"

#define P_ANI_MAIN_OPT			"data/pixmap/menu/main/editor.png"
#define P_ANI_MAIN_OPTON		"data/pixmap/menu/main/editoron.png"

#define P_ANI_MAIN_EXIT			"data/pixmap/menu/main/exit.png"
#define P_ANI_MAIN_EXITON		"data/pixmap/menu/main/exiton.png"

typedef enum {
	MAIN_SP,
	MAIN_MP,
	MAIN_OPT,
	MAIN_EXIT,
	MAIN_CRED
} MainMenuOpt;

const dot pos[]			= { {150, 111}, {78, 190}, {375, 104}, {490, 320}, {40, 430} };
const dot pos_ani[]		= { {0, 0},     {78, 190}, {375, 104}, {395, 328} };
const dot pos_ani_on[]		= { {46, 66},   {97, 201}, {396, 120}, {410, 320} };


/* Races */

#define P_BG_RACE_MENU			"data/pixmap/menu/races/background.png"
#define P_ANI_TERRAN			"data/pixmap/menu/races/terr.png"
#define P_ANI_PROTOSS			"data/pixmap/menu/races/prot.png"
#define P_ANI_ZERG			"data/pixmap/menu/races/zerg.png"
#define P_ANI_TERRANON			"data/pixmap/menu/races/terron.png"
#define P_ANI_PROTOSSON			"data/pixmap/menu/races/proton.png"
#define P_ANI_ZERGON			"data/pixmap/menu/races/zergon.png"
#define P_ANI_DISK			"data/pixmap/menu/races/disk.png"
#define P_WDG_ARML			"data/pixmap/menu/races/pbotl.png"
#define P_WDG_ARMR			"data/pixmap/menu/races/pbotr.png"
#define S_MOO				"data/sound/menu/moo.wav"			// http://www.wavplanet.com
#define S_MOO2				"data/sound/menu/moo2.wav"			// http://www.wavplanet.com
#define S_MOO3				"data/sound/menu/moo3.wav"			// http://www.wavplanet.com

typedef enum {
	R_ZERG,
	R_TERRAN,
	R_PROTOSS,
	R_MOO,
	R_NOTHING,
	R_CANCEL,
	R_EXIT
} RaceSelection;

inline void operator++(RaceSelection& opt, int)
	{ if ((opt = RaceSelection(opt+1)) > R_PROTOSS) opt = R_ZERG; }
inline void operator--(RaceSelection& opt, int)
	{ if ((opt = RaceSelection(opt-1)) < R_ZERG) opt = R_PROTOSS; }

enum {
	SELECTED_ALPHA		= SDL_ALPHA_OPAQUE,
	UNSELECTED_ALPHA	= SDL_ALPHA_OPAQUE/4,
	ALPHA_STEP		= 64,
	ALPHA_FLAGS		= SDL_SRCALPHA
};

inline Uint8 addAlpha(Uint8 alpha)
	{ return (alpha < SELECTED_ALPHA) ? (alpha + ALPHA_STEP) : alpha; }
inline Uint8 subAlpha(Uint8 alpha)
	{ return (alpha > UNSELECTED_ALPHA) ? (alpha - ALPHA_STEP) : alpha; }
	

/* Options */

#define P_BG_OPTIONS			"data/pixmap/menu/options/background.png"
#define P_BAR_FULL			"data/pixmap/menu/options/b_full.png"
#define P_BAR_EMPTY			"data/pixmap/menu/options/b_empt.png"

typedef enum {
	GOPT_RETURN,
	GOPT_OPT,
	GOPT_HELP,
	GOPT_EXIT,
	GOPT_NONE
} GameMenuOpt;

inline void operator++(GameMenuOpt& opt, int)
	{ if ((opt = GameMenuOpt(opt+1)) > GOPT_EXIT) opt = GOPT_RETURN; }
inline void operator--(GameMenuOpt& opt, int)
	{ if ((opt = GameMenuOpt(opt-1)) < GOPT_RETURN) opt = GOPT_EXIT; }


typedef enum{
	OPT_NAME,
	OPT_VOLM,
	OPT_VOLS,
	OPT_FS,
	OPT_KEYB,
	
	OPT_OK,
	OPT_EXIT
} MenuOpt;

inline void operator++(MenuOpt& opt, int)
	{ if ((opt = MenuOpt(opt+1)) > OPT_KEYB) opt = OPT_NAME; }
inline void operator--(MenuOpt& opt, int)
	{ if ((opt = MenuOpt(opt-1)) < OPT_NAME) opt = OPT_KEYB; }


enum {
	SOUND_BAR_STEPS		= 34,			// How many volume measure units has the volume bar graphic
	LEFT_COL		= (SCREEN_WIDTH/10),	// Cols example:
	RIGHT_COL		= SCREEN_WIDTH/2,	//  [LEFT_COL]	[RIGHT_COL]	[RIGHT_COL2]
	RIGHT_COL2		= RIGHT_COL*5/4,	//  Fullcreen	ON			OFF
	NAME_MAX_LENGTH		= 25,
	VOL_OFFSET		= 5,
	VOL_STEP		= 6
};

// Option screen position and volume calculations
inline int POS(int x) { return (SCREEN_HEIGHT*2/3)*(x+1)/5; }
inline int VOL(int x) { return VOL_OFFSET + VOL_STEP * x; }


// Multiplayer

#define P_BG_MULTIPLAYER_MENU		"data/pixmap/menu/multiplayer/background.png"


// Credits

#define P_BG_CRED_01			"data/pixmap/menu/credits/credits01.png"
#define P_BG_CRED_02			"data/pixmap/menu/credits/credits02.png"
#define P_BG_CRED_03			"data/pixmap/menu/credits/credits03.png"
#define P_BG_CRED_04			"data/pixmap/menu/credits/credits04.png"
#define P_BG_CRED_05			"data/pixmap/menu/credits/credits05.png"
#define P_BG_CRED_06			"data/pixmap/menu/credits/credits06.png"
#define P_BG_CRED_07			"data/pixmap/menu/credits/credits07.png"
#define P_BG_CRED_08			"data/pixmap/menu/credits/credits08.png"
#define P_BG_CRED_09			"data/pixmap/menu/credits/credits09.png"
#define P_BG_CRED_10			"data/pixmap/menu/credits/credits10.png"
#define M_CREDITS			"data/sound/menu/terran2.ogg"


// Sounds

#define S_SWISHIN			"data/sound/menu/swishin.wav"
#define S_SWISHOUT			"data/sound/menu/swishout.wav"
#define S_SWISHLOCK			"data/sound/menu/swishlock.wav"
#define S_MOUSEOVER			"data/sound/menu/mouseover.wav"
#define S_MOUSEDOWN2			"data/sound/menu/mousedown2.wav"
#define S_BUTTON			"data/sound/menu/button.wav"


enum {
	MENU_FPS = 15
};

void menu();
bool game_menu(Game *game);


#endif
