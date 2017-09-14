#include "test.hpp"
#include "../fraction/fraction.hpp"
#include "../matrix.hpp"
#include <sstream>

using namespace std;

namespace test {
	bool testMatrix01(string &testName, string &output) {
		testName = GET_TEST_NAME("test inversion of 3x3-matrix");
		bool result = true;

		Prime *prm = Prime::getInstance();
		prm->load("primes200.prm");

		Matrix<Fraction> M(
				2, Fraction(4,5), Fraction(1,3),
				3, Fraction(1,5), Fraction(7,3),
				1, Fraction(5,-2), 1);
		Fraction Mdet = M.getDeterminant();
		Matrix<Fraction> invM = M.getInversion();
		Fraction invMdet = invM.getDeterminant();
		Matrix<Fraction> MinvM = M * invM;
		if (MinvM != Matrix<Fraction>::Unit) {
			output = "F * F^{-1} != I";
			result = false;
		}
		Fraction One = invMdet*Mdet;
		if (One != (slong) 1) {
			output = "det(F) * det(F^{-1}) != 1";
			result = false;
		}
		return result;
	}
}
