#ifndef STARSYSTEM_H
#define STARSYSTEM_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
using namespace std;


class StarSystem {
   private:
	uint32_t nLehmer = 0;
	//uint32_t nLehmerOld = 0;
	uint32_t Lehmer32();

	inline int rndInt(int min, int max) { return (Lehmer32() % (max - min)) + min; }

	inline double rndDouble(double min, double max) {
		return ((double)Lehmer32() / (double)(0x7FFFFFFF)) * (max - min) + min;
	}

   public:
	bool starExists = false;
	double starRadius = 0.0;
	//double starCoord[3] = {0.0, 0.0, 0.0};
	double starOffset[3] = {0.0, 0.0, 0.0};
	float starColor[3] = {0.0, 0.0, 0.0};

	StarSystem(uint32_t i, uint32_t j, uint32_t k, double nSec, double secSize);
	//~StarSystem();
};

#endif	//STARSYSTEM_H