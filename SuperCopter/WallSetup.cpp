#include "WallSetup.h"

#include <random>
#include <Windows.h>
#include "Global.h"


WallSetup::WallSetup() {
	 cave1.upCave.resize(SCREEN_WIDTH);
	 cave2.upCave.resize(SCREEN_WIDTH);
	 cave3.upCave.resize(SCREEN_WIDTH);
	 cave1.downCave.resize(SCREEN_WIDTH);
	 cave2.downCave.resize(SCREEN_WIDTH);
	 cave3.downCave.resize(SCREEN_WIDTH);
	 ptrUpCave = &cave1.upCave;
	 ptrUpCave2 = &cave2.upCave;
	 ptrUpCave3 = &cave3.upCave;
	 ptrDownCave = &cave1.downCave;
	 ptrDownCave2 = &cave2.downCave;
	 ptrDownCave3 = &cave3.downCave;
	 ptrStock;
}

void WallSetup::buildWalls() {
	ptrStock = ptrUpCave;
	ptrUpCave = ptrUpCave2;
	ptrUpCave2 = ptrUpCave3;
	ptrUpCave3 = ptrStock;
	mBfUp_r(*ptrUpCave3, (*ptrUpCave2)[SCREEN_WIDTH - 1], 0, cave1.upCave.size() - 1, standardDeviation, generator, standardDeviationVariation, true);
	movingAverage(*ptrUpCave3);

	ptrStock = ptrDownCave;
	ptrDownCave = ptrDownCave2;
	ptrDownCave2 = ptrDownCave3;
	ptrDownCave3 = ptrStock;
	mBfDown_r(*ptrDownCave3, *ptrUpCave, (*ptrDownCave)[SCREEN_WIDTH - 1], 0, cave1.upCave.size() - 1, 5, standardDeviation, generator, standardDeviationVariation, true);
	movingAverage(*ptrDownCave3);

}

/*
====================
init

Initialize the first and last index of vectors passed on parameter with a random number
====================
*/
void WallSetup::init() {
	cave1.upCave[0] = rand_a_b(1, 4);
	cave1.upCave[SCREEN_WIDTH - 1] = rand_a_b(1, 4);
	cave2.upCave[0] = cave1.upCave[SCREEN_WIDTH - 1];
	cave2.upCave[SCREEN_WIDTH - 1] = rand_a_b(1, 4);
	cave3.upCave[0] = cave2.upCave[SCREEN_WIDTH - 1];
	cave3.upCave[SCREEN_WIDTH - 1] = rand_a_b(1, 4);
	cave1.upCave[0] = cave3.upCave[SCREEN_WIDTH - 1];

	cave1.downCave[0] = rand_a_b(21, 24);
	cave1.downCave[SCREEN_WIDTH - 1] = rand_a_b(21, 24);
	cave2.downCave[0] = cave1.downCave[SCREEN_WIDTH - 1];
	cave2.downCave[SCREEN_WIDTH - 1] = rand_a_b(21, 24);
	cave3.downCave[0] = cave2.downCave[SCREEN_WIDTH - 1];
	cave3.downCave[SCREEN_WIDTH - 1] = rand_a_b(21, 24);
	cave1.downCave[0] = cave3.downCave[SCREEN_WIDTH - 1];

	mBfUp_r(*ptrUpCave, 0, 0, cave1.upCave.size() - 1, standardDeviation, generator, standardDeviationVariation, false);
	movingAverage(*ptrUpCave);
	mBfUp_r(*ptrUpCave2, 0, 0, cave1.upCave.size() - 1, standardDeviation, generator, standardDeviationVariation, false);
	movingAverage(*ptrUpCave2);
	mBfUp_r(*ptrUpCave3, 0, 0, cave1.upCave.size() - 1, standardDeviation, generator, standardDeviationVariation, false);
	movingAverage(*ptrUpCave3);

	mBfDown_r(*ptrDownCave, *ptrUpCave, 0, 0, cave1.upCave.size() - 1, 5, standardDeviation, generator, standardDeviationVariation, false);
	movingAverage(*ptrDownCave);
	mBfDown_r(*ptrDownCave2, *ptrUpCave2, 0, 0, cave1.upCave.size() - 1, 5, standardDeviation, generator, standardDeviationVariation, false);
	movingAverage(*ptrDownCave2);
	mBfDown_r(*ptrDownCave3, *ptrUpCave3, 0, 0, cave1.upCave.size() - 1, 5, standardDeviation, generator, standardDeviationVariation, false);
	movingAverage(*ptrDownCave3);
}

