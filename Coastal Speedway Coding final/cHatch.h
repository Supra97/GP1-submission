/*
=================
cHatch.h
- Header file for class definition - SPECIFICATION
- Header file for the Hatch class which is a child of cSprite class
=================
*/
#ifndef _CHATCH_H
#define _CHATCH_H
#include "cSprite.h"

class cHatch : public cSprite
{
private:
	SDL_Point hatchVelocity;

public:
	cHatch();
	void update(double deltaTime);		// Hatch update method
	void sethatchVelocity(SDL_Point hatchVel);   // Sets the velocity for the Hatch
	SDL_Point gethatchVelocity();				 // Gets the hatch velocity
};
#endif