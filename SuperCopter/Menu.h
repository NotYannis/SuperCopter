#ifndef __MENU__
#define __MENU__

#include <Windows.h>
#include "Global.h"

class Menu{
	public :
		int selected;
		
		Menu();
		void display();
		void getKey();
		void getScoreKey();
		void drawMenu(CHAR_INFO (&buffer)[SCREEN_HEIGHT][SCREEN_WIDTH]);
		void drawScoreMenu(CHAR_INFO (&buffer)[SCREEN_HEIGHT][SCREEN_WIDTH], int score);
		State getState();

	private :	
		CHAR_INFO buffer[25][80];
		int playY;
		int scoreY;
		int quitY;
};

#endif