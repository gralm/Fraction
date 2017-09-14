/*
 * fraction.cpp
 *
 *  Created on: 2 juli 2017
 *      Author: admin
 */

#include "fraction.hpp"
#include <iostream>
#include <string>

//const ulong FRACTION_ULONG_MAX = 7;//~((ulong) 0);

using namespace std;

FractionState fractionStateFromMultiplication(FractionState a, FractionState b) {
	if (a == Undefined || b == Undefined)
		return Undefined;
	if (a == Infinite)
		return b == Zero? Undefined: Infinite;
	if (b == Infinite)
		return a == Zero? Undefined: Infinite;
	return (a == Zero || b == Zero)? Zero: Finite;
}

FractionState fractionStateFromDivision(FractionState a, FractionState b) {
	if (a == Undefined || b == Undefined)
		return Undefined;
	if (b == Zero)
		return Undefined;
	if (b == Infinite)
		return a == Infinite? Undefined: Zero;
	return a;
}


Fraction::Fraction(bool positive, FractionState fractionState) {
	this->positive = positive;
	this->state = fractionState;
}


void Fraction::setPrimeFactor(ulong uval) {
	Prime *prm = Prime::getInstance();
	overPrimeFactors.clear();
	if (!prm->factorize(uval, primeFactors))
		overPrimeFactors.push_back(PrimeFactor(uval, 1));
}

// TODO: deprecated
void Fraction::getNumeratorAndDenominatorValues(PrimeFactorList &n, PrimeFactorList &d) const {
	n.clear();
	d.clear();
	Prime *prm = Prime::getInstance();
	for (PrimeFactorList::const_iterator it = primeFactors.begin(); it != primeFactors.end(); it++) {
		if (it->power > 0)
			n.push_back(PrimeFactor(prm->operator [](it->primeIndex), -it->power));
		else
			d.push_back(PrimeFactor(prm->operator [](it->primeIndex), it->power));
	}

	for (PrimeFactorList::const_iterator it = overPrimeFactors.begin(); it != overPrimeFactors.end(); it++) {
		if (it->power > 0)
			n.push_back(*it);
		else
			d.push_back(-*it);
	}
}


bool Fraction::getNumeratorAndDenominator(ulong &numerator, ulong &denominator) const {

	ulong n, d, on, od;
	if (!primeFactors.numeratorAndDenominatorToUlongWithPrimes(n, d))
		return false;

	if (!overPrimeFactors.numeratorAndDenominatorToUlong(on, od))
		return false;

	if (n > FRACTION_ULONG_MAX/on || on > FRACTION_ULONG_MAX/od)
		return false;

	numerator = n*on;
	denominator = d*od;

	return true;
}

Fraction Fraction::UNDEFINED(true, Undefined);
Fraction Fraction::INFINITE(true, Infinite);

Fraction::Fraction() {
	state = Undefined;
	positive = true;
}

// TODO: get this working
/*template<class T> Fraction::Fraction(T val) {
	Prime *prm = Prime::getInstance();
	//ulong uval = (ulong) (val<0? -val: val);
	ulong uval;
	if (val < 0)
		uval = static_cast<ulong>(-val);
	else
		uval = static_cast<ulong>(val);

	positive = val >= 0;
	state = val? Finite: Zero;

	//primeFactors = prm->factorize(uval);
}*/


Fraction::Fraction(slong val) {
	positive = val >= 0;
	ulong uval = (ulong) (positive? val: -val);
	state = val? Finite: Zero;
	setPrimeFactor(uval);
}
// TODO: somehow remove duplicate code
Fraction::Fraction(int val) {
	positive = val >= 0;
	ulong uval = (ulong) (positive? val: -val);
	state = val? Finite: Zero;
	setPrimeFactor(uval);
}
// Fraction::setPrimeFactor(ulong uval)
/*
Fraction::Fraction(ulong val) {
	if (val) {
		positive = true;
		setPrimeFactor(val);
		state = Finite;
	} else {
		state = Zero;
	}
}
*/
Fraction::Fraction(const Fraction &frac) {
	set(frac);
}


