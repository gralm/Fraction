#include "test.hpp"
#include "../fraction/fraction.hpp"

using namespace std;

namespace test {



	bool testPrimes01(string &testName, string &output) {
		testName = GET_TEST_NAME("test generate, load and save primes");
		bool result = true;
		Prime *prm = Prime::getInstance();
		prm->setHighestIndex(100);
		ulong highestIndex100 = prm->getHighestIndex();
		prm->saveAs("primes100.prm");

		prm->setHighestIndex(200);
		prm->saveAs("primes200.prm");
		ulong highestIndex200 = prm->getHighestIndex();

		prm->load("primes100.prm");
		ulong highestIndex100AfterLoad = prm->getHighestIndex();
		prm->load("primes200.prm");
		ulong highestIndex200AfterLoad = prm->getHighestIndex();

		if (highestIndex100 != 100 ||
				highestIndex200 != 200 ||
				highestIndex100AfterLoad != 100 ||
				highestIndex200AfterLoad != 200) {
			cout << "highestIndex100 = " << highestIndex100 << endl;
			cout << "highestIndex200 = " << highestIndex200 << endl;
			cout << "highestIndex100After = " << highestIndex100AfterLoad << endl;
			cout << "highestIndex200AfterLoad = " << highestIndex200AfterLoad << endl;
			output = "cannot load and save primes correctly";
			result = false;
		}

		return result;
	}
}
