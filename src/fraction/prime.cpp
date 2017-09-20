#include "fraction.hpp"
#include <iostream>
#include <ostream>
#include <istream>
#include <fstream>
using namespace std;

Prime Prime::prime;

Prime::Prime() {
	cout << "Primes created" << endl;
	primes = 0;
	highestPrime = 0;
	highestIndex = 0;
	filename = "";
}

Prime::~Prime() {
	if (primes)
		delete[] primes;
	cout << "Primes destroyed" << endl;
	primes = 0;
}

void Prime::changeLengthIndex(slong newHighestIndex)
{
	if (newHighestIndex <= this->highestIndex)
		return;

	// nextPossiblePrime = 6n + {1, 5}, n=1,2,3... -> = 7,9, 11,13, 17,19, 23,25...
	ulong *newPrimes = new ulong[newHighestIndex+1];
	slong index = highestIndex;
	ulong nextPossiblePrime;
	ulong nextAdd;
	slong sqrNextPossiblePrimeIndex;
	ulong sqrNextPossiblePrime2;
	if (primes) {
		for (slong i=0; i<=highestIndex; i++)
			newPrimes[i] = primes[i];
		index = highestIndex+1;
		nextAdd = (highestPrime%6)==1? 4: 2;
		nextPossiblePrime = highestPrime + nextAdd;
		nextAdd ^= 6;

		sqrNextPossiblePrimeIndex = getIndexFromPrime(static_cast<ulong>(
				sqrt(static_cast<double>(nextPossiblePrime)))+1);
		sqrNextPossiblePrime2 = primes[sqrNextPossiblePrimeIndex]*primes[sqrNextPossiblePrimeIndex];
	} else {
		newPrimes[0] = 2;
		newPrimes[1] = 3;
		newPrimes[2] = 5;
		index = 3;
		nextPossiblePrime = 7;
		nextAdd = 4;
		sqrNextPossiblePrimeIndex = 2;
		sqrNextPossiblePrime2 = 25;
	}

	while(index <= newHighestIndex) {

		bool isPrime = true;
		for (slong indexToTest = 2; indexToTest < sqrNextPossiblePrimeIndex; indexToTest++)
		{
			ulong primeToTest = newPrimes[indexToTest];
			if (primeToTest * (nextPossiblePrime / primeToTest) == nextPossiblePrime)
			{
				isPrime = false;
				break;
			}
		}

		if (isPrime) {
			newPrimes[index] = nextPossiblePrime;
			index++;
		}

		nextPossiblePrime += nextAdd;
		nextAdd ^= 6;

		if (nextPossiblePrime >= sqrNextPossiblePrime2)
		{
			sqrNextPossiblePrimeIndex++;
			sqrNextPossiblePrime2 = newPrimes[sqrNextPossiblePrimeIndex]*newPrimes[sqrNextPossiblePrimeIndex];
		}

	}

	if (primes)
		delete[] primes;

	primes = newPrimes;
	highestIndex = newHighestIndex;
	highestPrime = primes[highestIndex];
}

Prime* Prime::getInstance()
{
	if (!prime.primes)
		prime.changeLengthIndex(7);
	return &prime;
}

void Prime::setHighestIndex(slong highestIndex)
{
	changeLengthIndex(highestIndex);
}

ulong Prime::operator[](slong index) const
{
	return primes[index];
}

slong Prime::getIndexFromPrime(ulong primeNumber) const
{
	// https://en.wikipedia.org/wiki/Prime-counting_function
	// f(x) = int(2 -> x) {dy / ln (y) } = the logarithmic integral ~= x / ln(x)
	// prime density = f'(x) = 1 / ln(x)
	if (primeNumber > highestPrime)
		return -1;

	if (primeNumber < 20)
	{
		slong index = 0;
		while(primes[index] < primeNumber)
			index++;
		return index;
	}

	double startIndex = primeNumber * log(static_cast<double>(primeNumber));
	// TODO: fixa senare
	return static_cast<slong>(startIndex);
}