Fraction::Fraction(double val) {

	if (val == 0) {
		state = Zero;
		return;
	} else if (val == numeric_limits<double>::quiet_NaN()) {
		state = Undefined;
		return;
	} else if (val == numeric_limits<double>::infinity()) {
		state = Infinite;
		positive = true;
		return;
	} else if (val == -numeric_limits<double>::infinity()) {
		state = Infinite;
		positive = false;
		return;
	}
	this->state = Finite;


	const uint64_t *data = (uint64_t*)(&val);
	//static const uint64_t one = (uint64_t) 1;
	static const uint64_t highestBit = 0x8000000000000000;
	static const uint64_t exponent = 0x7FF0000000000000;
	int exp = ((int) ((exponent&*data) >> 52)) - 1023 - 52;
	static const uint64_t addExtraOne = 0x0010000000000000;
	static const uint64_t mantissa = 0x000fffffffffffff;
	uint64_t mantissaValue = addExtraOne | (mantissa&*data);
	positive = ((*data) & highestBit)? false: true;

	setPrimeFactor(static_cast<ulong>(mantissaValue));
	primeFactors.multiply(PrimeFactor(0, exp));

}

void Fraction::setPrimeFactor(ulong n, ulong d) {
	Prime *prm = Prime::getInstance();
	if (!prm->factorize(n, primeFactors)) {
		overPrimeFactors.multiply(PrimeFactor(n, 1));
	}
	PrimeFactorList toSubtract;
	if (!prm->factorize(d, toSubtract)) {
		if (d == n) {
			overPrimeFactors.clear();
		}else {
			overPrimeFactors.multiply(PrimeFactor(d, -1));
		}
	}
	primeFactors.divide(toSubtract);
}

void Fraction::set(const Fraction &frac) {
	positive = frac.positive;
	state = frac.state;
	if (state == Finite) {
		primeFactors = frac.primeFactors;
		overPrimeFactors = frac.overPrimeFactors;
	}
}

void Fraction::set(slong val) {
	if (val < 0) {
		setPrimeFactor((ulong) -val);
		positive = false;
		state = Finite;
	}else if (!val)
		state = Zero;
	else {
		setPrimeFactor((ulong) val);
		positive = true;
		state = Finite;
	}
}
void Fraction::set(ulong val) {
	if (!val) {
		state = Zero;
	} else {
		state = Finite;
		positive = true;
		setPrimeFactor(val);
	}
}

bool Fraction::safeMultiplicationUlong(ulong a, ulong b, ulong &c) {

	if (a > FRACTION_ULONG_MAX/b)
		return false;
	else
		c = a*b;
	return true;
}

Fraction::Fraction(slong snumerator, slong sdenominator) {
	ulong numerator = (ulong) ((snumerator<0)? -snumerator: snumerator);
	ulong denominator = (ulong) ((sdenominator<0)? -sdenominator: sdenominator);

	if (denominator == 0) {
		state = Undefined;
		return;
	} else if (numerator == 0) {
		state = Zero;
		return;
	}
	setPrimeFactor(numerator, denominator);

	state = Finite;
	positive = snumerator*sdenominator > 0;
}

Fraction Fraction::factorial(ulong val) {
	Fraction f;
	for (ulong i=2; i<=val; i++)
		f *= i;
	return f;
}

Fraction& Fraction::operator=(const Fraction &frac)
{
	set(frac);
	return *this;
}

Fraction& Fraction::operator=(slong val) {
	positive = val >= 0;
	if (val) {
		ulong uval = (ulong) (positive? val: -val);
		state = Finite;
		setPrimeFactor(uval);
	} else
		state = Zero;
	return *this;
}

Fraction::operator slong() const {
	if (state == Zero)
		return 0;
	else if (state == Infinite)
		throw FractionError(FractionError::CAST_OVERFLOW, "slong()", "cast overflow in slong() because of infinite Fraction");
	else if (state == Undefined)
		throw FractionError(FractionError::UNDEFINED_VALUE, "slong()", "undefined value in slong()");

	ulong n, d;
	if (!getNumeratorAndDenominator(n, d)) {
		//return (slong) getDouble();	// this will leave inaccurate results :(
		throw FractionError(FractionError::CAST_OVERFLOW, "slong()", "cast overflow in slong()");
	}
	ulong result = n/d;
	if (result > numeric_limits<slong>::max()) {
		//return (slong) getDouble();
		throw FractionError(FractionError::CAST_OVERFLOW, "slong()", "cast overflow in slong()");
	}

	return positive? ((slong)result): -((slong)result);
}

