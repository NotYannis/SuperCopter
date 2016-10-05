#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include "NYTimer.cpp"
#include <vector>
#include <ctime>
#include <random>
#include "Ship.cpp"

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

Ship ship(12, 20);

void draw(int cpt, const vector<int>* upLine, const vector<int>* upLine2, const vector<int>* downLine, const vector<int>* downLine2){
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];


	ReadConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);

	int x;
	int y;



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
	}
	//buffer[ship.yPos][ship.xPos].Char.AsciiChar = ship.shape;

	WriteConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);


}

int asciiPhysics(int cpt){
	if (!GetAsyncKeyState(VK_UP)){
		if (cpt % ship.fallingSpeed == 0){
			return ship.yPos + 1;
		} else {
			return ship.yPos;
		}
	}
	else{
		return ship.yPos - 1;
	}
}

int rand_a_b(int a, int b){
	return rand() % (b - a) + a;
}

void mBfUp(vector<int>& up, int lowerBound, int upperBound, double stanDev, default_random_engine& gen, double stanDevVariation){
	
	normal_distribution<double> distrib(0.0, stanDev);
	if ( upperBound - lowerBound > 1 ){
		int mid = (upperBound + lowerBound) / 2;

		up[mid] = (up[lowerBound] + up[upperBound]) / 2;
		up[mid] += (int)distrib(gen);
		
		up[mid] = max(0, min(24, up[mid]));

		stanDev = max(0.02, stanDev * stanDevVariation);
		
		mBfUp(up, lowerBound, mid, stanDev, gen, stanDevVariation);
		mBfUp(up, mid, upperBound, stanDev, gen, stanDevVariation);
	}
}

void mBfDown(vector<int>& down, const vector<int>& up, int lowerBound, int upperBound, int val, double stanDev, default_random_engine& gen, double stanDevVariation){

	normal_distribution<double> distrib(0.0, stanDev);
	if (upperBound - lowerBound > 1){
		int mid = (upperBound + lowerBound) / 2;

		down[mid] = (down[lowerBound] + down[upperBound]) / 2;
		down[mid] += (int)distrib(gen);

		down[mid] = min(25, max(0, down[mid]));
		if (down[mid] - up[mid] < val){
			down[mid] = up[mid] + val;
		}

		stanDev = max(0.02, stanDev * stanDevVariation);

		mBfDown(down, up, lowerBound, mid, val, stanDev, gen, stanDevVariation);
		mBfDown(down, up, mid, upperBound, val, stanDev, gen, stanDevVariation);
	}
}

void init(vector<int>& upL1, vector<int>& upL2, vector<int>& upL3, vector<int>& downL1, vector<int>& downL2, vector<int>& downL3){
	upL1[0] = rand_a_b(1, 7);
	upL1[SCREEN_WIDTH - 1] = rand_a_b(1, 7);
	upL2[0] = upL1[SCREEN_WIDTH - 1];
	upL2[SCREEN_WIDTH - 1] = rand_a_b(1, 7);
	upL3[0] = upL2[SCREEN_WIDTH - 1];
	upL3[SCREEN_WIDTH - 1] = rand_a_b(1, 7);
	downL1[0] = rand_a_b(17, 24);
	downL1[SCREEN_WIDTH - 1] = rand_a_b(17, 24);
	downL2[0] = downL1[SCREEN_WIDTH - 1];
	downL2[SCREEN_WIDTH - 1] = rand_a_b(17, 24);
	downL3[0] = downL2[SCREEN_WIDTH - 1];
	downL3[SCREEN_WIDTH - 1] = rand_a_b(17, 24);
}



int main(int argc, char * argv[]){
	bool dead = false;
	int cpt = 0;
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
	double stanDev = 1;


	init(*ptrU, *ptrU2, *ptrU3, *ptrD, *ptrD2, *ptrD3);
	mBfUp(*ptrU, 0, upLine.size() - 1, stanDev, gen, stanDevVariation);
	mBfUp(*ptrU2, 0, upLine.size() - 1, stanDev, gen, stanDevVariation);
	mBfUp(*ptrU3, 0, upLine.size() - 1, stanDev, gen, stanDevVariation);

	mBfDown(*ptrD, *ptrU, 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation);
	mBfDown(*ptrD2, *ptrU2, 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation);
	mBfDown(*ptrD3, *ptrU3, 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation);


	timer.start();
	while (!dead){
		if (timer.getElapsedSeconds() > 0.02f){
			if (cpt != 0 && cpt % 80 == 0){
				ptrStock = ptrU;
				ptrU = ptrU2;
				ptrU2 = ptrU3;
				ptrU3 = ptrStock;
				mBfUp(*ptrU3, 0, upLine.size() - 1, stanDev, gen, stanDevVariation);
				ptrStock = ptrD;
				ptrD = ptrD2;
				ptrD2 = ptrD3;
				ptrD3 = ptrStock;
				mBfDown(*ptrD3, *ptrU3, 0, upLine.size() - 1, 5, stanDev, gen, stanDevVariation);
			}

			draw(cpt, ptrU, ptrU2, ptrD, ptrD2);
			//ship.yPos = asciiPhysics(cpt);
			++cpt;
			if (ship.yPos == 5 || ship.yPos == 19){
				dead = true;
			}
			
			timer.restart();

			//stanDev += 0.03;

		}
	}
	return 0;
}
