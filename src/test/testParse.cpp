#include "test.hpp"
#include "../fraction/fraction.hpp"

using namespace std;

namespace test {


	bool testParse01(string &testName, string &output) {
		testName = GET_TEST_NAME("parse to double to Fraction");
		double v = 37./32;
		Fraction A(v);
		Fraction B((slong) 37, (slong) 32);
		bool result = (A==B);
		return result;
	}
}
