#ifndef __SHIP__
#define __SHIP__

class Ship{
public:
	const int xPos = 5;
	int yPos;
	int fallingSpeed;
	const char shape = '>';

	Ship(int y, int fall){
		yPos = y;
		fallingSpeed = fall;
	}
};

#endif