Fraction::operator double() const {
	return getDouble();
}


// TODO:
std::string Fraction::toString() const {
	return std::string();
}


// helper for operator++ and operator--
void Fraction::plusPlusFunction(bool positivePlusPlus) {
	switch(state) {
	case Zero:
		primeFactors.clear();
		overPrimeFactors.clear();
		state = Finite;
		positive = positivePlusPlus;
		break;
	case Finite: {
		ulong numerator = getNumeratorAsUlong();
		ulong denominator = getDenominatorAsUlong();

		PrimeFactorList od;
		PrimeFactorList d;
		primeFactors.denominatorToPFL(d);
		overPrimeFactors.denominatorToPFL(od);

		if (positive == positivePlusPlus) {
			numerator += denominator;
		} else if (denominator > numerator) {	// can(should) never be equal
			numerator = denominator - numerator;
			positive = positivePlusPlus;
		} else if (denominator == numerator){
			state = Zero;
			overPrimeFactors.clear();
			primeFactors.clear();
			return;
		} else {
			numerator -= denominator;
		}

		//print("A");
		setPrimeFactor(numerator);
		//print("B");
		//cout << "d = " << d << endl;
		primeFactors.divide(d);
		//cout << "od = " << od << endl;
		overPrimeFactors.divide(od);
		//print("C");
		break;
	}
	case Undefined:
		throw FractionError(FractionError::UNDEFINED_VALUE, "Fraction::plusPlusFunction(bool positivePlusPlus)");
	default:
		break;
	}
}


void Fraction::addition(const PrimeFactorList &valA, const PrimeFactorList &valAo,
		const PrimeFactorList &valB, const PrimeFactorList &valBo,
		PrimeFactorList &valS, PrimeFactorList &valSo) {

	PrimeFactorList A(valA);
	PrimeFactorList B(valB);
	PrimeFactorList Ao(valAo);
	PrimeFactorList Bo(valBo);

	valS.baseABC(A, B);
	valSo.baseABC(Ao, Bo);
	ulong a, b;
	ulong ao, bo;
	if (!A.numeratorToUlongWithPrimes(a) ||
			!B.numeratorToUlongWithPrimes(b) ||
			!Ao.numeratorToUlong(ao) ||
			!Bo.numeratorToUlong(bo) ||
			(a > FRACTION_ULONG_MAX/ao) ||
			(b > FRACTION_ULONG_MAX/bo)) {
		throw FractionError(FractionError::CAST_OVERFLOW, "Fraction::operator+");
	}

	a *= ao;
	b *= bo;

	if (a > FRACTION_ULONG_MAX-b)
		throw FractionError(FractionError::CAST_OVERFLOW, "Fraction::operator+");
	ulong ab = a+b;

	PrimeFactorList Ctf;
	Prime *prm = Prime::getInstance();
	if (!prm->factorize(ab, Ctf))
		valSo.multiply(ab);
	valS.multiply(Ctf);
}

// S = A-B, returns -1 if A < B, 0 if A == B, 1 if A > B
int Fraction::subtraction(const PrimeFactorList &valA, const PrimeFactorList &valAo,
		const PrimeFactorList &valB, const PrimeFactorList &valBo,
		PrimeFactorList &valS, PrimeFactorList &valSo) {
	PrimeFactorList A(valA);
	PrimeFactorList B(valB);
	PrimeFactorList Ao(valAo);
	PrimeFactorList Bo(valBo);

	valS.baseABC(A, B);
	valSo.baseABC(Ao, Bo);
	ulong a, b;
	ulong ao, bo;
	if (!A.numeratorToUlongWithPrimes(a) ||
			!B.numeratorToUlongWithPrimes(b) ||
			!Ao.numeratorToUlong(ao) ||
			!Bo.numeratorToUlong(bo) ||
			(a > FRACTION_ULONG_MAX/ao) ||
			(b > FRACTION_ULONG_MAX/bo)) {
		throw FractionError(FractionError::CAST_OVERFLOW, "Fraction::operator+");
	}

	a *= ao;
	b *= bo;

	bool positive = a>b;
	ulong ab = positive? a-b: b-a;
	if (!ab)
		return 0;

	PrimeFactorList Ctf;
	Prime *prm = Prime::getInstance();
	if (!prm->factorize(ab, Ctf))
		valSo.multiply(ab);
	valS.multiply(Ctf);

	return positive? 1: -1;
}

