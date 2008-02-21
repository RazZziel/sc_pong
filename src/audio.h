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



#ifndef __AUDIO_H__
#define __AUDIO_H__


#include "SDL/SDL_mixer.h"// TODO audio wrapper

#include "error.h"


Mix_Music *loadMusic(const char *filename);
Mix_Chunk *loadSound(const char *filename);


inline void playMusic(Mix_Music *track) { if (Mix_PlayMusic(track, -1 ) == -1) die(ERR_PMUSIC); }
inline void playSound(Mix_Chunk *chunk) { if (Mix_PlayChannel(-1, chunk, 0 ) == -1);}// die(ERR_PCHUNK); }FIXME


inline int setMusicVolume(int volume) { return Mix_VolumeMusic(volume); }
inline int setSoundVolume(int volume) { return Mix_Volume(-1, volume); }


void fadeToBlack();


#endif
