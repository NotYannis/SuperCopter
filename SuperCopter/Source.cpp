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
const int xShip = 5;
int upLine1[SCREEN_WIDTH];
int upLine2[SCREEN_WIDTH];
int upLine3[SCREEN_WIDTH];
int downLine1[SCREEN_WIDTH];
int downLine2[SCREEN_WIDTH];
int downLine3[SCREEN_WIDTH];
NYTimer timer;

Ship ship(12);

void draw(int cpt, const vector<int>* upLine, const vector<int>* upLine2, const vector<int>* downLine, const vector<int>* downLine2, float standDev, bool hard, int maxCaveSpace){
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];


	ReadConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);

	int x;
	int y;
	string test = to_string(standDev);
	int stringSize = test.size();

	for (x = 0; x < SCREEN_WIDTH; ++x){
		for (y = 0; y < SCREEN_HEIGHT; ++y){
			buffer[y][x].Char.AsciiChar = ' ';
		}
	}
	int i;
	int tabIndex = cpt % 80;
	char c = '0';
	for (x = 0; x < SCREEN_WIDTH - 1; ++x){
		if ((x + tabIndex) >= 80 && tabIndex != 0){
			for (i = (*upLine2)[(x + tabIndex) % 80]; i >= 0; --i){
				buffer[i][x].Char.AsciiChar = c;
			}
			for (i = (*downLine2)[(x + tabIndex) % 80]; i < SCREEN_HEIGHT; ++i){
				buffer[i][x].Char.AsciiChar = c;
			}

		}
		else{
			for (i = (*upLine)[x + tabIndex]; i >= 0; --i){
				buffer[i][x].Char.AsciiChar = c;
			}
			for (i = (*downLine)[x + tabIndex]; i < SCREEN_HEIGHT; ++i){
				buffer[i][x].Char.AsciiChar = c;
			}
		}
		if (hard){
			if ((x + tabIndex) >= 80 && tabIndex != 0){
				if ((*downLine2)[(x + tabIndex) % 80] - (*upLine2)[(x + tabIndex) % 80] > maxCaveSpace){
					for (i = (*upLine2)[(x + tabIndex) % 80] + maxCaveSpace; i < (*downLine2)[(x + tabIndex) % 80] - maxCaveSpace; ++i){
						buffer[i][x].Char.AsciiChar = c;
					}
				}
			}
			else{
				if ((*downLine)[x + tabIndex] - (*upLine)[x + tabIndex] > maxCaveSpace){
					for (i = (*upLine)[(x + tabIndex)] + maxCaveSpace; i < (*downLine)[(x + tabIndex)] - maxCaveSpace; ++i){
						buffer[i][x].Char.AsciiChar = c;
					}
				}
			}
		}
	}
	buffer[(int)ship.yPos][ship.xPos].Char.AsciiChar = ship.shape;
	for (x = 0; x < stringSize; ++x){
		buffer[0][x].Char.AsciiChar = test.at(x);
	}
	WriteConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);


}

int rand_a_b(int a, int b){
	return rand() % (b - a) + a;
}

void mBfUp(vector<int>& up, int firstCase, int lowerBound, int upperBound, double stanDev, default_random_engine& gen, double stanDevVariation, bool first){
	normal_distribution<double> distrib(0.0, stanDev);
	
	if (first){
		up[0] = firstCase;
		up[SCREEN_WIDTH - 1] += (int)distrib(gen);
		first = false;
	}

	if ( upperBound - lowerBound > 1 ){
		int mid = (upperBound + lowerBound) / 2;

		up[mid] = (up[lowerBound] + up[upperBound]) / 2;
		up[mid] += (int)distrib(gen);
		
		up[mid] = max(0, min(24, up[mid]));

		stanDev = max(0.02, stanDev * stanDevVariation);
		
		mBfUp(up, 0, lowerBound, mid, stanDev, gen, stanDevVariation, false);
		mBfUp(up, 0, mid, upperBound, stanDev, gen, stanDevVariation, false);
	}
}

