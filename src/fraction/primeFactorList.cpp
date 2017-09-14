#include "fraction.hpp"
#include <ostream>
#include <sstream>
// TODO: remove later
#include <iostream>

using namespace std;

PrimeFactorList::PrimeFactorList() {}

PrimeFactorList::PrimeFactorList(ulong val) {
	set(val);
}
PrimeFactorList::PrimeFactorList(const std::list<PrimeFactor> &val) {
	set(val);
}

// TODO: fixa det här omedelbart!!!
void PrimeFactorList::set(ulong val) {
	clear();
	push_back(PrimeFactor(val, 1));
}
void PrimeFactorList::set(const PrimeFactor &pf) {
	clear();
	push_back(pf);
}
void PrimeFactorList::set(const std::list<PrimeFactor> &val) {
	clear();
	for (list<PrimeFactor>::const_iterator it = val.begin(); it != val.end(); it++)
		push_back(*it);
}

void PrimeFactorList::multiply(ulong index) {
	multiply(PrimeFactor(index, 1));
}
void PrimeFactorList::multiply(const PrimeFactor &pf) {
	list<PrimeFactor>::iterator it = begin();
	do {
		if (it == end() || it->primeIndex > pf.primeIndex) {
			insert(it, pf);
			//push_back(pf);
			return;
		} else if (it->primeIndex == pf.primeIndex) {
			if (it->power == -pf.power)
				erase(it);
			else
				it->power += pf.power;
			return;
		}
		it++;
	} while(true);
}
void PrimeFactorList::multiply(const PrimeFactorList &pfl) {
	list<PrimeFactor>::iterator it = begin();
	list<PrimeFactor>::const_iterator fracIt = pfl.begin();
	while(it != end() && fracIt != pfl.end()) {
		if (it->primeIndex > fracIt->primeIndex) {
			insert(it, *fracIt);

			fracIt++;
		} else if (it->primeIndex == fracIt->primeIndex) {
			if (it->power == -fracIt->power) {
				it = erase(it);
			} else {
				it->power += fracIt->power;
				it++;
			}
			fracIt++;
		} else {
			it++;
		}
	}

	while(fracIt != pfl.end()) {
		push_back(*fracIt);
		fracIt++;
	};
}

void PrimeFactorList::divide(ulong index) {
	multiply(PrimeFactor(index, -1));
}


void PrimeFactorList::divide(const PrimeFactorList &pfl) {
	list<PrimeFactor>::iterator it = begin();
	list<PrimeFactor>::const_iterator fracIt = pfl.begin();

	while(it != end() && fracIt != pfl.end()) {
		if (it->primeIndex > fracIt->primeIndex) {
			insert(it, PrimeFactor(fracIt->primeIndex, -fracIt->power));
			fracIt++;

		} else if (it->primeIndex == fracIt->primeIndex) {
			if (it->power == fracIt->power) {
				it = erase(it);
			} else {
				it->power -= fracIt->power;
				it++;
			}
			fracIt++;
		} else {
			it++;
		}
	}

	while(fracIt != pfl.end()) {
		push_back(PrimeFactor(fracIt->primeIndex, -fracIt->power));
		fracIt++;
	};
}

void PrimeFactorList::baseAB(PrimeFactorList &A, PrimeFactorList &B) {
	A.baseAB(B);
}

void PrimeFactorList::baseAB(PrimeFactorList &B) {
	list<PrimeFactor>::iterator itA = begin();
	list<PrimeFactor>::iterator itB = B.begin();
	while(itA != end() || itB != B.end()) {
		if (itA == end()) {
			if (itB->power < 0) {
				push_back(-*itB);
				itB = B.erase(itB);
			} else {
				itB++;
			}
		} else if (itB == B.end()) {
			if (itA->power < 0) {
				B.push_back(-*itA);
				itA = erase(itA);
			} else {
				itA++;
			}
		} else if (itA->primeIndex < itB->primeIndex) {
			if (itA->power > 0) {
				itA++;
			} else {
				B.push_back(-*itA);
				itA = erase(itA);
			}
		} else if (itA->primeIndex == itB->primeIndex) {
			if (itA->power > itB->power) {
				itA->power -= itB->power;
				itB = B.erase(itB);
				itA++;
			} else if (itA->power == itB->power) {
				itA = erase(itA);
				itB = B.erase(itB);
			} else { // itA->power < itB->power
				itB->power -= itA->power;
				itA = erase(itA);
				itB++;
			}
		} else { // itA->primeIndex > itB->primeIndex
			if (itB->power > 0) {
				itB++;
			} else {
				push_back(-*itB);
				itB = B.erase(itB);
			}
		}
	}
}

void PrimeFactorList::baseABC(PrimeFactorList &A, PrimeFactorList &B) {
	clear();
	list<PrimeFactor>::iterator itA = A.begin();
	list<PrimeFactor>::iterator itB = B.begin();
	while(itA != A.end() || itB != B.end()) {
		if (itA == A.end()) {
			if (itB->power < 0) {
				A.push_back(-*itB);
				push_back(*itB);
				itB = B.erase(itB);
			} else {
				itB++;
			}
		} else if (itB == B.end()) {
			if (itA->power < 0) {

				B.push_back(-*itA);
				push_back(*itA);
				itA = A.erase(itA);
			} else {

				itA++;
			}
		} else if (itA->primeIndex < itB->primeIndex) {
			if (itA->power > 0) {

				itA++;
			} else {

				push_back(*itA);
				B.push_back(-*itA);
				itA = A.erase(itA);
			}
		} else if (itA->primeIndex == itB->primeIndex) {
			if (itA->power > itB->power) {

				push_back(*itB);
				itA->power -= itB->power;
				itB = B.erase(itB);
				itA++;
			} else if (itA->power == itB->power) {

				push_back(*itA);
				itA = A.erase(itA);
				itB = B.erase(itB);
			} else {

				push_back(*itA);
				itB->power -= itA->power;
				itA = A.erase(itA);
				itB++;
			}
		} else {
			if (itB->power > 0) {

				itB++;
			} else {

				push_back(*itB);
				A.push_back(-*itB);
				itB = B.erase(itB);
			}
		}
	}
}
void PrimeFactorList::baseABC(PrimeFactorList &A, PrimeFactorList &B, PrimeFactorList &C) {
	C.baseABC(A, B);
}

