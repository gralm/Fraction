#include "test/test.hpp"
#include "fraction/fraction.hpp"
#include <iostream>

using namespace std;

void runAllTests() {
	try {
		test::runOne();
	}catch(FractionError &e) {
		cout << "error = " << e << endl;
	}
}

void basicExample() {
	Prime *prm = Prime::getInstance();
	prm->setHighestIndex(100);
	cout << "100th prime = " << (*prm)[99] << endl;
		// Fraction can factorize the first 100 primes


	Fraction f = 5;
	f /= 7;
	f *= 3;
	cout << "f = 5/7*3 = " << f << endl;
	slong expon = -2;	// slong is a long long int
	Fraction g = f^(expon);
	cout << "g = f^-2 = " << g << endl;
	Fraction oneFourth = 0.25;
	Fraction h = g * oneFourth * Fraction(9,7);
	cout << "h = g * (1/4) * (9/7) = " << h << endl;
	Fraction i = --h;
	i *= 50;
	cout << "i = 50(h-1) = " << i << endl;
	slong mySignedLong = i;
	cout << "(signed long long int) i = " << mySignedLong << endl;
	cout << "(double) i = " << ((double)i) << endl;

}

int main() {
	basicExample();
	return 0;
}


