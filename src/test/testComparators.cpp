#include "test.hpp"
#include "../fraction/fraction.hpp"
#include <limits>

using namespace std;

template<class T>
bool funkEQ(const T &a, const T &b) {	// ==
	return a == b;
}
template<class T>
bool funkNEQ(const T &a, const T &b) {	// !=
	return a != b;
}
template<class T>
bool funkMT(const T &a, const T &b) {	// >
	return a > b;
}
template<class T>
bool funkLT(const T &a, const T &b) {	// <
	return a < b;
}
template<class T>
bool funkMTE(const T &a, const T &b) {	// >=
	return a >= b;
}
template<class T>
bool funkLTE(const T &a, const T &b) {	// <=
	return a <= b;
}



namespace test {



	bool testComparator01(string &testName, string &output) {
		testName = GET_TEST_NAME("Sorting-method");

		list<Fraction> fracs;
		fracs.push_back(Fraction((slong) -1));
		fracs.push_back(Fraction(2, 3));
		fracs.push_back(-Fraction(2, 3));
		fracs.push_back(-Fraction(2, 3));
		fracs.push_back(Fraction((slong) 2));
		fracs.push_back(Fraction((slong) 0));
		fracs.push_back(Fraction(99, 102));
		fracs.push_back(Fraction(102, 99));
		fracs.push_back(Fraction((slong)23*5)); //23*5 = 115, 23 is outside range of primes
		fracs.push_back(Fraction((slong)29*4)); //29*4 = 116, 29 is outside range of primes
		fracs.push_back(Fraction(23*5, 29*4));
		fracs.push_back(Fraction(1, 2));
		fracs.push_back(-Fraction::INFINITE);
		fracs.push_back(Fraction::INFINITE);

		list<bool(*)(const Fraction&, const Fraction&)> sortingFunctionsFraction;
		list<bool(*)(const double&, const double&)> sortingFunctionsDouble;
		sortingFunctionsFraction.push_back(funkMT<Fraction>);
		sortingFunctionsDouble.push_back(funkMT<double>);

		sortingFunctionsFraction.push_back(funkLT<Fraction>);
		sortingFunctionsDouble.push_back(funkLT<double>);

		sortingFunctionsFraction.push_back(funkMTE<Fraction>);
		sortingFunctionsDouble.push_back(funkMTE<double>);

		sortingFunctionsFraction.push_back(funkLTE<Fraction>);
		sortingFunctionsDouble.push_back(funkLTE<double>);

		list<double> dbls;
		for (list<Fraction>::iterator it = fracs.begin(); it != fracs.end(); it++)
			dbls.push_back(*it);

		list<bool (*)(const Fraction&, const Fraction&)>::iterator sortFunctionFractionIter = sortingFunctionsFraction.begin();
		list<bool (*)(const double&, const double&)>::iterator sortFunctionDoubleIter = sortingFunctionsDouble.begin();
		do {
			fracs.sort(*sortFunctionFractionIter);
			dbls.sort(*sortFunctionDoubleIter);
			/*cout << "double: " << endl;
			for (list<double>::iterator it = dbls.begin(); it != dbls.end(); it++)
				cout << (it == dbls.begin()? "": ", ") << *it;
			cout << endl;

			cout << "Fractions: " << endl;
			for (list<Fraction>::iterator it = fracs.begin(); it != fracs.end(); it++)
				cout << (it == fracs.begin()? "": ", ") << ((double) *it);
			cout << endl;
*/
			list<Fraction>::iterator fracsIt = fracs.begin();
			list<double>::iterator dblsIt = dbls.begin();

			while(fracsIt != fracs.end() && dblsIt != dbls.end()) {
				if (!testRelativeDoubles(fracsIt->getDouble(), *dblsIt)) {
					cout << fracsIt->getDouble() << " != " << *dblsIt << endl;
					output = "comparison operator failed";
					//return false;
				}
				fracsIt++;
				dblsIt++;
			}

			sortFunctionFractionIter++;
			sortFunctionDoubleIter++;
		} while(sortFunctionFractionIter != sortingFunctionsFraction.end());
		return true;
	}


