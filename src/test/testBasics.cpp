#include "test.hpp"
#include "../fraction/fraction.hpp"

using namespace std;


namespace test {



	bool testBasics01(string &testName, string &output) {
		testName = GET_TEST_NAME("create fraction with overPrimes");
		Fraction F(23*5, 29*4);
		ulong num = F.getNumeratorAsUlong();
		ulong den = F.getDenominatorAsUlong();

		if (num != 23*5 || den != 29*4) {
			output = "23*5 != 23*5 or 29*4 != 29*4";
			return false;
		}
		return true;
	}


	bool testBasics02(string &testName, string &output) {
		testName = GET_TEST_NAME("comparison of undefined should throw FractionError");

		Fraction f = Fraction::UNDEFINED;
		bool result = false;
		slong val = 15;
		try {
			f > val;
		} catch (FractionError &e) {
			result = true;
		}
		if (!result)
			output = "did not throw FractionError";
		return result;
	}

	bool testBasics03(string &testName, string &output) {
		testName = GET_TEST_NAME("Test longer expressions");
		bool result = true;

		Fraction A = (slong) 4;
		Fraction B((slong) 3, (slong) 7);
		Fraction C = (slong) -3;
		Fraction D((slong) 5,(slong)  4);

		Fraction E = A*D - B*C; // 4*(5/4) - (3/7)(-3) = 5 + 9/7 = 44/7

		result = ((((slong)7)*E) == ((slong)44));
		if (!result) {

			output = "4*(5/4) - (3/7)(-3) != 44/7";
		}


		return result;
	}
}

