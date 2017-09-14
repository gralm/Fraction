#include <iostream>
#include <list>
#include <cstring>
#include <ctime>
#include <thread>
#include <chrono>
#include <exception>
#include "test.hpp"
#include "../fraction/fraction.hpp"
#include <sstream>

using namespace std;

namespace test {

	bool testRelativeDoubles(double val1, double val2) {
		//return true;
		if (val1 == 0) return val2<0? val2>-finite_: val2<finite_;
		if (val2 == 0) return val1<0? val1>-finite_: val1<finite_;

		if (val1 == numeric_limits<double>::infinity() ||
				val1 == numeric_limits<double>::quiet_NaN() ||
				val1 == -numeric_limits<double>::infinity()) {
			return val1 == val2;
		}
		double rel = val1/val2;
		return rel < (1+finite_) && rel > (1-finite_);
	}

	// ugly C-99 / Java mixture-method, but it works :)
	std::string createTestName(const char *fileName, const char *testName, const char *comment, string &output) {
		output = "";
		stringstream ss;
		ss << fileName << ": " << testName << "() - " << comment;
		return ss.str();
	}

	void runAll() {
		list<bool (*)(string&, string&)> testList = {
				testPrimes01,
				testMatrix01,
				testMultiplication01,
				testMultiplication02,
				testMultiplication03,
				testMultiplication04,
				testMultiplication05,
				testMultiplication06,
				testMultiplication07,
				testAddition01,
				testAddition02,
				testAddition03,
				testAddition04,
				testComparator01,
				testComparator02,
				testComparator03,
				testComparator04,
				testBasics01,
				testBasics02,
				testBasics03,
				testParse01
		};
		runTests(testList);
	}

	void runOne() {
		list<bool (*)(string&, string&)> testList = {
				testMatrix01,
		};

		runTests(testList);
	}

	void runTests(list<bool (*)(string&, string&)> &testList) {

		cout << "**************************" << endl;
		int failed = 0, succeeded = 0;
		for (list<bool (*)(string&, string&)>::iterator testIt = testList.begin(); testIt != testList.end(); testIt++) {
			string testName, output;
			if ((*testIt)(testName, output)) {
				succeeded++;
				cout << testName << ": " << "Passed" << endl;
			} else {
				failed++;
				cout << testName << ": " << "Failed\t" << output << endl;
			}
		}

		cout << "**************************" << endl;
		cout << (failed? "FAILED": "PASSED") << endl;
		cout << "Passed: " << succeeded << "/" << (succeeded+failed) << "\tFailed: " << failed << "/" << (succeeded+failed) << endl;
		cout << "**************************" << endl;
	}
}
