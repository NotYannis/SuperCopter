#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>

using namespace std;

const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;
const int xShip = 5;


void draw(int yShip, int cpt){
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
	if (cpt % 2 == 0){
		for (x = 0; x < SCREEN_WIDTH; ++x){
			for (y = 0; y < 6; ++y){
				if (x % 2 == 0){
					buffer[y][x].Char.AsciiChar = 'I';
				}
				else {
					buffer[y][x].Char.AsciiChar = 'O';
				}
			}
			for (y = 19; y < SCREEN_HEIGHT; ++y){
				if (x % 2 == 0){
					buffer[y][x].Char.AsciiChar = 'I';
				}
				else {
					buffer[y][x].Char.AsciiChar = 'O';
				}
			}
		}
	} else { 
		for (x = 0; x < SCREEN_WIDTH; ++x){
			for (y = 0; y < 6; ++y){
				if (x % 2 == 0){
					buffer[y][x].Char.AsciiChar = 'O';
				}
				else {
					buffer[y][x].Char.AsciiChar = 'I';
				}
			}
			for (y = 19; y < SCREEN_HEIGHT; ++y){
				if (x % 2 == 0){
					buffer[y][x].Char.AsciiChar = 'O';
				}
				else {
					buffer[y][x].Char.AsciiChar = 'I';
				}
			}
		}
	}

	buffer[yShip][xShip].Char.AsciiChar = '>';

	WriteConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);


}

int asciiPhysics(int yPos, int cpt){
	if (!GetAsyncKeyState(VK_UP)){
		if (cpt % 3 == 0){
			return yPos + 1;
		} else {
			return yPos;
		}
	}
	else{
		return yPos - 1;
	}
}


int main(int argc, char * argv[]){
	bool dead = false;
	int cpt = 0;
	int yPos = 8;
	while (!dead){
		draw(yPos, cpt);
		yPos = asciiPhysics(yPos, cpt);
		++cpt;
		Sleep(100);
		if (yPos == 6 || yPos == 19){
			dead = true;
		}
	}
	return 0;
}
