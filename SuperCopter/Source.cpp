#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include "NYTimer.cpp"
#include <vector>
#include <ctime>
#include <random>
#include "Ship.h"
#include <string>

using namespace std;

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;
NYTimer timer;

Ship ship(12);

CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
int tail;

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

int rand_a_b(int a, int b){
	return rand() % (b - a) + a;
}

/*
====================
mBfUp_r

Fill the vector passed on parameter using the fractionnal brownian motion
The firstCase parameter is used to make a clean junction with the previous vector
This function is used for vectors on the upper side of the screen
====================
*/

void mBfUp_r(vector<int>& up, int firstCase, int lowerBound, int upperBound, double stanDev, default_random_engine& gen, double stanDevVariation, bool first){
	normal_distribution<double> distrib(0.0, stanDev);
	
	//Fill the first index with the last index of the previous array and the last one with a random number
	if (first){
		up[0] = firstCase;
		up[SCREEN_WIDTH - 1] += (int)distrib(gen);
		first = false;
		if (up[SCREEN_WIDTH - 1] <= 0){
			up[SCREEN_WIDTH - 1] = 1;
		}
	}

	if ( upperBound - lowerBound > 1 ){
		int mid = ( upperBound + lowerBound ) / 2;

		up[mid] = ( up[lowerBound] + up[upperBound] ) / 2;
		up[mid] += (int)distrib(gen);
		
		up[mid] = max(0, min(24, up[mid]));

		stanDev = max(0.02, stanDev * stanDevVariation);
		
		mBfUp_r(up, 0, lowerBound, mid, stanDev, gen, stanDevVariation, false);
		mBfUp_r(up, 0, mid, upperBound, stanDev, gen, stanDevVariation, false);
	}
}

/*
====================
mBf_r

Same function as the previous one, but for down side vectors
It needs the upper side vector as parameter to avoid collision between arrays of each side
====================
*/

void mBfDown_r(vector<int>& down, const vector<int>& up, int firstCase, int lowerBound, int upperBound, int val, double stanDev, default_random_engine& gen, double stanDevVariation, bool first){
	normal_distribution<double> distrib(0.0, stanDev);

	if (first){
		down[0] = firstCase;
		down[SCREEN_WIDTH - 1] += (int)distrib(gen);
		if (down[SCREEN_WIDTH - 1] - up[SCREEN_WIDTH - 1] < val){
			down[SCREEN_WIDTH - 1] = up[SCREEN_WIDTH - 1] + val;
		}
		if (down[SCREEN_WIDTH - 1] >= 25){
			down[SCREEN_WIDTH - 1] = 24;
		}
		first = false;
	}

	if (upperBound - lowerBound > 1){
		int mid = ( upperBound + lowerBound ) / 2;

		down[mid] = ( down[lowerBound] + down[upperBound] ) / 2;
		down[mid] += (int)distrib(gen);

		down[mid] = min(24, max(0, down[mid]));
		if (down[mid] - up[mid] < val){
			down[mid] = up[mid] + val;
		}

		stanDev = max(0.02, stanDev * stanDevVariation);

		mBfDown_r(down, up, 0, lowerBound, mid, val, stanDev, gen, stanDevVariation, false);
		mBfDown_r(down, up, 0, mid, upperBound, val, stanDev, gen, stanDevVariation, false);
	}
}

/*
====================
init

Initialize the first and last index of vectors passed on parameter with a random number
====================
*/

void init(vector<int>& upL1, vector<int>& upL2, vector<int>& upL3, vector<int>& downL1, vector<int>& downL2, vector<int>& downL3){
	upL1[0] = rand_a_b(1, 4);
	upL1[SCREEN_WIDTH - 1] = rand_a_b(1, 4);
	upL2[0] = upL1[SCREEN_WIDTH - 1];
	upL2[SCREEN_WIDTH - 1] = rand_a_b(1, 4);
	upL3[0] = upL2[SCREEN_WIDTH - 1];
	upL3[SCREEN_WIDTH - 1] = rand_a_b(1, 4);
	upL1[0] = upL3[SCREEN_WIDTH - 1];

	downL1[0] = rand_a_b(21, 24);
	downL1[SCREEN_WIDTH - 1] = rand_a_b(21, 24);
	downL2[0] = downL1[SCREEN_WIDTH - 1];
	downL2[SCREEN_WIDTH - 1] = rand_a_b(21, 24);
	downL3[0] = downL2[SCREEN_WIDTH - 1];
	downL3[SCREEN_WIDTH - 1] = rand_a_b(21, 24);
	downL1[0] = downL3[SCREEN_WIDTH - 1];
}

