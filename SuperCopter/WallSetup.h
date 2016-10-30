#ifndef __WALL_SETUP__
#define __WALL_SETUP__

#include <Windows.h>
#include <vector>
#include <random>
#include "Cave.h"
#include "Global.h"

using namespace std;

class WallSetup {
	public :
		//Three upper and lower walls : two for display, one for buffering
		Cave cave1;
		Cave cave2;
		Cave cave3;

		double standardDeviation;

		//Pointers on vectors and one stock to make	walls turn over
		vector<int>* ptrUpCave;
		vector<int>* ptrUpCave2;
		vector<int>* ptrUpCave3;
		vector<int>* ptrDownCave;
		vector<int>* ptrDownCave2;
		vector<int>* ptrDownCave3;
		vector<int>* ptrStock;

		//Aspect of the walls
		CHAR_INFO limit;
		CHAR_INFO fill;

		WallSetup();
		void buildWalls();
		void drawWalls(CHAR_INFO (&buffer)[SCREEN_HEIGHT][SCREEN_WIDTH], int cpt);
		void init();
	
	private :
		default_random_engine generator;

		void mBfUp_r(vector<int>& up, int firstCase, int lowerBound, int upperBound, double stanDev, default_random_engine& gen, bool first);
		void mBfDown_r(vector<int>& down, const vector<int>& up, int firstCase, int lowerBound, int upperBound, int val, double stanDev, default_random_engine& gen, bool first);
		void movingAverage(vector<int>& array);
		int rand_a_b(int a, int b);

};


#endif
