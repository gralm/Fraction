#include <iostream>
#include <string>
#include <ostream>
#include "fraction/fraction.hpp"

template<class T>
class Matrix {
	template<class U>
	friend std::ostream& operator<<(std::ostream &os, const Matrix<U> &m);
private:
	T *x;

	void set(const Matrix &M);

public:
	Matrix();
	Matrix(const T*);
	Matrix(const T&, const T&, const T&, const T&, const T&, const T&, const T&, const T&, const T&);
	Matrix(const Matrix&);
	~Matrix();

	static const Matrix Unit;
	std::string toString() const;
	T getDeterminant() const;
	void invert();
	T* operator[](int val);
	Matrix<T>& operator=(const Matrix<T>&);
	Matrix<T> getInversion() const;
	//TODO: remove this method, and only use operator*
	Matrix<T> multiply(const Matrix &M) const;
	Matrix<T> operator*(const Matrix &M) const;
	bool test() const;
	bool operator==(const Matrix<T>&);
	bool operator!=(const Matrix<T>&);
};

template<class T>
std::ostream& operator<<(std::ostream &os, const Matrix<T> &m);