	bool testComparator02(string &testName, string &output) {
		testName = GET_TEST_NAME("compare infinite, undefined and finite");
		const int len = 5;
		const int oprLen = 6;

		string opers[oprLen] = {
				"==",
				"!=",
				">",
				"<",
				">=",
				"<="
		};
		bool (*dblComps[oprLen])(const double&, const double&) = {
				funkEQ<double>,
				funkNEQ<double>,
				funkMT<double>,
				funkLT<double>,
				funkMTE<double>,
				funkLTE<double>
		};
		bool (*fracComps[oprLen])(const Fraction&, const Fraction&) = {
				funkEQ<Fraction>,
				funkNEQ<Fraction>,
				funkMT<Fraction>,
				funkLT<Fraction>,
				funkMTE<Fraction>,
				funkLTE<Fraction>
		};

		string strs[len] = {
				"inf",
				"-inf",
				"undef",
				"0",
				"1"
		};
		Fraction fracs[len] = {
				Fraction(Fraction::INFINITE),
				Fraction(-Fraction::INFINITE),
				Fraction(Fraction::UNDEFINED),
				Fraction((slong) 0),
				Fraction((slong) 1)
		};

		double dbls[len] = {
				numeric_limits<double>::infinity(),
				-numeric_limits<double>::infinity(),
				numeric_limits<double>::quiet_NaN(),
				0,
				1
		};

		bool a[2];
		bool result = true;

		for (int k=0; k<oprLen; k++) {
			for (int i=0; i<len; i++) {
				for (int j=0; j<len; j++) {
					try {
						a[0] = (*fracComps[k])(fracs[i], fracs[j]);
						a[1] = (*dblComps[k])(dbls[i], dbls[j]);
					} catch (FractionError &e) {
						result = false;
						cout << "FractionError: " << e << endl;
						continue;
					}
					if (a[0] != a[1]) {
						cout << strs[i] << " " << opers[k] << " " << strs[j] << " = dbls/fracs:\t" << (a[1]? "true": "false")
													<< "/" << (a[0]? "true": "false") << endl;
						result = false;
					}
				}
			}
		}
		return result;
	}

	bool testComparator03(string &testName, string &output) {
		testName = GET_TEST_NAME("basic comparison of MT, LT between Fractions");
		ulong bn = 70;
		ulong bd = 102;
		ulong cn = 102;
		ulong cd = 99;

		Fraction B(bn, bd);
		Fraction C(cn, cd);

		double b = ((double) bn) / ((double) bd);
		double c = ((double) cn) / ((double) cd);
		bool result = true;
		result &= (B<C) == (b<c);
		result &= (B>C) == (b>c);
		result &= (C<B) == (c<b);
		result &= (C>B) == (c>b);
		if (!result) {
			cout << B << " < " << C << " = " << (B<C? "true": "false") << " = " << (b<c? "true": "false") << endl;
			cout << B << " > " << C << " = " << (B>C? "true": "false") << " = " << (b>c? "true": "false") << endl;
			cout << C << " < " << B << " = " << (C<B? "true": "false") << " = " << (c<b? "true": "false") << endl;
			cout << C << " > " << B << " = " << (C>B? "true": "false") << " = " << (c>b? "true": "false") << endl;
			output = "comparison for Fractions does not match doubles";
		}
		return result;
	}

	bool testComparator04(string &testName, string &output) {
		testName = GET_TEST_NAME("fraction - integer comparison");
		slong s = -20;
		Fraction f = s;
		bool result = true;
		for (; s<20; s+=2) {
			bool equal = (s == f);
			if (!equal) {
				cout << "s = " << s << " == " << f << " = " << f << ": " << (equal? "true": "false") << endl;
				output = "slong(-20 + 2*n) != Fraction(-20 + 2n)";
				result = false;
			}
			f += 2;
		}


		return result;
	}

}
