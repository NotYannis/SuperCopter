#ifndef __WALL_SETUP__
#define __WALL_SETUP__

#include <vector>
#include <random>
#include "Global.h"
#include "Cave.h"

using namespace std;

class WallSetup {
	public :
		Cave cave1;
		Cave cave2;
		Cave cave3;

		double standardDeviation;
		double standardDeviationVariation;

		WallSetup();
		void buildWalls();
		void init();

		vector<int>* ptrUpCave;
		vector<int>* ptrUpCave2;
		vector<int>* ptrUpCave3;
		vector<int>* ptrDownCave;
		vector<int>* ptrDownCave2;
		vector<int>* ptrDownCave3;
		vector<int>* ptrStock;
	
	private :

		default_random_engine generator;


		void mBfUp_r(vector<int>& up, int firstCase, int lowerBound, int upperBound, double stanDev, default_random_engine& gen, double stanDevVariation, bool first);
		void mBfDown_r(vector<int>& down, const vector<int>& up, int firstCase, int lowerBound, int upperBound, int val, double stanDev, default_random_engine& gen, double stanDevVariation, bool first);
		void movingAverage(vector<int>& array);
		int rand_a_b(int a, int b);

};


#endif
