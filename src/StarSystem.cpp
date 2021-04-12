#include "../include/StarSystem.h"

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
	nLehmer = (k & 0xFFFF) << 16 | (j & 0xFFFF) << 8 | (i & 0xFFFF);

	starExists = (rndInt(0, 200) == 1);
	if (!starExists) return;

	starRadius = rndDouble(0.03, secSize / 5);

	for (int i = 0; i < 3; i++) {
		int sig = (rndInt(1, 2) == 1) ? 1 : -1;
		starOffset[i] = sig * rndDouble(0.01, secSize / 8);
	}

	starCoord[0] = ((i - nSec / 2) * secSize) + starOffset[0];
	starCoord[1] = ((j - nSec / 2) * secSize) + starOffset[1];
	starCoord[2] = ((k - nSec / 2) * secSize) + starOffset[2];

    // Cor da estrela
    // Numero de planetas (talvez criar classe de planetas)
    // dados do planeta - tamanho - distancia - numero de luas - rotacao - translacao - cor do planeta
    // dados das luas - tamanho - distancia - rotacao - tranlacao - cor da lua
}