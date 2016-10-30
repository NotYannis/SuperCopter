#include "ship.h"

	Ship::Ship(float y)
	:	xPos(5),
		fallingVelocity(0.0f),
		fallingAcceleration(0.0f),
		shape('>'),
		maxFallingV(0.2f) {
			yPos = y;
	}

	void Ship::drawShip(CHAR_INFO (&buffer)[SCREEN_HEIGHT][SCREEN_WIDTH]){
		buffer[(int)yPos][xPos].Char.AsciiChar = shape;
		buffer[(int)yPos][xPos].Attributes = 7;
	}

	/*
	====================
	fall

	Make the ship fall every frame when the up key is not pressed
	====================
	*/
	void Ship::fall(){
		if (GetAsyncKeyState(VK_UP)){
			fallingVelocity -= 0.01f;
		}
		else
			fallingVelocity += 0.01f;
		
		//Check max velocity
		if (fallingVelocity < -maxFallingV)
			fallingVelocity = -maxFallingV;
		else if (fallingVelocity > maxFallingV)
			fallingVelocity = maxFallingV;
		//Add velocity to y position
		yPos += fallingVelocity;
	}

	bool Ship::checkCollision(int minY, int maxY) {
		return yPos <= minY || yPos >= maxY;
	}
	