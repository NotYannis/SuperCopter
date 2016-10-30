#ifndef __SCORE__
#define __SCORE__

#include <Windows.h>
#include "State.cpp"

class Score{
	public :
		int score;

		Score();
		void drawScore(CHAR_INFO (&buffer)[25][80]);
		void drawScoreMenu(CHAR_INFO (&buffer)[25][80]);
		State getState();
};

#endif