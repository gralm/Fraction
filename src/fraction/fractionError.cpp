#include "fraction.hpp"
#include <ostream>
#include <list>
#include <cstring>

using namespace std;

void FractionError::constructor(int error, const std::string &message, const std::string &methodName) {
	thrownError = error;
	errorMessage = message;
	add(methodName);
}

FractionError::FractionError(int error, const std::string &methodName, const std::string &message) {
	constructor(error, methodName, message);
}
FractionError::FractionError(const std::string &message, const std::string &methodName) {
	constructor(FractionError::UNKNOWN, methodName, message);
}
FractionError::FractionError(int error, const std::string &methodName) {
	constructor(error, methodName, "");
}


std::ostream& operator <<(std::ostream &std, const FractionError &err) {
	std << "Fraction Error(" << err.thrownError << "): ";
	switch(err.thrownError) {
	case FractionError::DIVISION_BY_ZERO:
		std << "Division by zero";
		break;
	case FractionError::UNDEFINED_VALUE:
		std << "operation with undefined value";
		break;
	case FractionError::CAST_OVERFLOW:
		std << "cast overflow";
		break;
	default:
		std << "undefined error";
	}
	if (err.errorMessage.length() > 0)
		std << "\t\"" << err.errorMessage << "\"";

	if (err.methodList.size() == 0)
		return std;

	for (list<string>::const_reverse_iterator rit = err.methodList.rbegin(); rit != err.methodList.rend(); rit++)
		std << (rit != err.methodList.rbegin()? " -> ": "") << *rit;

	return std;
}

void FractionError::add(const std::string &methodName) {
	methodList.push_back(methodName);
}