slong Prime::getIndexFromPrime2(ulong primeNumber) const
{
	// https://en.wikipedia.org/wiki/Prime-counting_function
	// f(x) = int(2 -> x) {dy / ln (y) } = the logarithmic integral ~= x / ln(x)
	// prime density = f'(x) = 1 / ln(x)
	if (primeNumber > highestPrime)
		return -1;

	if (primeNumber < 20)
	{
		slong index = 0;
		while(primes[index] < primeNumber)
			index++;
		return index;
	}

	double startIndex = primeNumber * log(static_cast<double>(primeNumber));
	// TODO: fixa senare
	return static_cast<slong>(startIndex);
}

ulong Prime::getHighestIndex() const {
	return this->highestIndex;
}

void Prime::saveAs(const std::string &filename) const
{
	std::ofstream outfile(filename.c_str(), std::ofstream::binary);

	ulong lengthInBytesOfUlong = sizeof(ulong);
	ulong lengthInBytesOfPrimes = (this->highestIndex+1) * lengthInBytesOfUlong;

	char lengthInBytesOfUlongAsChar = (char) lengthInBytesOfUlong;
	//outFile.write(buffer, size);
	outfile.write((char*) &lengthInBytesOfUlongAsChar, 1);
	//outfile.write((char*)(&lengthInBytesOfPrimes), lengthInBytesOfUlong);
	outfile.write((char*)primes, lengthInBytesOfPrimes);
// 129 bytes = 31*8 + 8 + 1
	outfile.close();
}
void Prime::save() const
{

}

// TODO rewrite for int_8, int_16...
void Prime::load(const std::string &filename) {
	std::ifstream infile(filename.c_str(), std::ifstream::binary);

	  // get size of file
	infile.seekg (0, infile.end);
	long size = infile.tellg();
	infile.seekg(0);
	if (size < 1)
	{
		cout << "file size < 1" << endl;
		infile.close();
		return;
	}

	// allocate memory for file content
	char* buffer = new char[size-1];

	// read content of infile
	char ulongSize;
	//infile.read (buffer,size);
	infile.read(&ulongSize, 1);
	infile.read(buffer, size-1);

	if (sizeof(ulong) != ulongSize) {
		cout << "wrong element size in filename:  \"" << filename << "\", sizeof(ulong) = " <<
				sizeof(ulong) << " != filesize element size = " << ulongSize << endl;
		infile.close();
		return;
	}

	if (primes)
		delete[] primes;

	primes = (ulong*) buffer;
	highestIndex = ((slong)(size-1)) / ((slong) ulongSize) - 1;
	highestPrime = primes[highestIndex];
	infile.close();
}

bool Prime::factorize(ulong &value, list<PrimeFactor> &pfs) const {
	if (value == 0)
		return false;


	pfs.clear();
	slong index = 0;
	do {
		slong power = 0;
		bool changed = false;
		do {
			ulong divd = value / primes[index];
			changed = divd*primes[index] == value;
			if (changed) {
				value = divd;
				power++;
			}
		} while(changed);
		if (power)
			pfs.push_back(PrimeFactor(index, power));
		index++;

	} while(value != 1 && index <= highestIndex);

	return value == 1;
}

void Prime::print() const
{
	for (slong i=0; i<=this->highestIndex; i++)
		cout << "prm[" << i << "]:\t" << primes[i] << endl;
}

/*
PrimeComparisonResult Prime::compare(std::list<PrimeFactor> &pfs1, std::list<PrimeFactor> &pf2) {
	// TODO:
	return EqualTo;
}

PrimeComparisonResult Prime::compare(std::list<PrimeFactor> &pfs, ulong val) {
	// TODO:
	return EqualTo;
}*/

bool Prime::getUlongIfPossible(bool useNumerator, const std::list<PrimeFactor> &pfs, ulong &val) {
	val = 1;
	static ulong maxLimit = ~((ulong) 0);
	for(list<PrimeFactor>::const_iterator it = pfs.begin(); it != pfs.end(); it++) {
		ulong thisPrime = primes[it->primeIndex];
		ulong tfMaxLimit = maxLimit / thisPrime;
		if (useNumerator) {
			for (slong i=0; i<it->power; i++) {
				if (val > tfMaxLimit)
					return false;
				val *= thisPrime;
			}
		} else {
			for (slong i=0; i>it->power; i--) {
				if (val > tfMaxLimit)
					return false;
				val *= thisPrime;
			}
		}
	}
	return true;
}
