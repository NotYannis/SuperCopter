#include "Menu.h"
#include <iostream>
#include <string>
#include <Windows.h>

Menu::Menu(){
	selected = 0;
	playY = 9;
	scoreY = 12;
	quitY = 15;
}

void Menu::getKey(){
	if (GetAsyncKeyState(VK_UP)){
		switch (selected){
			case 0: selected = 2;
				break;
			case 1: selected = 0;
				break;
			case 2: selected = 1;
				break;
		}
	}
	if (GetAsyncKeyState(VK_DOWN)){
		switch (selected){
		case 0: selected = 1;
			break;
		case 1: selected = 2;
			break;
		case 2: selected = 0;
			break;
		}
	}
}

void Menu::getScoreKey(){
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_DOWN)){
		if (selected == 0){
			selected = 2;
		}
		else{
			selected = 0;
		}
	}
}

void Menu::drawMenu(CHAR_INFO (&buffer)[SCREEN_HEIGHT][SCREEN_WIDTH]){
	buffer[playY][38].Char.AsciiChar = 'P';
	buffer[playY][39].Char.AsciiChar = 'L';
	buffer[playY][40].Char.AsciiChar = 'A';
	buffer[playY][41].Char.AsciiChar = 'Y';

	buffer[scoreY][38].Char.AsciiChar = 'S';
	buffer[scoreY][39].Char.AsciiChar = 'C';
	buffer[scoreY][40].Char.AsciiChar = 'O';
	buffer[scoreY][41].Char.AsciiChar = 'R';
	buffer[scoreY][42].Char.AsciiChar = 'E';

	buffer[quitY][38].Char.AsciiChar = 'Q';
	buffer[quitY][39].Char.AsciiChar = 'U';
	buffer[quitY][40].Char.AsciiChar = 'I';
	buffer[quitY][41].Char.AsciiChar = 'T';

	switch (selected % 3){
		case 0:	for (int i = 38; i <= 41; ++i){
					buffer[playY][i].Attributes = FOREGROUND_GREEN;
					buffer[quitY][i].Attributes = 7;
				}
				for (int i = 38; i <= 42; ++i){
					buffer[scoreY][i].Attributes = 7;
				}
			break;
		case 1:	for (int i = 38; i <= 42; ++i){
					buffer[scoreY][i].Attributes = FOREGROUND_GREEN;
				}
				for (int i = 38; i <= 41; ++i){
					buffer[playY][i].Attributes = 7;
					buffer[quitY][i].Attributes = 7;
				}
			break;
		case 2:	for (int i = 38; i <= 41; ++i){
					buffer[quitY][i].Attributes = FOREGROUND_GREEN;
					buffer[playY][i].Attributes = 7;
				}
				for (int i = 38; i <= 42; ++i){
					buffer[scoreY][i].Attributes = 7;
				}
			break;
	}
}

void Menu::drawScoreMenu(CHAR_INFO (&buffer)[SCREEN_HEIGHT][SCREEN_WIDTH], int score){
	std::string scoreString = std::to_string(score);
	int stringSize = scoreString.size();

	buffer[13][28].Char.AsciiChar = 'V';
	buffer[13][29].Char.AsciiChar = 'O';
	buffer[13][30].Char.AsciiChar = 'T';
	buffer[13][31].Char.AsciiChar = 'R';
	buffer[13][32].Char.AsciiChar = 'E';

	buffer[13][34].Char.AsciiChar = 'S';
	buffer[13][35].Char.AsciiChar = 'C';
	buffer[13][36].Char.AsciiChar = 'O';
	buffer[13][37].Char.AsciiChar = 'R';
	buffer[13][38].Char.AsciiChar = 'E';

	buffer[13][40].Char.AsciiChar = ':';
	for (int x = 28; x <= 40; ++x){
		buffer[13][x].Attributes = 7;
	}



	for (int x = 39; x < 39 + stringSize; ++x){
		buffer[14][x].Char.AsciiChar = scoreString.at((x - 39));
		buffer[14][x].Attributes = FOREGROUND_RED;
	}

	buffer[20][39].Char.AsciiChar = 'P';
	buffer[20][40].Char.AsciiChar = 'L';
	buffer[20][41].Char.AsciiChar = 'A';
	buffer[20][42].Char.AsciiChar = 'Y';

	buffer[23][39].Char.AsciiChar = 'Q';
	buffer[23][40].Char.AsciiChar = 'U';
	buffer[23][41].Char.AsciiChar = 'I';
	buffer[23][42].Char.AsciiChar = 'T';

	if (selected == 0){
		for (int i = 39; i <= 42; ++i){
			buffer[23][i].Attributes = 7;
			buffer[20][i].Attributes = FOREGROUND_GREEN;
		}
	}
	else{
		for (int i = 39; i <= 42; ++i){
			buffer[20][i].Attributes = 7;
			buffer[23][i].Attributes = FOREGROUND_GREEN;
		}
	}
}

State Menu::getState(){
	switch (selected % 3){
		case 0 : return (State)STATE_LEVEL;
			break;
		case 1: return (State)STATE_SCORE;
			break;
		case 2: return (State)STATE_QUIT;
	}
}