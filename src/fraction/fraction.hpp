#ifndef FRACTION_HPP_
#define FRACTION_HPP_

#include <list>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdint>

typedef unsigned long long 	ulong;
typedef long long			slong;
//typedef unsigned int 	ulong;
//typedef int			slong;


// TODO: remove this constant and use numeric_limits<ulong>::max() instead
const ulong FRACTION_ULONG_MAX = ~((ulong)0);

/* *************************************************** */

struct PrimeFactor {
	ulong primeIndex;
	slong power;
	PrimeFactor(ulong primeIndex, slong power) {this->primeIndex = primeIndex; this->power = power;}
	PrimeFactor operator-() const {	return PrimeFactor(primeIndex, -power);}
};


/* ********************* Prime singleton ****************************** */
class Prime {
private:
	static Prime prime;

	ulong *primes;
	ulong highestPrime;
	slong highestIndex;
	std::string filename;

	Prime();
	~Prime();

	void changeLengthIndex(slong highestIndex);
public:

	static Prime* getInstance();
	void setHighestIndex(slong highestIndex);

	ulong operator[](slong index) const;

	slong getIndexFromPrime(ulong primeNumber) const;
	ulong getHighestIndex() const;
	void print() const;
	void saveAs(const std::string &filename) const;
	void save() const;
	void load(const std::string &filename);

	void slowTest();
	//std::list<PrimeFactor> factorize(ulong value) const;
		// returns (value == 1), unfactorized primes are stored in value, pfs consists of factorized primes
	bool factorize(ulong &value, std::list<PrimeFactor> &pfs) const; // https://en.wikipedia.org/wiki/Integer_factorization
	bool getUlongIfPossible(bool numerator, const std::list<PrimeFactor>&, ulong&);
};

/* *************************************************** */

enum FractionState {
	Zero,
	Finite,
	Infinite,
	Undefined
};

enum FractionComparisonResult {
	MoreThan,
	Equal,
	LessThan,
	Uncomparable
};

/* ************************ Fraction Error ************************ */

// TODO: check if it should inherit from std::exception ?
class FractionError {
	friend std::ostream& operator <<(std::ostream&, const FractionError&);

private:
	int thrownError;
	std::string errorMessage;
	std::list<std::string> methodList;
	void constructor(int error, const std::string &message, const std::string &methodName);

public:
	static const int UNKNOWN = -1;
	static const int DIVISION_BY_ZERO = 0;
	static const int UNDEFINED_VALUE = 1;
	static const int CAST_OVERFLOW = 2;

	FractionError(int error, const std::string &methodName, const std::string &message);
	FractionError(const std::string &message, const std::string &methodName);
	FractionError(int error, const std::string &methodName);

	void add(const std::string &methodName);
};

std::ostream& operator <<(std::ostream&, const FractionError&);


/* ************************* PrimeFactorList ************************* */
class PrimeFactorList: public std::list<PrimeFactor> {
public:
	PrimeFactorList();
	PrimeFactorList(ulong);
	PrimeFactorList(const std::list<PrimeFactor>&);

	void set(ulong);
	void set(const PrimeFactor&);
	void set(const std::list<PrimeFactor>&);

	void multiply(ulong index);
	void multiply(const PrimeFactor&);
	void multiply(const PrimeFactorList&);
	void divide(ulong index);
	void divide(const PrimeFactorList&);

	// X' = output, X = input
	// C = getBaseOf(A, B)
	// C'*A' = A
	// C'*B' = B
	// if A' contains PrimeFactor(i, *) then B' will not contain a PrimeFactor(i, *)
	// if B' contains PrimeFactor(i, *) then A' will not contain a PrimeFactor(i, *)
	// all powers in A' and B' are positive (no denominator exists)
	static void baseAB(PrimeFactorList &A, PrimeFactorList &B);
	void baseAB(PrimeFactorList &B);	// this = A

	void baseABC(PrimeFactorList &A, PrimeFactorList &B);	// this = C
	static void baseABC(PrimeFactorList &A, PrimeFactorList &B, PrimeFactorList &C);

	void baseC(const PrimeFactorList &B, PrimeFactorList &C) const;
	static void baseC(const PrimeFactorList &A, const PrimeFactorList &B, PrimeFactorList &C);

	void numeratorToPFL(PrimeFactorList&) const;
	void denominatorToPFL(PrimeFactorList&) const;

		// TODO: fixa
	FractionComparisonResult comparison(const PrimeFactorList&) const;
	bool numeratorToUlong(ulong&) const;	// multiplies the primeIndex without first converting them to prime-values
	bool numeratorToUlongWithPrimes(ulong&) const;	// product of the prime-values
	bool denominatorToUlong(ulong&) const;	// multiplies the primeIndex without first converting them to prime-values
	bool denominatorToUlongWithPrimes(ulong&) const;	// product of the prime-values
	bool numeratorAndDenominatorToUlong(ulong &n, ulong &d) const;
	bool numeratorAndDenominatorToUlongWithPrimes(ulong &n, ulong &d) const;

		// checks that the PrimeFactors are in an ordered list and no powers are equal to zero
	bool isValid() const;
};

/* ************************* Fraction **************************** */

