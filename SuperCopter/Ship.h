#ifndef __SHIP__
#define __SHIP__

#include <Windows.h>

class Ship{
	public:
		const int xPos;
		float yPos;
		const char shape;

		Ship(float y);
		void fall();
		bool checkCollision(int minY, int maxY);

	private : 
		float fallingVelocity;
		float fallingAcceleration;
		const float maxFallingV;
};

#endif