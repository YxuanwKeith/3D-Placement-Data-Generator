#pragma once

#include "Common/TypeDef.h"

using namespace std;

namespace WorkSpace {

class RandomGen {
	mt19937 pseudoRandNumGen;
public:

	void initial(int seed) {
		pseudoRandNumGen = mt19937(seed);
	}
	void initial(Str& seed) {
		if (!seed.empty()) {
			pseudoRandNumGen = mt19937(stoi(seed));
		}
	}
	
	int rand(const int& lowerBound, int& upperBound) {
		return uniform_int_distribution<int>(lowerBound, upperBound)(pseudoRandNumGen);
	}
	int rand(const int upperBound) {
		return uniform_int_distribution<int>(0, upperBound - 1)(pseudoRandNumGen);
	}

	RandomGen() : pseudoRandNumGen(sCast<mt19937>(time(nullptr) + clock())) {}
};

} //WorkSpace