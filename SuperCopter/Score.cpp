#include "Score.h"
#include "Global.cpp"
#include <iostream>
#include <string>

Score::Score(){
	score = 0;
}

void Score::drawScore(CHAR_INFO (&buffer)[25][80]){
	std::string scoreString = std::to_string(score);
	int stringSize = scoreString.size();

	for (int x = SCREEN_WIDTH - stringSize; x < SCREEN_WIDTH; ++x) {
		buffer[0][SCREEN_WIDTH - stringSize - 1].Char.AsciiChar = ' ';
		buffer[0][SCREEN_WIDTH - stringSize - 1].Attributes = 0;
		buffer[0][x].Char.AsciiChar = scoreString.at(stringSize - (SCREEN_WIDTH - x));
		buffer[0][x].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}

}