Fraction& Fraction::operator*=(const Fraction &frac) {
	positive = frac.positive == positive;

	state = fractionStateFromMultiplication(this->state, frac.state);
	if (state == Finite) {
		primeFactors.multiply(frac.primeFactors);
		overPrimeFactors.multiply(frac.overPrimeFactors);
	}
	return *this;
}

Fraction Fraction::operator*(const Fraction &frac) const {
	Fraction toReturn(*this);
	toReturn *= frac;
	return toReturn;
}


Fraction& Fraction::operator*=(slong val) {
	positive = positive == (val>=0);
	ulong uval = (ulong) (val<0? -val: val);

	state = fractionStateFromMultiplication(state, val? Finite: Zero);

	if (state == Finite) {
		PrimeFactorList toMultiply;
		if (!Prime::getInstance()->factorize(uval, toMultiply))
			overPrimeFactors.multiply(uval);
		primeFactors.multiply(toMultiply);
	}

	return *this;
}

Fraction Fraction::operator*(slong f) const {
	Fraction toReturn(*this);
	toReturn *= f;
	return toReturn;
}



Fraction& Fraction::operator/=(const Fraction &frac) {
	positive = frac.positive == positive;

	state = fractionStateFromDivision(state, frac.state);
	if (state == Finite) {
		primeFactors.divide(frac.primeFactors);
		overPrimeFactors.divide(frac.overPrimeFactors);
	}
	return *this;
}

Fraction Fraction::operator/(const Fraction &frac) const {
	Fraction toReturn(*this);
	toReturn /= frac;
	return toReturn;
}

Fraction Fraction::operator/(slong f) const {
	Fraction toReturn(*this);
	toReturn /= f;
	return toReturn;
}

Fraction& Fraction::operator/=(slong val) {
	positive = positive == (val>=0);
	ulong uval = (ulong) (val<0? -val: val);

	state = fractionStateFromDivision(state, val? Finite: Zero);

	if (state == Finite) {
		PrimeFactorList toDivide;
		if (!Prime::getInstance()->factorize(uval, toDivide))
			overPrimeFactors.divide(uval);
		primeFactors.divide(toDivide);
	}

	return *this;
}

Fraction& Fraction::operator^=(slong power) {

	if (state == Finite) {
		if (power) {
			for (PrimeFactorList::iterator it = primeFactors.begin(); it != primeFactors.end(); it++)
				it->power *= power;
			for (PrimeFactorList::iterator it = overPrimeFactors.begin(); it != overPrimeFactors.end(); it++)
				it->power *= power;
		} else {
			primeFactors.clear();
			overPrimeFactors.clear();
		}
    } else if (state == Undefined) {
		throw FractionError(FractionError::UNDEFINED_VALUE, "operator^=(slong)", "using undefined value in operator^=(slong power)");
	} else if (state == Zero) {
		if (power == 0) {
			state = Finite;
			primeFactors.clear();
			overPrimeFactors.clear();
		} else if (power < 0)
			state = Infinite;
	} else if (state == Infinite) {
		if (power < 0) {
			state = Zero;
			primeFactors.clear();
			overPrimeFactors.clear();
		}
	}
	return *this;
}

Fraction Fraction::operator^(slong power) const {
	Fraction toReturn(*this);
	toReturn ^= power;
	toReturn.positive = !toReturn.positive && (power&1) ? false: true;
	return toReturn;
}


Fraction Fraction::operator+(const Fraction &frac) const {
	Fraction toReturn(*this);
	toReturn += frac;
	return toReturn;
}

