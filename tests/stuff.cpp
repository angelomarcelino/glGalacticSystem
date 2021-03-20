#include <iostream>

using namespace std;

uint32_t nLehmer = 0;
uint32_t Lehmer32() {
	nLehmer += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;

	return m2;
}

int rndInt(int min, int max) { return (Lehmer32() % (max - min)) + min; }

double rndDouble(double min, double max) {
	return ((double)Lehmer32() / (double)(0x7FFFFFFF)) * (max - min) + min;
}

int main() {
	int lim = (3 / 1);
	for (int x = 0; x <= lim; x++) 
		for (int y = 0; y <= lim; y++) 
			for (int z = 0; z <= lim; z++) {
				int nLehmer = (z & 0xFFFF) << 16 | (y & 0xFFFF) << 16 | (x & 0xFFFF);

				cout << nLehmer << endl;
			}

}