void mBfDown(vector<int>& down, const vector<int>& up, int firstCase, int lowerBound, int upperBound, int val, double stanDev, default_random_engine& gen, double stanDevVariation, bool first){
	normal_distribution<double> distrib(0.0, stanDev);

	if (first){
		down[0] = firstCase;
		down[SCREEN_WIDTH - 1] += (int)distrib(gen);
		first = false;
	}

	if (upperBound - lowerBound > 1){
		int mid = (upperBound + lowerBound) / 2;

		down[mid] = (down[lowerBound] + down[upperBound]) / 2;
		down[mid] += (int)distrib(gen);

		down[mid] = min(24, max(0, down[mid]));
		if (down[mid] - up[mid] < val){
			down[mid] = up[mid] + val;
		}

		stanDev = max(0.02, stanDev * stanDevVariation);

		mBfDown(down, up, 0, lowerBound, mid, val, stanDev, gen, stanDevVariation, false);
		mBfDown(down, up, 0, mid, upperBound, val, stanDev, gen, stanDevVariation, false);
	}
}

void init(vector<int>& upL1, vector<int>& upL2, vector<int>& upL3, vector<int>& downL1, vector<int>& downL2, vector<int>& downL3){
	upL1[0] = rand_a_b(1, 7);
	upL1[SCREEN_WIDTH - 1] = rand_a_b(1, 7);
	upL2[0] = upL1[SCREEN_WIDTH - 1];
	upL2[SCREEN_WIDTH - 1] = rand_a_b(1, 7);
	upL3[0] = upL2[SCREEN_WIDTH - 1];
	upL3[SCREEN_WIDTH - 1] = rand_a_b(1, 7);
	upL1[0] = upL3[SCREEN_WIDTH - 1];
	downL1[0] = rand_a_b(17, 24);
	downL1[SCREEN_WIDTH - 1] = rand_a_b(18, 24);
	downL2[0] = downL1[SCREEN_WIDTH - 1];
	downL2[SCREEN_WIDTH - 1] = rand_a_b(18, 24);
	downL3[0] = downL2[SCREEN_WIDTH - 1];
	downL3[SCREEN_WIDTH - 1] = rand_a_b(18, 24);
	downL1[0] = downL3[SCREEN_WIDTH - 1];
}

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
	mBfUp(*ptrU, 0, 0, upLine.size() - 1, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrU);
	mBfUp(*ptrU2, 0, 0, upLine.size() - 1, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrU2);
	mBfUp(*ptrU3, 0, 0,upLine.size() - 1, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrU3);

	mBfDown(*ptrD, *ptrU, 0, 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrD);
	mBfDown(*ptrD2, *ptrU2, 0, 0,upLine.size() - 1, 5, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrD2);
	mBfDown(*ptrD3, *ptrU3, 0, 0,upLine.size() - 1, 5, stanDev, gen, stanDevVariation, false);
	movingAverage(*ptrD3);
	

	timer.start();
	while (!dead){
		if (timer.getElapsedSeconds() > 0.01f){
			if (cpt != 0 && cpt % 80 == 0){
				ptrStock = ptrU;
				ptrU = ptrU2;
				ptrU2 = ptrU3;
				ptrU3 = ptrStock;
				mBfUp(*ptrU3, (*ptrU2)[SCREEN_WIDTH - 1], 0, upLine.size() - 1, stanDev, gen, stanDevVariation, true);
				movingAverage(*ptrU3);
				ptrStock = ptrD;
				ptrD = ptrD2;
				ptrD2 = ptrD3;
				ptrD3 = ptrStock;
				mBfDown(*ptrD3, *ptrU3, (*ptrD2)[SCREEN_WIDTH - 1], 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation, true);
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
			}/*
			ship.fall();
			if ((int)ship.yPos <= (*ptrU)[(cpt + ship.xPos) % SCREEN_WIDTH] || (int)ship.yPos >= (*ptrD)[(cpt + ship.xPos) % SCREEN_WIDTH]){
				dead = true;
			}//*/
			draw(cpt, ptrU, ptrU2, ptrD, ptrD2, stanDev, hardcoreMode, maxCaveSpace);
			++cpt;
			
			timer.restart();

		}
	}
	return 0;
}