Fraction& Fraction::operator+=(const Fraction &frac) {
	// C = A+B;
	if (state == Undefined || frac.state == Undefined) {
		state = Undefined;
	} else if (state == Infinite) {
		state = (positive != frac.positive && frac.state == Infinite)? Undefined: Infinite;
	} else if (frac.state == Infinite) {
		positive = (positive != frac.positive)? frac.positive: positive;
		state = Infinite;
	} else if (state == Zero) {
		set(frac);
	} else if (frac.state != Zero) { 	// state = frac.state = Finite
		PrimeFactorList valS, valSo;
		if (positive == frac.positive) {
			addition(primeFactors, overPrimeFactors,
					frac.primeFactors, frac.overPrimeFactors,
					valS, valSo);
		} else {
			switch(subtraction(primeFactors, overPrimeFactors,
					frac.primeFactors, frac.overPrimeFactors,
					valS, valSo)) {
			case -1:
				positive = frac.positive;
				break;
			case 0:
				state = Zero;
				return *this;
			}
		}
		primeFactors.set(valS);
		overPrimeFactors.set(valSo);
	}
	return *this;
}

Fraction Fraction::operator+(slong val) const {
	Fraction toReturn(*this);
	toReturn += val;
	return toReturn;
}

Fraction& Fraction::operator+=(slong val) {
	if (!val) {
		switch(state) {
		case Infinite:
		case Undefined:
			state = Undefined;
			break;
		default:
			state = Zero;
			break;
		}
		return *this;
	}

	// val = Finite
	if (state != Finite) {
		if (state == Zero)
			set(val);
		return *this;
	}

	bool valSgn = val > 0;
	ulong uval = (ulong) (valSgn? val: -val);
	ulong num, den;
	PrimeFactorList denPfl, oDenPfl;
	primeFactors.denominatorToPFL(denPfl);
	overPrimeFactors.denominatorToPFL(oDenPfl);

	if (!getNumeratorAndDenominator(num, den))
		throw FractionError(FractionError::CAST_OVERFLOW, "Fraction& Fraction::operator+=(slong val)");

	ulong vd = den * uval;
	ulong newNum;
	if (valSgn == positive) {	// n/d + v = (n+vd)/d = newNum / d
		if (uval > FRACTION_ULONG_MAX/den || vd > FRACTION_ULONG_MAX - num)
			throw FractionError(FractionError::CAST_OVERFLOW, "Fraction& Fraction::operator+=(slong val)");
		newNum = vd + num;
	} else {	// n/d - v = (n-vd)/d = newNum / d
		if (uval > FRACTION_ULONG_MAX/den)
			throw FractionError(FractionError::CAST_OVERFLOW, "Fraction& Fraction::operator+=(slong val)");

		if (num > vd) {
			newNum = num - vd;
		} else if (num == vd) {
			state = Zero;
			return *this;
		} else {
			newNum = vd - num;
			positive = !positive;
		}
	}

	primeFactors.set(denPfl);
	overPrimeFactors.set(oDenPfl);
	Prime *prm = Prime::getInstance();
	if (!prm->factorize(newNum, denPfl)) {
		overPrimeFactors.multiply(PrimeFactor(newNum, 1));
	}
	primeFactors.multiply(denPfl);
	return *this;
}

Fraction Fraction::operator++(int) {
	Fraction toReturn(*this);
	plusPlusFunction(true);
	return toReturn;
}

Fraction& Fraction::operator++()
{
	plusPlusFunction(true);
	return *this;
}

Fraction Fraction::operator-(const Fraction &frac) const {
	Fraction toReturn(*this);
	toReturn -= frac;
	return toReturn;
}

Fraction& Fraction::operator-=(const Fraction &frac) {
	// C = A+B;
	if (state == Undefined || frac.state == Undefined) {
		state = Undefined;
	} else if (state == Infinite) {
		state = (positive == frac.positive && frac.state == Infinite)? Undefined: Infinite;
	} else if (frac.state == Infinite) {
		positive = (positive != frac.positive)? frac.positive: positive;
		state = Infinite;
	} else if (state == Zero) {
		set(frac);
		positive = !frac.positive;
	} else if (frac.state != Zero) { 	// state = frac.state = Finite
		PrimeFactorList valS, valSo;
		if (positive != frac.positive) {
			addition(primeFactors, overPrimeFactors,
					frac.primeFactors, frac.overPrimeFactors,
					valS, valSo);
		} else {
			switch(subtraction(primeFactors, overPrimeFactors,
					frac.primeFactors, frac.overPrimeFactors,
					valS, valSo)) {
			case -1:
				positive = !positive;
				break;
			case 0:
				state = Zero;
				return *this;
			}
		}
		primeFactors.set(valS);
		overPrimeFactors.set(valSo);
	}
	return *this;
}

