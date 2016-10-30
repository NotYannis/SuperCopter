#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <random>
#include <string>
#include "NYTimer.cpp"
#include "Ship.h"
#include "Cave.h"
#include "WallSetup.h"
#include "FrameBuffer.h"
#include "Menu.h"
#include "Score.h"

/*
TODO :
Better menus (string place and titles and color and all)
GetButtonDown functions for menus
Reset the game to restart it
Highscore tab (gooooood luck)
*/


using namespace std;

State state;
bool playing;
NYTimer timer;

Ship ship(12);

FrameBuffer frameBuf;
CHAR_INFO (&buf)[SCREEN_HEIGHT][SCREEN_WIDTH] = frameBuf.getBuffer();
WallSetup walls;
Menu menu;
Score score;
/*
====================
writeScore

Write the score on the upper right corner of the screen
====================
*/
void writeScore(int cpt) {
	string score = to_string(cpt);
	int stringSize = score.size();

	for (int x = SCREEN_WIDTH - stringSize; x < SCREEN_WIDTH; ++x) {
		buf[0][SCREEN_WIDTH - stringSize - 1].Char.AsciiChar = ' ';
		buf[0][SCREEN_WIDTH - stringSize - 1].Attributes = 0;
		buf[0][x].Char.AsciiChar = score.at(stringSize - (SCREEN_WIDTH - x));
		buf[0][x].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}
}

/*
====================
draw

Fill the buffer with the informations calculated by WallSetup fonctions and ship position
Set char colors
====================
*/
void draw(int cpt){
	int tabIndex;
	
	ReadConsoleOutput(frameBuf.hOutput, (*buf), frameBuf.dwBufferSize,
		frameBuf.dwBufferCoord, &frameBuf.rcRegion);

		frameBuf.clear();

	switch (state){
		case STATE_MENU :
				menu.drawMenu(buf);
			break;
		case STATE_LEVEL :
			//Draw the cave
			walls.drawWalls(buf, cpt);

			//Draw the ship
			ship.drawShip(buf);

			score.drawScore(buf);
				break;
		case STATE_SCORE :
			menu.drawScoreMenu(buf, score.score);
			break;
	}

	WriteConsoleOutput(frameBuf.hOutput, (*buf), frameBuf.dwBufferSize, frameBuf.dwBufferCoord, &frameBuf.rcRegion);
}

int main(int argc, char * argv[]){
	playing = true;
	state = (State)STATE_MENU;

	bool dead;

	int cpt;
	int difficultyLvl;
	default_random_engine gen;
	dead = false;

	cpt = 0;
	difficultyLvl = 20;

	walls.init();
	timer.start();
	srand(time(NULL));
	float framerate = 0.1f;

	while (playing){
		if (timer.getElapsedSeconds() > framerate){
			switch (state){
			case STATE_MENU:
				menu.getKey();
				if (GetAsyncKeyState(VK_SPACE)){
					state = menu.getState();
				}
				draw(cpt);
				break;
			case STATE_LEVEL:
				framerate = 0.01f;

					//Each time an array is passed, fill a new array
					if (cpt != 0 && cpt % SCREEN_WIDTH == 0){
						walls.buildWalls();
						++difficultyLvl;
					}
					//Increase difficulty every 5 new arrays, with a limit
					if (difficultyLvl < 70){
						if (difficultyLvl % 5 == 0){
							walls.standardDeviation += 0.1;
							++difficultyLvl;
						}
					}
					ship.fall();

					//Check collision with cave walls
					if (cpt % 80 >= 75){
						dead = ship.checkCollision((*walls.ptrUpCave2)[(cpt + ship.xPos) % SCREEN_WIDTH], (*walls.ptrDownCave2)[(cpt + ship.xPos) % SCREEN_WIDTH]);
					}
					else{
						dead = ship.checkCollision((*walls.ptrUpCave)[(cpt + ship.xPos) % SCREEN_WIDTH], (*walls.ptrDownCave)[(cpt + ship.xPos) % SCREEN_WIDTH]);
					}

					draw(cpt);
					++cpt;
					++score.score;

					if (dead){
						state = (State)STATE_SCORE;
						menu.selected = 0;
					}
				break;
			case STATE_SCORE:
				menu.getScoreKey();
				menu.drawScoreMenu(buf, score.score);
				if (GetAsyncKeyState(VK_SPACE)){
					state = menu.getState();
				}
				draw(cpt);
				break;
			case STATE_QUIT:
				playing = false;
				break;
			}
			timer.restart();
		}
	}
	return 0;
}
