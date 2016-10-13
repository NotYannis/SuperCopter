#include "ship.h"

	Ship::Ship(float y)
	:	xPos(5),
		fallingVelocity(0.0f),
		fallingAcceleration(0.0f),
		shape('>'),
		maxFallingV(0.2f) {
			yPos = y;
	}

	void Ship::fall(){
		if (GetAsyncKeyState(VK_UP)){
			fallingVelocity -= 0.01f;
		}
		else
			fallingVelocity += 0.01f;
		
		if (fallingVelocity < -maxFallingV)
			fallingVelocity = -maxFallingV;
		else if (fallingVelocity > maxFallingV)
			fallingVelocity = maxFallingV;
		yPos += fallingVelocity;
	}
