#include <iostream>
#include <iomanip>

using namespace std;

uint32_t random_seed = 42;

uint32_t Lehmer32() {
    random_seed += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)random_seed * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;

	return m2;
}

double Lehmer() {
    return (double)Lehmer32();
}

double map_range(double x, double min, double max) {
    return x * (max - min) / 2 + min;
}

int main() {
    
    double min = 1.0, max = 2.0;
    
    cout << "\n\nMin: " << min << " Max: " << max << "\n\n";
    cout << setw(20) << "Lehmer32\t" << setw(20) << "Lehmer32/0x7FFFFFFF \t"  << setw(20) << "Min-Max Mapping\t" << setw(20) << "New Mapping\n";
    
    for (int i = 0; i < 20; i++) {
        double n = Lehmer();
        cout << setw(20) << n << "\t" 
            << setw(20) << n / (double)(0x7FFFFFFF) << "\t"
            << setw(20) << (n / (double)(0x7FFFFFFF)) * (max - min) + min << "\t" 
            << setw(20) << map_range((n / (double)(0x7FFFFFFF)), min, max) << endl;
    }

    return 0;
}
