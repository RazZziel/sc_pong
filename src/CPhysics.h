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



#ifndef __CPHYSICS_H__
#define __CPHYSICS_H__



#include <math.h>
#include <vector>


#include "CPlayer.h"
#include "CBall.h"

enum {
	BALL_Y_ACCEL = 3
};


class Physics {
	
	private:
		
		
		
	public:
		Physics();
		~Physics();
		
		void checkBallCollisions(std::vector<Ball*> *balls);
		void checkPlayerCollisions(std::vector<Ball*> *balls, Player **players, int n_players);
		void calculateImpulse(Vector p1, Vector *v1, float m1, Vector p2, Vector *v2, float m2);
		
		inline float distance(Vector p1, float r1, Vector p2, float r2)
			{ return (p1 - p2).module() - (r1 + r2); }
	
};


#endif