/*
====================
movingAverage

Make the moving average on the vector passed on parameter
====================
*/

void movingAverage(vector<int>& array){
	vector<int> averageArray(array.size());
averageArray[0] = array[0];
averageArray[array.size() - 1] = array[array.size() - 1];

int i;
int average;
for (i = 1; i < array.size() - 1; ++i){
	average = (array[i - 1] + array[i] + array[i + 1]) / 3;
	averageArray[i] = average;
}
for (i = 0; i < array.size(); ++i){
	array[i] = averageArray[i];
}
}

int main(int argc, char * argv[]){
	bool dead = false;
	bool hardcoreMode = false;

	int cpt = 0;
	int difficultyLvl = 20;
	int maxCaveSpace = 15;
	default_random_engine gen;
	vector<int> upLine(SCREEN_WIDTH);
	vector<int> upLine2(SCREEN_WIDTH);
	vector<int> upLine3(SCREEN_WIDTH);
	vector<int> downLine(SCREEN_WIDTH);
	vector<int> downLine2(SCREEN_WIDTH);
	vector<int> downLine3(SCREEN_WIDTH);
	vector<int>* ptrU = &upLine;
	vector<int>* ptrU2 = &upLine2;
	vector<int>* ptrU3 = &upLine3;
	vector<int>* ptrD = &downLine;
	vector<int>* ptrD2 = &downLine2;
	vector<int>* ptrD3 = &downLine3;
	vector<int>* ptrStock;

	srand(time(NULL));
	double stanDevVariation = 1;
	double stanDev = 0.7;


	init(*ptrU, *ptrU2, *ptrU3, *ptrD, *ptrD2, *ptrD3);
	mBfUp_r(*ptrU, 0, 0, upLine.size() - 1, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrU);
	mBfUp_r(*ptrU2, 0, 0, upLine.size() - 1, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrU2);
	mBfUp_r(*ptrU3, 0, 0, upLine.size() - 1, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrU3);

	mBfDown_r(*ptrD, *ptrU, 0, 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrD);
	mBfDown_r(*ptrD2, *ptrU2, 0, 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrD2);
	mBfDown_r(*ptrD3, *ptrU3, 0, 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrD3);


	timer.start();
	while (!dead){
		if (timer.getElapsedSeconds() > 0.01f){
			if (cpt != 0 && cpt % SCREEN_WIDTH == 0){
				ptrStock = ptrU;
				ptrU = ptrU2;
				ptrU2 = ptrU3;
				ptrU3 = ptrStock;
				mBfUp_r(*ptrU3, (*ptrU2)[SCREEN_WIDTH - 1], 0, upLine.size() - 1, stanDev, gen, stanDevVariation, true);
				movingAverage(*ptrU3);

				ptrStock = ptrD;
				ptrD = ptrD2;
				ptrD2 = ptrD3;
				ptrD3 = ptrStock;
				mBfDown_r(*ptrD3, *ptrU3, (*ptrD2)[SCREEN_WIDTH - 1], 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation, true);
				movingAverage(*ptrD3);
				++difficultyLvl;
			}
			if (difficultyLvl < 70){
				if (difficultyLvl == 30){
					hardcoreMode = true;
				}
				if (difficultyLvl % 5 == 0){
					stanDev += 0.1;
					++difficultyLvl;
					if (hardcoreMode){
						if (maxCaveSpace > 9){
							--maxCaveSpace;
						}
					}
				}
			}
			
			draw(cpt, ptrU, ptrU2, ptrD, ptrD2, hardcoreMode, maxCaveSpace);
			ship.fall();
			if (cpt % 80 >= 75){
				if ((int)ship.yPos <= (*ptrU2)[(cpt + ship.xPos) % SCREEN_WIDTH] || (int)ship.yPos >= (*ptrD2)[(cpt + ship.xPos) % SCREEN_WIDTH]){
					dead = true;
				}
			}
			else{
				if ((int)ship.yPos <= (*ptrU)[(cpt + ship.xPos) % SCREEN_WIDTH] || (int)ship.yPos >= (*ptrD)[(cpt + ship.xPos) % SCREEN_WIDTH]){
						dead = true;
					}
			}
			++cpt;
			
			timer.restart();

		}
	}
	return 0;
}