void PrimeFactorList::baseC(const PrimeFactorList &B, PrimeFactorList &C) const {
	list<PrimeFactor>::const_iterator itA = begin();
	list<PrimeFactor>::const_iterator itB = B.begin();
	while(itA != end() || itB != B.end()) {
		if (itA == end()) {
			if (itB->power < 0)
				C.push_back(*itB);
			itB++;
		} else if (itB == B.end()) {
			if (itA->power < 0)
				C.push_back(*itA);
			itA++;
		} else if (itA->primeIndex < itB->primeIndex) {
			if (itA->power < 0)
				C.push_back(*itA);
			itA++;
		} else if (itA->primeIndex == itB->primeIndex) {
			C.push_back(itA->power > itB->power? *itB: *itA);
			itA++;
			itB++;
		} else {
			if (itB->power < 0)
				C.push_back(*itB);
			itB++;
		}
	}
}
void PrimeFactorList::baseC(const PrimeFactorList &A, const PrimeFactorList &B, PrimeFactorList &C) {
	A.baseC(B, C);
}


void PrimeFactorList::numeratorToPFL(PrimeFactorList &pfl) const {
	pfl.clear();
	for (const_iterator it = begin(); it != end(); it++) {
		if (it->power > 0)
			pfl.push_back(*it);
	}
}
void PrimeFactorList::denominatorToPFL(PrimeFactorList &pfl) const {
	pfl.clear();
	for (const_iterator it = begin(); it != end(); it++) {
		if (it->power < 0)
			pfl.push_back(-*it);
	}
}

FractionComparisonResult PrimeFactorList::comparison(const PrimeFactorList&) const {
	return Equal;
}

bool PrimeFactorList::numeratorToUlong(ulong &val) const {
	val = 1;
	static ulong maxLimit = ~((ulong) 0);
	for(list<PrimeFactor>::const_iterator it = begin(); it != end(); it++) {
		ulong thisPrime = it->primeIndex;
		ulong tfMaxLimit = maxLimit / thisPrime;
		for (slong i=0; i<it->power; i++) {
			if (val > tfMaxLimit)
				return false;
			val *= thisPrime;
		}
	}
	return true;
}
bool PrimeFactorList::numeratorToUlongWithPrimes(ulong &val) const {
	return Prime::getInstance()->getUlongIfPossible(true, *this, val);
}
bool PrimeFactorList::denominatorToUlong(ulong &val) const {
	val = 1;
	static ulong maxLimit = ~((ulong) 0);
	for(list<PrimeFactor>::const_iterator it = begin(); it != end(); it++) {
		ulong thisPrime = it->primeIndex;
		ulong tfMaxLimit = maxLimit / thisPrime;
		for (slong i=0; i>it->power; i--) {
			if (val > tfMaxLimit)
				return false;
			val *= thisPrime;
		}
	}
	return true;
}
bool PrimeFactorList::denominatorToUlongWithPrimes(ulong &val) const {
	return Prime::getInstance()->getUlongIfPossible(false, *this, val);
}
bool PrimeFactorList::numeratorAndDenominatorToUlong(ulong &n, ulong &d) const {
	static ulong maxLimit = ~((ulong) 0);
	n = 1;
	d = 1;

	for(list<PrimeFactor>::const_iterator it = begin(); it != end(); it++) {
		ulong tfMaxLimit = maxLimit / it->primeIndex;
		if (it->power > 0) {
			for (slong i=0; i<it->power; i++) {
				if (n > tfMaxLimit)
					return false;
				n *= it->primeIndex;
			}
		} else {
			for (slong i=0; i>it->power; i--) {
				if (d > tfMaxLimit)
					return false;
				d *= it->primeIndex;
			}
		}
	}
	return true;
}
bool PrimeFactorList::numeratorAndDenominatorToUlongWithPrimes(ulong &n, ulong &d) const {
	Prime *prm = Prime::getInstance();
	return prm->getUlongIfPossible(true, *this, n) && prm->getUlongIfPossible(false, *this, d);
}

bool PrimeFactorList::isValid() const {
	ulong lastIndex;
	for (list<PrimeFactor>::const_iterator it = begin(); it != end(); it++) {
		if (it == begin())
			lastIndex = it->primeIndex;
		else if (it->primeIndex <= lastIndex) {
			std::cout << "error -> " << it->primeIndex << " <= " << lastIndex << endl;
			return false;
		}

		if (it->power == 0)
			return false;
	}
	return true;
}

ostream& operator <<(ostream &os, const PrimeFactorList &pfl) {
	const int maxCount = 20;
	int count = maxCount;
	os << "{";
	stringstream tf;
	for (list<PrimeFactor>::const_iterator it = pfl.begin(); it != pfl.end(); it++) {
		tf << (it == pfl.begin()? "": " * ") << *it;
		//os << (it == pfl.begin()? "": " * ") << *it;
		if (--count < 0)
			break;
	}
	if (count < 0)
		os << "More Than " << maxCount << " elements";
	else
		os << tf.str();
	os << "}";
	return os;
}

