#include "ship.h"

	Ship::Ship(float y){
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
