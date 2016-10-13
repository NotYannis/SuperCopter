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
#include "Global.h"

using namespace std;

NYTimer timer;

Ship ship(12);
const int A = 25;
const int B = 80;

CHAR_INFO buffer[A][B];
int tail;

WallSetup walls;

/*
====================
draw

Fill the buffer with the informations calculated by WallSetup fonctions and ship position
Set char colors
====================
*/
void draw(int cpt, const vector<int>* upLine, const vector<int>* upLine2, const vector<int>* downLine, const vector<int>* downLine2, bool hard, int maxCaveSpace){
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	CHAR_INFO limit;
	limit.Attributes = FOREGROUND_RED;
	limit.Char.AsciiChar = '0';
	CHAR_INFO fill;
	fill.Attributes = FOREGROUND_RED | BACKGROUND_GREEN | BACKGROUND_RED;
	fill.Char.AsciiChar = '²';


	ReadConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);

	int x;
	int y;
	string score = to_string(cpt);
	int stringSize = score.size();

	//Fill the buffer with spaces to erase the previous frame
	for ( x = 0; x < SCREEN_WIDTH; ++x ){
		for ( y = 0; y < SCREEN_HEIGHT; ++y ){
			buffer[y][x].Char.AsciiChar = ' ';
			buffer[y][x].Attributes = 0;
		}
	}

	int i;
	int tabIndex = cpt % SCREEN_WIDTH;
	for ( x = 0; x < SCREEN_WIDTH - 1; ++x ){
		if ( ( x + tabIndex ) >= SCREEN_WIDTH && tabIndex != 0 ){
			for ( i = (*upLine2)[( x + tabIndex ) % SCREEN_WIDTH]; i >= 0; --i ){
				if (i == (*upLine2)[(x + tabIndex) % SCREEN_WIDTH])
					buffer[i][x] = limit ;
				else
					buffer[i][x] = fill;
			}
			for ( i = (*downLine2)[( x + tabIndex ) % SCREEN_WIDTH]; i < SCREEN_HEIGHT; ++i ){
				if (i == (*downLine2)[(x + tabIndex) % SCREEN_WIDTH])
					buffer[i][x] = limit;
				else
					buffer[i][x] = fill;
			}

		}
		else{
			for ( i = (*upLine)[x + tabIndex]; i >= 0; --i ){
				if (i == (*upLine)[x + tabIndex])
					buffer[i][x] = limit;
				else
					buffer[i][x] = fill;
			}
			for ( i = (*downLine)[ x + tabIndex ]; i < SCREEN_HEIGHT; ++i ){
				if (i == (*downLine)[x + tabIndex])
					buffer[i][x] = limit;
				else
					buffer[i][x] = fill;
			}
		}

		/*
		if (hard){
			if ( ( x + tabIndex ) >= SCREEN_WIDTH && tabIndex != 0 ){
				if ( (*downLine2)[( x + tabIndex ) % SCREEN_WIDTH] - (*upLine2)[( x + tabIndex ) % SCREEN_WIDTH] > maxCaveSpace ){
					for ( i = (*upLine2)[ ( x + tabIndex ) % SCREEN_WIDTH ] + maxCaveSpace; i < (*downLine2)[( x + tabIndex ) % SCREEN_WIDTH] - maxCaveSpace; ++i ){
						buffer[i][x].Char.AsciiChar = c;
					}
				}
			}
			else{
				if ( (*downLine)[x + tabIndex] - (*upLine)[x + tabIndex] > maxCaveSpace ){
					for ( i = (*upLine)[x + tabIndex] + maxCaveSpace; i < (*downLine)[x + tabIndex] - maxCaveSpace; ++i ){
						buffer[i][x].Char.AsciiChar = c;
					}
				}
			}
		}*/
	}

	buffer[(int)ship.yPos][ship.xPos].Char.AsciiChar = ship.shape;
	buffer[(int)ship.yPos][ship.xPos - 1].Char.AsciiChar = '-';
	buffer[(int)ship.yPos][ship.xPos - 2].Char.AsciiChar = '-';
	buffer[tail][ship.xPos - 3].Char.AsciiChar = '-';
	buffer[tail][ship.xPos - 4].Char.AsciiChar = '-';
	buffer[(int)ship.yPos][ship.xPos].Attributes = 7;
	buffer[(int)ship.yPos][ship.xPos - 1].Attributes = 7;
	buffer[(int)ship.yPos][ship.xPos - 2].Attributes = 7;
	buffer[tail][ship.xPos - 3].Attributes = 7;
	buffer[tail][ship.xPos - 4].Attributes = 7;
	if(ship.fallingVelocity > 0.2f) tail = (int)ship.yPos - 1;
	else if (ship.fallingVelocity < -0.2f) tail = (int)ship.yPos + 1;
	else tail = (int)ship.yPos;

	for (x = SCREEN_WIDTH - stringSize; x < SCREEN_WIDTH; ++x){
		buffer[0][SCREEN_WIDTH - stringSize - 1].Char.AsciiChar = ' ';
		buffer[0][SCREEN_WIDTH - stringSize - 1].Attributes = 0;
		buffer[0][x].Char.AsciiChar = score.at(stringSize - (SCREEN_WIDTH - x));
		buffer[0][x].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	}

	WriteConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}


int main(int argc, char * argv[]){
	bool dead = false;
	bool hardcoreMode = false;

	int cpt = 0;
	int difficultyLvl = 20;
	int maxCaveSpace = 15;
	default_random_engine gen;

	srand(time(NULL));
	walls.standardDeviationVariation = 1;
	walls.standardDeviation = 0.7;

	walls.init();

	timer.start();
	while (!dead){
		if (timer.getElapsedSeconds() > 0.01f){
			if (cpt != 0 && cpt % SCREEN_WIDTH == 0){
				walls.buildWalls();
				++difficultyLvl;
			}
			if (difficultyLvl < 70){
				if (difficultyLvl == 30){
					hardcoreMode = true;
				}
				if (difficultyLvl % 5 == 0){
					walls.standardDeviation += 0.1;
					++difficultyLvl;
					if (hardcoreMode){
						if (maxCaveSpace > 9){
							--maxCaveSpace;
						}
					}
				}
			}
			
			draw(cpt, walls.ptrUpCave, walls.ptrUpCave2, walls.ptrDownCave, walls.ptrDownCave2, hardcoreMode, maxCaveSpace);
			ship.fall();
			if (cpt % 80 >= 75){
				if ((int)ship.yPos <= (*walls.ptrUpCave2)[(cpt + ship.xPos) % SCREEN_WIDTH] || (int)ship.yPos >= (*walls.ptrDownCave2)[(cpt + ship.xPos) % SCREEN_WIDTH]){
					dead = true;
				}
			}
			else{
				if ((int)ship.yPos <= (*walls.ptrUpCave)[(cpt + ship.xPos) % SCREEN_WIDTH] || (int)ship.yPos >= (*walls.ptrDownCave)[(cpt + ship.xPos) % SCREEN_WIDTH]){
						dead = true;
					}
			}
			++cpt;
			
			timer.restart();

		}
	}
	return 0;
}
