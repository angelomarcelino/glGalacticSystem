#include <iostream>

using namespace std;

int main() {
	int lim = (3 / 0.3)/2;
	for (int x = -lim; x <= lim; x++) {
		for (int y = -lim; y <= lim; y++) {
			for (int z = -lim; z <= lim; z++) {
				cout << x * 0.3 << " " << y * 0.3 << " " << z * 0.3 << endl;
			}
		}
	}
}