Fraction Fraction::operator-(slong val) const {
	Fraction toReturn(*this);
	toReturn += (-val);
	return toReturn;
}

Fraction& Fraction::operator-=(slong val) {
	return *this += (-val);
}


Fraction Fraction::operator--(int) {
	Fraction toReturn(*this);
	plusPlusFunction(false);
	return toReturn;
}

Fraction& Fraction::operator--() {
	plusPlusFunction(false);
	return *this;
}

Fraction Fraction::operator-() const {
	Fraction toReturn(*this);
	toReturn.positive = !toReturn.positive;
	return toReturn;
}

/************* Comparison *******************/

FractionComparisonResult Fraction::comparison(slong val) const {
	if (state == Undefined)
		throw FractionError(FractionError::UNDEFINED_VALUE, "comparison(slong)");

	if (state == Zero)	return val? (val>0? LessThan: MoreThan): Equal;

	if (state == Infinite || ((val>0) != positive) || (val==0))
		return positive? MoreThan: LessThan;
	ulong num;
	ulong den;

	try {
		num = getNumeratorAsUlong();
		den = getDenominatorAsUlong();
	} catch(FractionError &e) {	// TODO: dont throw, just catch and use with "class Long"
		e.add("comparison(slong)");
		throw e;
	}

	ulong uval = (ulong) (val<0? -val: val);
	if (den != 1)
		return ((num/den) > uval) == positive? MoreThan: LessThan;
	else if (num == uval)	// den = 1;
		return Equal;

	return (num > uval) == positive? MoreThan: LessThan;
}

	// for undefined, infinite and -infinite Fractions should return same results as double
FractionComparisonResult Fraction::comparison(const Fraction &frac) const {
	if (state == Undefined || frac.state == Undefined)
		return FractionComparisonResult::Uncomparable;

	if (state == Infinite) {
		return (frac.state == Infinite && positive == frac.positive)? Equal: (positive? MoreThan: LessThan);
	} else if (state == Zero) {
		return frac.state == Zero? Equal: (frac.positive? LessThan: MoreThan);
	}

	// state = finite
	if (frac.state == Zero)
		return positive? MoreThan: LessThan;
	else if (frac.state == Infinite)
		return frac.positive? LessThan: MoreThan;

	// frac.state = finite;
	if (positive != frac.positive)
		return positive? MoreThan: LessThan;

	// An/Ad = this > frac = Bn/Bd
	// this/frac = AnBd / AdBn, AnBd > AdBn

	ulong n, d, on, od;
	PrimeFactorList pfl(this->primeFactors);
	pfl.divide(frac.primeFactors);
	if (!pfl.numeratorAndDenominatorToUlongWithPrimes(n, d)) {
		throw FractionError(FractionError::CAST_OVERFLOW, "comparison(const Fraction&)");
	}
	PrimeFactorList opfl(this->overPrimeFactors);
	opfl.divide(frac.overPrimeFactors);
	if (!opfl.numeratorAndDenominatorToUlong(on, od)) {
		throw FractionError(FractionError::CAST_OVERFLOW, "comparison(const Fraction&)");
	}
	if (n > FRACTION_ULONG_MAX/on || d > FRACTION_ULONG_MAX/od) {
		throw FractionError(FractionError::CAST_OVERFLOW, "comparison(const Fraction&)");
	}
	n *= on;
	d *= od;
	return (n == d)? Equal: (((n>d) == positive)? MoreThan: LessThan);
}



bool Fraction::operator>(slong val) const {
	bool returnValue = false;
	try {
		returnValue = comparison(val) == MoreThan;
	} catch (FractionError &e) {
		e.add("operator>(slong)");
		throw e;
	}
	return returnValue;
}

