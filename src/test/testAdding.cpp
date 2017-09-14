#include "test.hpp"
#include "../fraction/fraction.hpp"

using namespace std;

namespace test {


	bool testAddition01(string &testName, string &output) {
		testName = GET_TEST_NAME("basic adding test");
		int val = -10;
		Fraction F(-10, 11);

		for (int i=0; i<5; i++) {
			ulong num = F.getNumeratorAsUlong();
			ulong correctNum = (ulong) (val<0? -val: val);
			if (num != correctNum) {
				output = "failed with operator++";
				return false;
			}

			F++;
			val += 11;
		}

		for (int i=0; i<10; i++) {
			ulong num = F.getNumeratorAsUlong();
			ulong correctNum = (ulong) (val<0? -val: val);
			if (num != correctNum) {
				output = "failed with operator--";
				return false;
			}
			F--;
			val -= 11;
		}
		return true;
	}

	bool testAddition02(string &testName, string &output) {
		testName = GET_TEST_NAME("for loop with integers");
		Fraction f((slong) -10);
		for (int i=0; i<20; i++)
			f++;

		if (f.getNumeratorAsUlong() != (ulong) 10) {
			output = "Fraction(-10) + 20 != 10";
			return false;
		}

		return true;
	}

	bool testAddition03(string &testName, string &output) {
		testName = GET_TEST_NAME("test multiple transformations");
		Fraction frac1 = (slong) 5;
		frac1 ^= -1;
		frac1 *= 2;
		Fraction frac2 = -frac1;
		++frac2;
		Fraction fracResult = frac1 + frac2;
		if (fracResult != Fraction((slong)1)) {
			output = "2(5^-1) + (1 - 2(5^-1)) = (2/5 + 3/5) != 1";

			cout << "frac1 = 2/5 = " << frac1 << endl;
			cout << "frac2 = 3/5 = " << frac2 << endl;
			cout << "frac1 + frac2 = " << fracResult << endl;
			return false;
		}
		return true;
	}

	bool testAddition04(string &testName, string &output) {
		testName = GET_TEST_NAME("test subtraction with large numbers");
		slong a = 5*7*7*2*2;
		slong b = -19*5*7*2*2; // ... / 3
		//slong c = 2*2*5*7*(3*7-19)/3;	// a+b
		Fraction A(a);
		Fraction B(b);
		B /= 3;
		Fraction C = A+B;
		//cout << A << " + " << B << " = " << C << " = " << c << endl;
		if (C != Fraction((slong) 2*2*2*5*7, (slong) 3)) {
			output = "2*2*5*7*(3*7-19)/3 != 93.3333... = 280/3";
			return false;
		}
		return true;
	}
}
