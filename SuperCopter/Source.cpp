#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include "NYTimer.cpp"
#include <vector>
#include <ctime>
#include <random>
#include "Ship.h"
#include "Cave.h"
#include <string>
#include "WallSetup.h"
#include "FrameBuffer.h"

using namespace std;

NYTimer timer;

Ship ship(12);

FrameBuffer frameBuf;
CHAR_INFO (&buf)[SCREEN_HEIGHT][SCREEN_WIDTH] = frameBuf.getBuffer();
WallSetup walls;

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
void draw(int cpt, const vector<int>* upLine, const vector<int>* upLine2, const vector<int>* downLine, const vector<int>* downLine2, int maxCaveSpace){
	ReadConsoleOutput(frameBuf.hOutput, (*buf), frameBuf.dwBufferSize,
		frameBuf.dwBufferCoord, &frameBuf.rcRegion);

	int x;
	int i;

	frameBuf.clear();

	//Draw the cave
	int tabIndex = cpt % SCREEN_WIDTH;
	for ( x = 0; x < SCREEN_WIDTH - 1; ++x ){
		if ( ( x + tabIndex ) >= SCREEN_WIDTH && tabIndex != 0 ){
			for ( i = (*upLine2)[( x + tabIndex ) % SCREEN_WIDTH]; i >= 0; --i ){
				if (i == (*upLine2)[(x + tabIndex) % SCREEN_WIDTH])
					buf[i][x] = walls.limit ;
				else
					buf[i][x] = walls.fill;
			}
			for ( i = (*downLine2)[( x + tabIndex ) % SCREEN_WIDTH]; i < SCREEN_HEIGHT; ++i ){
				if (i == (*downLine2)[(x + tabIndex) % SCREEN_WIDTH])
					buf[i][x] = walls.limit;
				else
					buf[i][x] = walls.fill;
			}
		}
		else{
			for ( i = (*upLine)[x + tabIndex]; i >= 0; --i ){
				if (i == (*upLine)[x + tabIndex])
					buf[i][x] = walls.limit;
				else
					buf[i][x] = walls.fill;
			}
			for ( i = (*downLine)[ x + tabIndex ]; i < SCREEN_HEIGHT; ++i ){
				if (i == (*downLine)[x + tabIndex])
					buf[i][x] = walls.limit;
				else
					buf[i][x] = walls.fill;
			}
		}


	}

	//Draw the ship
	buf[(int)ship.yPos][ship.xPos].Char.AsciiChar = ship.shape;
	buf[(int)ship.yPos][ship.xPos].Attributes = 7;

	writeScore(cpt);

	WriteConsoleOutput(frameBuf.hOutput, (*buf), frameBuf.dwBufferSize, frameBuf.dwBufferCoord, &frameBuf.rcRegion);
}

int main(int argc, char * argv[]){
	bool dead = false;

	int cpt = 0;
	int difficultyLvl = 20;
	int maxCaveSpace = 15;
	default_random_engine gen;

	srand(time(NULL));

	walls.init();

	timer.start();
	while (!dead){
		if (timer.getElapsedSeconds() > 0.01f){
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
			
			draw(cpt, walls.ptrUpCave, walls.ptrUpCave2, walls.ptrDownCave, walls.ptrDownCave2, maxCaveSpace);
			
			ship.fall();

			//Check collision with cave walls
			if (cpt % 80 >= 75){
				dead = ship.checkCollision((*walls.ptrUpCave2)[(cpt + ship.xPos) % SCREEN_WIDTH], (*walls.ptrDownCave2)[(cpt + ship.xPos) % SCREEN_WIDTH]);
			}
			else{
				dead = ship.checkCollision((*walls.ptrUpCave)[(cpt + ship.xPos) % SCREEN_WIDTH], (*walls.ptrDownCave)[(cpt + ship.xPos) % SCREEN_WIDTH]);
			}

			++cpt;
			
			timer.restart();
		}
	}
	return 0;
}