bool Fraction::operator>(const Fraction &frac) const {
	bool returnValue = false;
	try {
		returnValue = comparison(frac) == MoreThan;
	} catch (FractionError &e) {
		e.add("operator>(const Fraction&)");
		throw e;
	}
	return returnValue;
}

bool Fraction::operator>=(slong val) const {
	FractionComparisonResult returnValue = Uncomparable;
	try {
		returnValue = comparison(val);
	} catch (FractionError &e) {
		e.add("operator>=(slong)");
		throw e;
	}
	return returnValue == MoreThan || returnValue == Equal;
}

bool Fraction::operator>=(const Fraction &frac) const {
	FractionComparisonResult returnValue = Uncomparable;
	try {
		returnValue = comparison(frac);
	} catch (FractionError &e) {
		e.add("operator>=(const Fraction&)");
		throw e;
	}
	return returnValue == MoreThan || returnValue == Equal;
}


bool Fraction::operator<(slong val) const {
	bool returnValue = false;

	try {
		returnValue = comparison(val) == LessThan;
	} catch (FractionError &e) {
		e.add("operator<(slong)");
		throw e;
	}
	return returnValue;
}

bool Fraction::operator<(const Fraction &frac) const {
	bool returnValue = false;
	try {
		returnValue = comparison(frac) == LessThan;
	} catch (FractionError &e) {
		e.add("operator<(const Fraction&)");
		throw e;
	}
	return returnValue;
}

bool Fraction::operator<=(slong val) const {
	FractionComparisonResult returnValue = Uncomparable;
	try {
		returnValue = comparison(val);
	} catch (FractionError &e) {
		e.add("operator<=(slong)");
		throw e;
	}
	return returnValue == LessThan || returnValue == Equal;
}

bool Fraction::operator<=(const Fraction &frac) const {
	FractionComparisonResult returnValue = Uncomparable;
	try {
		returnValue = comparison(frac);
	} catch (FractionError &e) {
		e.add("operator<=(const Fraction&)");
		throw e;
	}
	return returnValue == LessThan || returnValue == Equal;
}

bool Fraction::operator==(slong val) const {
	bool returnValue = false;
	try {
		returnValue = comparison(val) == Equal;
	} catch (FractionError &e) {
		e.add("operator==(slong)");
		throw e;
	}
	return returnValue;

}

bool Fraction::operator==(const Fraction &frac) const {
	FractionComparisonResult fcr = comparison(frac);
	return fcr == Equal;
}


bool Fraction::operator!=(slong val) const {
	return !(*this == val);
}

// TODO: create special Equal method, faster than full comparison.
bool Fraction::operator!=(const Fraction &frac) const {
	FractionComparisonResult fcr = comparison(frac);
	return fcr != Equal;

}


/********************************************/

void Fraction::print(const string& output) const
{
	cout << output << ":" << endl;
	cout << "\tPrimeFactors: " << this->primeFactors << endl;
	cout << "\tOverPrimeFactors: " << this->overPrimeFactors << endl;
	cout << "\tstate = ";
	switch(state) {
	case Finite:
		cout << "Finite" << endl;
		break;
	case Infinite:
		cout << "Infinite" << endl;
		break;
	case Undefined:
		cout << "Undefined" << endl;
		break;
	case Zero:
		cout << "Zero" << endl;
		break;
	}
	cout << "\tpositive = " << (positive? "true": "false") << endl;

}

	// signature does not matter
ulong Fraction::getNumeratorAsUlong() const {
	if (state == Zero)
		return 0;
	Prime *prm = Prime::getInstance();
	ulong val1, val2;


	if (!prm->getUlongIfPossible(true, this->primeFactors, val1) ||
			!overPrimeFactors.numeratorToUlong(val2) ||
			val1 > FRACTION_ULONG_MAX/val2) {
		//cout << "val1 = " << val1 << "\t val2 = " << val2 << "\tasdf = " << FRACTION_ULONG_MAX/val2 << endl;
		throw FractionError(FractionError::CAST_OVERFLOW, "getNumeratorAsUlong()");
	}

	return val1*val2;
}

