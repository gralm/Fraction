# Fraction
c++ type Fraction. Is a c++ type that can handle integer fractions using prime number factorization. There is an example-class of Fraction-use in the inversion of a 3x3-matrix. 

To build project and run basic example:
>> make example

To build entire project and run all tests:
>> make tests

## Source code
```
void basicExample() {
	Prime *prm = Prime::getInstance();
	prm->setHighestIndex(100);
	cout << "100th prime = " << (*prm)[99] << endl;
		// Fraction can factorize the first 100 primes

　
	Fraction f = 5;
	f /= 7;
	f *= 3;
	cout << "f = 5/7*3 = " << f << endl;
	slong expon = -2;	// slong is a long long int
	Fraction g = f^(expon);
	cout << "g = f^-2 = " << g << endl;
	Fraction oneFourth = 0.25;
	Fraction h = g * oneFourth * Fraction(9,7);
	cout << "h = g * (1/4) * (9/7) = " << h << endl;
	Fraction i = --h;
	i *= 50;
	cout << "i = 50(h-1) = " << i << endl;
	slong mySignedLong = i;
	cout << "(signed long long int) i = " << mySignedLong << endl;
	cout << "(double) i = " << ((double)i) << endl;
}
```

## Output
```
Primes created
100th prime = 541
f = 5/7*3 = 15/7
g = f^-2 = 49/225
h = g * (1/4) * (9/7) = 7/100
i = 50(h-1) = -93/2
(signed long long int) i = -46
(double) i = -46.5
Primes destroyed
```

