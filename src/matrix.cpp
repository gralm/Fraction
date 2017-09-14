#include "matrix.hpp"

using namespace std;

template<class T>
void Matrix<T>::set(const Matrix<T> &M) {
	for (int i=0; i<9; i++) {
		x[i] = M.x[i];
	}
}

template<class T>
Matrix<T>::Matrix() {
	x = new T[9];
	set(Unit);
}

template<class T>
Matrix<T>::~Matrix() {
	delete[] x;
}

template<class T>
Matrix<T>::Matrix(const T *t) {
	x = new T[9];
	for (int i=0; i<9; i++) {
		x[i] = t[i];
	}
}

template<class T>
Matrix<T>::Matrix(
		const T &t00, const T &t01, const T &t02,
		const T &t10, const T &t11, const T &t12,
		const T &t20, const T &t21, const T &t22) {
	x = new T[9] {
		t00, t01, t02,
		t10, t11, t12,
		t20, t21, t22
	};
}

template<class T>
Matrix<T>::Matrix(const Matrix &M) {
	x = new T[9];
	set(M);
}

template<class T>
const Matrix<T> Matrix<T>::Unit(
		static_cast<T>(1), static_cast<T>(0), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(1), static_cast<T>(0),
		static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));


template<class T>
std::string Matrix<T>::toString() const {
	return "";
}

template<class T>
T* Matrix<T>::operator[](int val) {
	return x + (val*3);
}

template<class T> Matrix<T>& Matrix<T>::operator=(const Matrix<T> &M)
{
	set(M);
	return *this;
}

template<class T>
T Matrix<T>::getDeterminant() const {
	T t(0);
	for (int i=0; i<3; i++) {
		t += x[i] * x[3+(i+1)%3] * x[6+(i+2)%3];
		t -= x[i] * x[3+(i+2)%3] * x[6+(i+1)%3];
	}
	return t;
}

template<class T>
void Matrix<T>::invert() {
	Matrix<T> V;
	V = getInversion();
	set(V);
}

template<class T>
Matrix<T> Matrix<T>::getInversion() const {
	T det = getDeterminant();
	Matrix<T> toReturn;

	for (int r=0; r<3; r++) {
		for (int c=0; c<3; c++) {

			T A11(x[((c+1)%3)*3 + ((r+1)%3)]);
			T A12(x[((c+1)%3)*3 + ((r+2)%3)]);
			T A21(x[((c+2)%3)*3 + ((r+1)%3)]);
			T A22(x[((c+2)%3)*3 + ((r+2)%3)]);
			toReturn.x[r*3+c] = A11*A22;
			toReturn.x[r*3+c] -= A12*A21;
			toReturn.x[r*3+c] /= det;
		}
	}
	return toReturn;
}

template<class T>
Matrix<T> Matrix<T>::multiply(const Matrix<T> &M) const {
	Matrix<T> t;
	for (int r=0; r<3; r++) {
		for (int c=0; c<3; c++) {
			t.x[r*3+c] = 0;
			for (int i=0; i<3; i++) {
				t.x[r*3+c] += x[r*3+i] * M.x[i*3+c];
			}
		}
	}
	return t;
}

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix &M) const {
	Matrix<T> t;
		for (int r=0; r<3; r++) {
			for (int c=0; c<3; c++) {
				t.x[r*3+c] = 0;
				for (int i=0; i<3; i++) {
					t.x[r*3+c] += x[r*3+i] * M.x[i*3+c];
				}
			}
		}
		return t;
}

template<class T>
bool Matrix<T>::test() const {
	bool result = true;
	for (int r=0; r<3; r++) {
		for (int c=0; c<3; c++) {

			T val = static_cast<T>(r==c? 1: 0);
			if (x[r*3+c] != val) {
				cout << "error: X[" << r << "][" << c << "] = " << x[3*r+c] << " != " << (r==c? 1: 0) << endl;
				result = false;
			}
		}
	}
	return result;
}

template<class T> bool Matrix<T>::operator==(const Matrix<T> &M) {
	for (int i=0; i<9; i++) {
		if (x[i] != M.x[i])
			return false;
	}
	return true;
}
template<class T> bool Matrix<T>::operator!=(const Matrix<T> &M) {
	return !(*this == M);
}


template<class U>
ostream& operator<<(ostream &os, const Matrix<U> &m) {
	os << "[" << m.x[0] << ",\t" << m.x[1] << ",\t" << m.x[2] << ",\n";
	os << m.x[3] << ",\t" << m.x[4] << ",\t" << m.x[5] << ",\n";
	os << m.x[6] << ",\t" << m.x[7] << ",\t" << m.x[8] << "]";
	return os;
}


template class Matrix<double>;
template class Matrix<Fraction>;

template std::ostream& operator<<(std::ostream &os, const Matrix<double> &m);
template std::ostream& operator<<(std::ostream &os, const Matrix<Fraction> &m);
