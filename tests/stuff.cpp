#include <iostream>
#include <bitset>

using namespace std;

/*
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
	for (int x = 0; x <= lim; x++) {
		for (int y = 0; y <= lim; y++) {
			for (int z = 0; z <= lim; z++) {
				int nLehmer = (z & 0xFFFF) << 16 | ((y & 0xFFFF) | (x & 0xFFFF));

				cout << "(" << nLehmer << ", " << (rndInt(0, 5) == 1) << ")" << " ";
			}
			cout << "y" << " ";
		}
		cout << endl;
	}

	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++){
				int nLehmer = (z & 0xFFFF) << 20 | (y & 0xFFFF) << 10 | (x & 0xFFFF);
				bitset<32> az(z), bz((z & 0xFFFF) << 20), ay(y), by((y & 0xFFFF) << 10), ax(x), bx(x & 0xFFFF), c(nLehmer);

				//cout << az << " | " << ay << " | " << ax << endl << bz << " | " << by << " | " << bx << " ---> " << c << endl;
				cout << nLehmer << endl;
            }
			cout << "y" << endl;
		}
		cout << endl;
	}
} */
double secSize = 0.3;
double secLim = 3;
int nSecX = (secLim / secSize);
int nSecY = (secLim / secSize);
int nSecZ = (secLim / secSize);

double toCoord(int i, int nSec) {
	return (i - nSec/2) * secSize;
}

int main() {
	
	for (int i = 0; i < nSecX; i++)
		for (int j = 0; j < nSecY; j++)
			for (int k = 0; k < nSecZ; k++) {
				cout << toCoord(i, nSecX) << '\t' << toCoord(j, nSecY) << '\t' << toCoord(k, nSecZ * 2) << endl;
			}
}