ulong Fraction::getDenominatorAsUlong() const {
	if (state == Zero)
		return 0;
	Prime *prm = Prime::getInstance();
	ulong val1, val2;
	if (!prm->getUlongIfPossible(false, this->primeFactors, val1) ||
			!overPrimeFactors.denominatorToUlong(val2) ||
			val1 > FRACTION_ULONG_MAX/val2) {
		//cout << "val1 = " << val1 << "\t val2 = " << val2 << "\tasdf = " << FRACTION_ULONG_MAX/val2 << endl;
		throw FractionError(FractionError::CAST_OVERFLOW, "getDenominatorAsUlong()");
	}

	return val1*val2;
}

double Fraction::getDouble() const
{
	if (state == Zero)
		return 0;
	else if (state == Infinite)
		return positive? numeric_limits<double>::infinity(): -numeric_limits<double>::infinity();
	else if (state == Undefined)
		return numeric_limits<double>::quiet_NaN();

	Prime *prm = Prime::getInstance();
	double val = 1;
	for (PrimeFactorList::const_iterator it = primeFactors.begin(); it != primeFactors.end(); it++) {
		slong prime = (*prm)[it->primeIndex];
		if (it->power > 0) {
			for (slong i = 0; i < it->power; i++)
				val *= prime;
		} else {
			for (slong i = 0; i > it->power; i--)
				val /= prime;
		}
	}

	for (PrimeFactorList::const_iterator it = overPrimeFactors.begin(); it != overPrimeFactors.end(); it++)
	{
		if (it->power > 0) {
			for (slong i = 0; i < it->power; i++)
				val *= it->primeIndex;
		} else {
			for (slong i = 0; i > it->power; i--)
				val /= it->primeIndex;
		}
	}
	return positive? val: -val;
}

void Fraction::update() {
	// TODO: should this be done?
}

std::ostream& operator <<(std::ostream &os, const Fraction &frac)
{
	if (frac.state == Zero)
		return os << "0";
	else if (frac.state == Infinite)
		return os << (frac.positive? "#INF": "-#INF");
	else if (frac.state == Undefined)
		return os << "#UNDEFINED";
	ulong den = frac.getDenominatorAsUlong();
	if (den > 1)
		return os << (frac.positive? "": "-") << frac.getNumeratorAsUlong() << "/" << den;
	return os << (frac.positive? "": "-") << frac.getNumeratorAsUlong();
}

std::ostream& operator <<(std::ostream &os, const PrimeFactor &pf) {
	return os << "[p(" << pf.primeIndex << ")]^("<< pf.power << ")";
	/*return os << "[p(" << pf.primeIndex << ")=" << Prime::getInstance()->operator [](pf.primeIndex)
			<< "]^("<< pf.power << ")";*/
}


Fraction operator*(slong val, const Fraction &frac) {
	Fraction frac2(frac);
	frac2 *= val;
	return frac2;
}


Fraction operator/(slong val, const Fraction &frac) {
	Fraction frac2(frac);
	frac2 /= val;
	return frac2;
}
Fraction operator+(slong val, const Fraction &frac) {
	return frac + val;
}
Fraction operator-(slong val, const Fraction &frac) {
	return -(frac-val);
}

slong& operator*=(slong &val, const Fraction &frac) {
	Fraction newFraction(val);
	newFraction *= frac;
	val = (slong) newFraction;
	return val;
}

slong& operator/=(slong &val, const Fraction &frac) {
	Fraction newFraction(val);
	newFraction /= frac;
	val = (slong) newFraction;
	return val;
}

// TODO: kolla om de här funktionerna överhuvudtaget används
slong& operator+=(slong &val, const Fraction &frac) {
	slong plusVal(frac);
	val += plusVal;
	return val;
}

slong& operator-=(slong &val, const Fraction &frac) {
	slong plusVal(frac);
	val -= plusVal;
	return val;
}


bool operator>(slong val, const Fraction &frac) {
	return frac < val;
}

bool operator>=(slong val, const Fraction &frac) {
	return frac <= val;
}
bool operator<(slong val, const Fraction &frac) {
	return frac > val;
}
bool operator<=(slong val, const Fraction &frac) {
	return frac >= val;
}
bool operator==(slong val, const Fraction &frac) {
	return frac == val;
}
bool operator!=(slong val, const Fraction &frac) {
	return !(frac == val);
}




