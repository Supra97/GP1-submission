/*
=================
cNissan.h
- Header file for class definition - SPECIFICATION
- Header file for the Nissan class which is a child of cSprite class
=================
*/
#ifndef _CNISSAN_H
#define _CNISSAN_H
#include "cSprite.h"

class cNissan : public cSprite
{
private:
	SDL_Point nissanVelocity;

public:
	cNissan();
	void update(double deltaTime);		// Nissan update method
	void setNissanVelocity(SDL_Point nissanVel);   // Sets the velocity for the Nissan
	SDL_Point getNissanVelocity();				 // Gets the nissan velocity
};
#endif