int WallSetup::rand_a_b(int a, int b) {
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
void WallSetup::mBfUp_r(vector<int>& up, int firstCase, int lowerBound, int upperBound, double stanDev, std::default_random_engine& gen, double stanDevVariation, bool first) {
	normal_distribution<double> distrib(0.0, stanDev);

	//Fill the first index with the last index of the previous array and the last one with a random number
	if (first) {
		up[0] = firstCase;
		up[SCREEN_WIDTH - 1] += (int)distrib(gen);
		first = false;
		if (up[SCREEN_WIDTH - 1] <= 0) {
			up[SCREEN_WIDTH - 1] = 1;
		}
	}

	if (upperBound - lowerBound > 1) {
		int mid = (upperBound + lowerBound) / 2;

		up[mid] = (up[lowerBound] + up[upperBound]) / 2;
		up[mid] += (int)distrib(gen);

		up[mid] = max(0, min(24, up[mid]));

		stanDev = max(0.02, stanDev * stanDevVariation);

		mBfUp_r(up, 0, lowerBound, mid, stanDev, gen, stanDevVariation, false);
		mBfUp_r(up, 0, mid, upperBound, stanDev, gen, stanDevVariation, false);
	}
}

/*
====================
mBfDown_r

Same function as the previous one, but for down side vectors
It needs the upper side vector as parameter to avoid collision between arrays of each side
====================
*/
void WallSetup::mBfDown_r(vector<int>& down, const vector<int>& up, int firstCase, int lowerBound, int upperBound, int val, double stanDev, std::default_random_engine& gen, double stanDevVariation, bool first) {
	normal_distribution<double> distrib(0.0, stanDev);

	if (first) {
		down[0] = firstCase;
		down[SCREEN_WIDTH - 1] += (int)distrib(gen);
		if (down[SCREEN_WIDTH - 1] - up[SCREEN_WIDTH - 1] < val) {
			down[SCREEN_WIDTH - 1] = up[SCREEN_WIDTH - 1] + val;
		}
		if (down[SCREEN_WIDTH - 1] >= 25) {
			down[SCREEN_WIDTH - 1] = 24;
		}
		first = false;
	}

	if (upperBound - lowerBound > 1) {
		int mid = (upperBound + lowerBound) / 2;

		down[mid] = (down[lowerBound] + down[upperBound]) / 2;
		down[mid] += (int)distrib(gen);

		down[mid] = min(24, max(0, down[mid]));
		if (down[mid] - up[mid] < val) {
			down[mid] = up[mid] + val;
		}

		stanDev = max(0.02, stanDev * stanDevVariation);

		mBfDown_r(down, up, 0, lowerBound, mid, val, stanDev, gen, stanDevVariation, false);
		mBfDown_r(down, up, 0, mid, upperBound, val, stanDev, gen, stanDevVariation, false);
	}
}

/*
====================
movingAverage

Make the moving average on the vector passed on parameter
====================
*/
void WallSetup::movingAverage(vector<int>& array) {
	vector<int> averageArray(array.size());
	averageArray[0] = array[0];
	averageArray[array.size() - 1] = array[array.size() - 1];

	unsigned int i;
	int average;
	for (i = 1; i < array.size() - 1; ++i) {
		average = (array[i - 1] + array[i] + array[i + 1]) / 3;
		averageArray[i] = average;
	}
	for (i = 0; i < array.size(); ++i) {
		array[i] = averageArray[i];
	}
}