#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <list>
#include <cstring>
#include <ctime>
#include <exception>

#define GET_TEST_NAME(comment)		createTestName(__FILE__, __func__, comment, output);


namespace test {

	const double finite_ = 0.0001;

	bool testRelativeDoubles(double val1, double val2);

		/// all tests:
	bool testMultiplication01(std::string &testName, std::string &output);
	bool testMultiplication02(std::string &testName, std::string &output);
	bool testMultiplication03(std::string &testName, std::string &output);
	bool testMultiplication04(std::string &testName, std::string &output);
	bool testMultiplication05(std::string &testName, std::string &output);
	bool testMultiplication06(std::string &testName, std::string &output);
	bool testMultiplication07(std::string &testName, std::string &output);
	bool testAddition01(std::string &testName, std::string &output);
	bool testAddition02(std::string &testName, std::string &output);
	bool testAddition03(std::string &testName, std::string &output);
	bool testAddition04(std::string &testName, std::string &output);
	bool testComparator01(std::string &testName, std::string &output);
	bool testComparator02(std::string &testName, std::string &output);
	bool testComparator03(std::string &testName, std::string &output);
	bool testComparator04(std::string &testName, std::string &output);
	bool testBasics01(std::string &testName, std::string &output);
	bool testBasics02(std::string &testName, std::string &output);
	bool testBasics03(std::string &testName, std::string &output);
	bool testBasics04(std::string &testName, std::string &output);
	bool testPrimes01(std::string &testName, std::string &output);
	bool testMatrix01(std::string &testName, std::string &output);
	bool testParse01(std::string &testName, std::string &output);





	std::string createTestName(const char *fileName, const char *testName, const char *Comment, std::string &output);
	void runAll();
	void runOne();
	void runTests(std::list<bool (*)(std::string&, std::string&)> &testList);

}

#endif
