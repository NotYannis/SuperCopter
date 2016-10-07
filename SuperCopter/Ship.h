#ifndef __SHIP__
#define __SHIP__

#include <Windows.h>

class Ship{
	public:
		const int xPos;
		float yPos;
		float fallingVelocity;
		float fallingAcceleration;
		const char shape;
		const float maxFallingV;

		Ship(float y);
		void fall();
};

#endif