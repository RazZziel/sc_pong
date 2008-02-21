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
#include <math.h>


#include "audio.h"
#include "CBall.h"
#include "CVector.h"
#include "CPhysics.h"


Physics::Physics() {

}

Physics::~Physics() {

}

void Physics::calculateImpulse(Vector p1, Vector *v1, float m1, Vector p2, Vector *v2, float m2) {
	
	//printf("%f.%f %f.%f\n", v1->x, v1->y, v2->x, v2->y);
	
	Vector n = (p1 - p2);
	n.normalize();
	Vector vr = (*v1 - *v2);
	
	float vrn = vr * n;
	
	float fCr = 1.0;
	float vj = (-(1.0+fCr) * vrn) / ((n*n) * (1.0/m1 + 1.0/m2));
	
	*v1 += (n*vj) / m1;
	*v2 -= (n*vj) / m2;
	
	//printf("%f.%f %f.%f\n", v1->x, v1->y, v2->x, v2->y);
	
}

void Physics::checkBallCollisions(std::vector<Ball*> *balls) {
	/* Collisions between balls */
	
	for (Uint8 i=0; i<balls->size(); i++) {
		for (Uint8 j=i+1; j<balls->size(); j++) {
			
			if (distance(balls->at(i)->p, balls->at(i)->r, balls->at(j)->p, balls->at(j)->r) <= 0.0) {
				
				calculateImpulse( balls->at(i)->p, &balls->at(i)->v, balls->at(i)->m,
						  balls->at(j)->p, &balls->at(j)->v, balls->at(j)->m );
				
				balls->at(i)->p += balls->at(i)->v;
				balls->at(j)->p += balls->at(j)->v;
				
				balls->at(i)->way.x = (int) (balls->at(i)->v.x/fabs(balls->at(i)->v.x));
				balls->at(j)->way.y = (int) (balls->at(j)->v.y/fabs(balls->at(j)->v.y));
				
			}
			
		}
	}
	
}

void Physics::checkPlayerCollisions(std::vector<Ball*> *balls, Player **players, int n_players) {
	/* Collisions between players and balls */
	
	for (Uint8 i=0; i<n_players; i++) {
		for (Uint8 j=0; j<balls->size(); j++) {
			
			// Player with ball - horizontal
			if ((balls->at(j)->p.y >= players[i]->p1.y) && // Vertical - Top
			    (balls->at(j)->p.y <= players[i]->p2.y) && // Vertical - Bottom
			    (fabs(players[i]->p1.x - balls->at(j)->p.x) <= (players[i]->r + balls->at(j)->r))) { // Horizontal
				
				balls->at(j)->v.x = -balls->at(j)->v.x;
				balls->at(j)->way.x = -balls->at(j)->way.x;
				
				int vy = (players[i]->v.y == 0.0) ? 0 : ((int) (players[i]->v.y/fabs(players[i]->v.y)));
				
				if (players[i]->combo == CMB_NULL) {
					//balls->at(j)->setEffect(CMB_ZERG_1, vy, players[i]);
					//balls->at(j)->v.y += BALL_Y_ACCEL*vy;
				} else {
					balls->at(j)->setEffect(players[i]->combo, vy, players[i]);
					players[i]->combo = CMB_NULL;
				}
				
				playSound(players[i]->s_hit);
				
			// Player with ball - vertical
			} else if ((balls->at(j)->p.y < players[i]->p1.y) && // Vertcial - Over top
				   (distance( balls->at(j)->p, balls->at(j)->r,
					      players[i]->p1, players[i]->r ) <= 0.0f)) { // Distance
				
				calculateImpulse( balls->at(j)->p, &balls->at(j)->v, balls->at(j)->m,
						  players[i]->p1, &players[i]->v, players[i]->m );
				balls->at(j)->p += balls->at(j)->v;
				balls->at(j)->setEffect(CMB_NULL, 0);
				
			// Player with ball - vertical
			} else if ((balls->at(j)->p.y > players[i]->p2.y) && // Vertcial - Under bottom
				   (distance( balls->at(j)->p, balls->at(j)->r,
					      players[i]->p2, players[i]->r ) <= 0.0f)) { // Distance
				
				calculateImpulse( balls->at(j)->p, &balls->at(j)->v, balls->at(j)->m,
						  players[i]->p2, &players[i]->v, players[i]->m );
				balls->at(j)->p += balls->at(j)->v;
				
			}
			
			
		}
	}
	
}