class Fraction {
		// friend-operators
	friend std::ostream& operator <<(std::ostream&, const Fraction&);

private:
	PrimeFactorList overPrimeFactors;		// holds values that are product of primes higher than prime[getHighestIndex()-1]
	PrimeFactorList primeFactors;			// all prime number factorized values
	FractionState state;
	bool positive;
	Fraction(bool positive, FractionState fractionState);
	void setPrimeFactor(ulong val);
	void setPrimeFactor(ulong n, ulong d);
	void set(const Fraction&);
	void set(slong val);
	void set(ulong val);


	static bool safeMultiplicationUlong(ulong a, ulong b, ulong&);

	void getNumeratorAndDenominatorValues(PrimeFactorList &n, PrimeFactorList &d) const;
	bool getNumeratorAndDenominator(ulong &numerator, ulong &denominator) const;

	void plusPlusFunction(bool);
	// S = A+B
	static void addition(const PrimeFactorList &A, const PrimeFactorList &Ao,
			const PrimeFactorList &B, const PrimeFactorList &Bo,
			PrimeFactorList &S, PrimeFactorList &So);
	// S = A-B, returns -1 if A < B, 0 if A == B, 1 if A > B
	static int subtraction(const PrimeFactorList &A, const PrimeFactorList &Ao,
			const PrimeFactorList &B, const PrimeFactorList &Bo,
			PrimeFactorList &S, PrimeFactorList &So);

	FractionComparisonResult comparison(slong val) const;
	FractionComparisonResult comparison(const Fraction &frac) const;

public:
	static Fraction UNDEFINED;
	static Fraction INFINITE;

	Fraction();
	// TODO: create template methods?? or???
	//template<> Fraction<typename U>(U u);
	template<class T> Fraction(T);
	Fraction(slong);

	// TODO: Annoying constructor using int, slong can not be defined as an int when this constructor exists
	Fraction(int);
	Fraction(const Fraction&);


	// this double conversion is only possible for 64bit-slong/ulong, if slong = int it will throw overflow exception
	// TODO: should be able to handle 32-bit and x-bit slong/ulong's
	Fraction(double val);


		// expects n and d > 0
	Fraction(slong numerator, slong denominator);

	static Fraction factorial(ulong);

	Fraction& operator=(const Fraction&);
	Fraction& operator=(slong val);
	operator slong() const;
	operator double() const;


	/************ operators *******************/
	Fraction& operator*=(const Fraction&);
	Fraction operator*(const Fraction&) const;
	Fraction operator*(slong) const;
	Fraction& operator*=(slong);

		// division by zero should always return undefined as of mathematical standard, it should never return undefined
	Fraction& operator/=(const Fraction&);
	Fraction operator/(const Fraction&) const;
	Fraction operator/(slong) const;
	Fraction& operator/=(slong);

	Fraction& operator^=(slong power);
	Fraction operator^(slong power) const;

	Fraction operator+(const Fraction&) const;
	Fraction& operator+=(const Fraction&);
	Fraction operator+(slong) const;
	Fraction& operator+=(slong);
	Fraction& operator++();
	Fraction operator++(int);


	Fraction operator-(const Fraction&) const;
	//Fraction operator-(Fraction const&) const;
	Fraction& operator-=(const Fraction&);
	Fraction operator-(slong) const;
	Fraction& operator-=(slong);
	Fraction& operator--();
	Fraction operator--(int);

	Fraction operator-() const;
	/************ comparison *******************/

	bool operator==(slong val) const;
	bool operator==(const Fraction&) const;
	bool operator!=(slong val) const;
	bool operator!=(const Fraction&) const;

	//TODO, these operators only guarantees correct result when absolute value is <= ~((ulong) 0) use "Long" to evaluate these for all values
	// A > B can be compared in multiple ways, each one has pros and cons:
	// log(A) > log(B); for large values of PrimeFactor.power the comparison will be quicker, but this has lower accuracy
	// Fraction(A) > Fraction(B); conversion to Fractions and then using getBaseOf
	// ulong(A) > ulong(B); is quick for small values and inaccurate when overriding
	bool operator>(slong val) const;
	bool operator>(const Fraction&) const;
	bool operator>=(slong val) const;
	bool operator>=(const Fraction&) const;
	bool operator<(slong val) const;
	bool operator<(const Fraction&) const;
	bool operator<=(slong val) const;
	bool operator<=(const Fraction&) const;

	/************************************/
	void print(const std::string&) const;
	ulong getNumeratorAsUlong() const;
	ulong getDenominatorAsUlong() const;

	// TODO: remove
	double getDouble() const;

	// TODO: if Prime::prime.highestPrime is updated overPrimeFactors might be able to factorized by running Fraction::update();
	void update();

	std::string toString() const;
};

std::ostream& operator <<(std::ostream&, const Fraction&);
std::ostream& operator <<(std::ostream&, const PrimeFactor&);
std::ostream& operator <<(std::ostream&, const PrimeFactorList&);

Fraction operator*(slong, const Fraction&);	// returns Fraction for higher precision
Fraction operator/(slong, const Fraction&);
Fraction operator+(slong, const Fraction&);
Fraction operator-(slong, const Fraction&);
slong& operator*=(slong&, const Fraction&);
slong& operator/=(slong&, const Fraction&);
slong& operator+=(slong&, const Fraction&);
slong& operator-=(slong&, const Fraction&);

bool operator>(slong, const Fraction&);
bool operator>=(slong, const Fraction&);
bool operator<(slong, const Fraction&);
bool operator<=(slong, const Fraction&);
bool operator==(slong, const Fraction&);
bool operator!=(slong, const Fraction&);


/* *************************************************** */

#endif /* FRACTION_HPP_ */
