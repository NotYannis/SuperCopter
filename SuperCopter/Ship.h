#ifndef __SHIP__
#define __SHIP__

#include <Windows.h>

class Ship{
	public:
		const int xPos = 5;
		float yPos;
		float fallingVelocity = 0.0f;
		float fallingAcceleration = 0.0f;
		const char shape = '>';
		const float maxFallingV = 0.2f;

		Ship(float y);
		void fall();
};

#endif