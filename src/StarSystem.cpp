#include "../include/StarSystem.h"

#include <bitset>
#include <functional>
#include <iostream>
using namespace std;

uint32_t StarSystem::Lehmer32() {
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;

	return m2;
}

StarSystem::StarSystem(uint32_t i, uint32_t j, uint32_t k, double nSec, double secSize) {
	//nLehmer = (k & 0xFFFF) << 16 | (j & 0xFFFF) << 8 | (i & 0xFFFF);
	nLehmer = hash<uint32_t>{}(i + (j * 23) + (k * 135));
	iniLehmer = nLehmer;
	starExists = (rndInt(0, (int)(nSec * 10)) == 1);
	if (!starExists) return;

	starRadius = rndDouble(0.03, secSize / 5);

	for (int i = 0; i < 3; i++) {
		int sig = (rndInt(1, 2) == 1) ? 1 : -1;
		starOffset[i] = sig * rndDouble(0.01, secSize / 8);
	}

	int kelvin = rndInt(3, 15);
	if (kelvin < 6) {  // RED - ORANGE
		starColor.x = 1.0;
		starColor.y = rndInt(0, 150) / 255.0;
		starColor.z = 0.0;
	} else if (kelvin >= 6 && kelvin < 9) {  // ORANGE - YELLOW
		starColor.x = 1.0;
		starColor.y = rndInt(200, 255) / 255.0;
		starColor.z = 0.0;
	} else if (kelvin >= 9 && kelvin < 12) {	// YELLOW - WHITE
		starColor.x = 1.0;
		starColor.y = rndInt(150, 255) / 255.0;
		starColor.z = rndInt(0, 255) / 255.0;
	} else if (kelvin >= 12 && kelvin <= 15) {	 // WHITE - BLUE
		starColor.x = 0.0;
		starColor.y = rndInt(100, 200) / 255.0;
		starColor.z = 1.0;
	}
	//cout << ' ' << kelvin << ' ' << starColor[0] << ' ' << starColor[1] << ' ' << starColor[2] << endl;

	// Cor da estrela
	// Numero de planetas (talvez criar classe de planetas)
	// dados do planeta - tamanho - distancia - numero de luas - rotacao - translacao - cor do planeta
	// dados das luas - tamanho - distancia - rotacao - tranlacao - cor da lua
}

StarSystem::StarSystem(uint32_t lehmer, double nSec, double secSize) {
	nLehmer = lehmer;
	iniLehmer = nLehmer;

	starExists = (rndInt(0, (int)(nSec * 10)) == 1);
	if (!starExists) return;

	starRadius = rndDouble(0.03, secSize / 5);

	for (int i = 0; i < 3; i++) {
		int sig = (rndInt(1, 2) == 1) ? 1 : -1;
		starOffset[i] = sig * rndDouble(0.01, secSize / 8);
	}

	int kelvin = rndInt(3, 15);
	if (kelvin < 6) {  // RED - ORANGE
		starColor.x = 1.0;
		starColor.y = rndInt(0, 150) / 255.0;
		starColor.z = 0.0;
	} else if (kelvin >= 6 && kelvin < 9) {	 // ORANGE - YELLOW
		starColor.x = 1.0;
		starColor.y = rndInt(200, 255) / 255.0;
		starColor.z = 0.0;
	} else if (kelvin >= 9 && kelvin < 12) {  // YELLOW - WHITE
		starColor.x = 1.0;
		starColor.y = rndInt(150, 255) / 255.0;
		starColor.z = rndInt(0, 255) / 255.0;
	} else if (kelvin >= 12 && kelvin <= 15) {	// WHITE - BLUE
		starColor.x = 0.0;
		starColor.y = rndInt(100, 200) / 255.0;
		starColor.z = 1.0;
	}
}