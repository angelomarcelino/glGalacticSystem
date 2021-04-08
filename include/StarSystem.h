#ifndef STARSYSTEM_H
#define STARSYSTEM_H

#include <iostream>
using namespace std;

class StarSystem {
   private:
	uint32_t nLehmer = 0;

	uint32_t Lehmer32();

	inline int rndInt(int min, int max) { return (Lehmer32() % (max - min)) + min; }

	inline double rndDouble(double min, double max) {
		return ((double)Lehmer32() / (double)(0x7FFFFFFF)) * (max - min) + min;
	}

   public:
	bool starExists = false;
	double starRadius = 0.0;
	double starCoord[3] = {0.0, 0.0, 0.0};
	double starOffset[3] = {0.0, 0.0, 0.0};

	StarSystem(uint32_t x, uint32_t y, uint32_t z, double nSec, double secSize);
};

#endif	//STARSYSTEM_H