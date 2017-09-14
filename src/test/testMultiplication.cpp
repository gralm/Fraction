#include "test.hpp"
#include "../fraction/fraction.hpp"

using namespace std;

namespace test {



	bool testMultiplication01(string &testName, string &output) {
		testName = GET_TEST_NAME("Check basic Fraction-multiplication and -division");

		Prime *prm = Prime::getInstance();
		prm->load("primes.prm");
		slong fVal = 3*3*5*13;
		double fv = (double) fVal;

		Fraction fF(fVal);	// [0,2,1,0,0,1,0]

		if (!testRelativeDoubles(fF.getDouble(), fv)) {
			output = "Failed in comparing f and fv";
			return false;
		}

		slong gVal = 3*5*5*13*17;
		double gv = (double) gVal;
		Fraction gF(gVal);	// [0,1,2,0,0,0,1]
		if (!testRelativeDoubles(gF.getDouble(), gv)) {
			output = "Failed in comparing g and gv";
			return false;
		}


		fF *= gF;
		fv *= gv;
		if (!testRelativeDoubles(fF.getDouble(), fv)) {
			cout << fF << ", " << fF.getDouble() << ", " << fv << endl;
			output = "Failed in comparing f*g and fv";
			return false;
		}


		fF /= gF;
		fv /= gv;
		if (!testRelativeDoubles(fF.getDouble(), fv)) {
			output = "Failed in comparing f and fv";
			return false;
		}

		fF /= gF;
		fv /= gv;
		if (!testRelativeDoubles(fF.getDouble(), fv)) {
			output = "Failed in comparing f/g and fv";
			return false;
		}
		return true;
	}

	bool testMultiplication02(string &testName, string &output) {
		testName = GET_TEST_NAME("Check basic Fraction-multiplication and -division 2");
		ulong f = (ulong) (6*3*3*11);
		ulong g = (ulong) (5*3*3*11);
		Fraction fF(f, g);
		f = fF.getNumeratorAsUlong();
		g = fF.getDenominatorAsUlong();
		if (f != 6 || g != 5) {
			output = "f != 6 || g != 5";
			return false;
		}
		return true;
	}

	bool testMultiplication03(string &testName, string &output) {
		testName = GET_TEST_NAME("Check factorial calculation");
		Fraction frac = (slong)1;
		for (slong i = 1; i<=10; i++)
			frac *= 2*i;

		if (frac.getNumeratorAsUlong() != (ulong) 3715891200) {
			output = "20!! != 3715891200";
			return false;
		}

		for (slong i = 19; i> 1; i-=2)
			frac /= i;

		double approx = (double) frac;
		if (!testRelativeDoubles(approx, 5.6754638550304184979107579726775)) {
			output = "20!! / 19!! != 5.675...";
			return false;
		}

		// 20!! / 19!! = 2*4*6*8*10*12*14*16*18*20 / 3*5*7*9*11*13*15*17*19
		// = 2^10 2*3*4*5*6*7*8*9*10 / 3*5*7*9*11*13*15*17*19
		// = 2^10 2*2*2*2*2*2*2*2*3*3*3*3*5*5*7 / 3*5*7*3*3*11*13*3*5*17*19
		// = 2^10 2*2*2*2*2*2*2*2*3*3*3*3*5*5*7 / 3*3*3*3*5*5*7*11*13*17*19
		// = 2^18 / 11*13*17*19 ~= 5,6754638550304184979107579726775
		return true;
	}


	bool testMultiplication04(string &testName, string &output) {
		testName = GET_TEST_NAME("check overPrimeFactors");
		slong val = (slong) 5*23*29;
		slong div = (slong) 7*23*29;

		Fraction A(val);
		Fraction Aref = (slong) 5;
		Aref /= 7;
		Fraction Adiv = A/div;
		Fraction Amul = A*Fraction(div);


		if (Adiv != Aref) {
			output = "(5*23*29) / (7*23*29) != 5/7";
			return false;
		}

		try {
			Amul *= Fraction((slong) 23*29)^((slong) (-2));
			/*Fraction asdf((ulong) 23*29);
			asdf = asdf^((slong) (-2));
			Amul *= asdf;*/
			if (Amul != (slong)35) {
				output = "(5*23*29) * (7*23*29) * (23*29)^(-2) != 5*7";
				return false;
			}
		} catch (FractionError &e) {
			cout << e << endl;
		}
		return true;
	}


	bool testMultiplication05(string &testName, string &output) {
		testName = GET_TEST_NAME("check slong multiplication");

		slong val = 100;
		Fraction frac((slong) 5);

		val *= frac;
		if (val != ((slong)500)) {
			output = "5*100 != 500";
			cout << "val = " << val << endl;
			return false;
		}
		return true;
	}

	bool testMultiplication06(string &testName, string &output) {
		testName = GET_TEST_NAME("basic multiplication");
		slong maxLength = 5;
		bool result = true;
		for (slong a = -maxLength;a < maxLength; a++) {
			Fraction A(a);
			for (slong b = -maxLength;b < maxLength; b++) {
				Fraction B(b);
				slong ab = A*B;
				if (ab != a*b) {
					result = false;
					cout << A << " * " << B << " = " << A*B << " != " << a*b << ", ab=" << ab << endl;
					output = "a*b != ab";
				}
			}
		}
		return result;
	}

	bool testMultiplication07(string &testName, string &output) {
		testName = GET_TEST_NAME("test class PrimeFactorLists");
		// (2^3 / 5^2) * (5*3/2)^i, i = 0,1,2
		// (8/25) * (15/2)^i

		PrimeFactorList pfl;
		pfl.set(PrimeFactor(2, -2));
		//ulong n, d;
		pfl.multiply(PrimeFactor(0, 3));
		PrimeFactorList divider;
		divider.set(PrimeFactor(0, -1));
		divider.multiply(PrimeFactor(2, 1));
		divider.multiply(PrimeFactor(1, 1));

		// [3, 0, -2] * [-1, 1, 1]^i
		// [3, 0, -2], [2, 1, -1,], [1, 2, 0], [0, 3, 1], [-1, 4, 2]

		//cout << "divider = " << divider << (divider.isValid()? "": "\tINVALID") << endl;//
		PrimeFactorList firstResult(pfl);
		for (int i=0; i<5; i++) {
			//pfl.numeratorAndDenominatorToUlongWithPrimes(n, d);
			//cout << "A * B^" << i << " = " << pfl << "\t = " << n << "/" << d << (pfl.isValid()? "": "\tINVALID") << endl;
			pfl.multiply(divider);
		}

		for (int i=4; i>=0; i--) {
			pfl.divide(divider);
			//pfl.numeratorAndDenominatorToUlongWithPrimes(n, d);
			//cout << "A * B^" << i << " = " << pfl << "\t = " << n << "/" << d << (pfl.isValid()? "": "\tINVALID") << endl;
		}
		PrimeFactorList lastResult(pfl);
		firstResult.divide(lastResult);
		if (firstResult.size() != 0) {
			output = "firstResult != lastResult";
			return false;
		}
		return true;
	}

}
