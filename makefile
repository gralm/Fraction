CC = g++

OBJPATH = obj/
OBJFLAGS = -c -std=c++11 -Wall -Wextra -o
EXEFILE = fraction
TESTFILE = testfile

example:
	rm -f $(OBJPATH)test*.o
	make fraction
	make -s $(OBJPATH)main.o
	
	$(CC) -o $(EXEFILE).exe $(OBJPATH)*.o

$(OBJPATH)main.o: src/main.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)main.o src/main.cpp 



fraction: 
	make -s $(OBJPATH)prime.o
	make -s $(OBJPATH)fraction.o
	make -s $(OBJPATH)primeFactorList.o
	make -s $(OBJPATH)fractionError.o
	


$(OBJPATH)prime.o: src/fraction/prime.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)prime.o src/fraction/prime.cpp 

$(OBJPATH)fractionError.o: src/fraction/fractionError.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)fractionError.o src/fraction/fractionError.cpp 

$(OBJPATH)fraction.o: src/fraction/fraction.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)fraction.o src/fraction/fraction.cpp 

$(OBJPATH)primeFactorList.o: src/fraction/primeFactorList.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)primeFactorList.o src/fraction/primeFactorList.cpp 


matrix:
	make -s $(OBJPATH)matrix.o

$(OBJPATH)matrix.o: src/matrix.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)matrix.o src/matrix.cpp 
 
tests:
	rm -f $(OBJPATH)main.o
	make fraction 
	make matrix
	make -s $(OBJPATH)test.o
	make -s $(OBJPATH)testAdding.o
	make -s $(OBJPATH)testBasics.o
	make -s $(OBJPATH)testComparators.o
	make -s $(OBJPATH)testMatrix.o
	make -s $(OBJPATH)testMultiplication.o
	make -s $(OBJPATH)testParse.o
	make -s $(OBJPATH)testPrimes.o
	make -s $(OBJPATH)testAll.o
	
	$(CC) -o $(TESTFILE).exe $(OBJPATH)*.o 
	
	
$(OBJPATH)test.o: src/test/test.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)test.o src/test/test.cpp 
	
$(OBJPATH)testAdding.o: src/test/testAdding.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testAdding.o src/test/testAdding.cpp 
	
$(OBJPATH)testBasics.o: src/test/testBasics.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testBasics.o src/test/testBasics.cpp 
	
$(OBJPATH)testComparators.o: src/test/testComparators.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testComparators.o src/test/testComparators.cpp 
	
$(OBJPATH)testMatrix.o: src/test/testMatrix.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testMatrix.o src/test/testMatrix.cpp 
	
$(OBJPATH)testMultiplication.o: src/test/testMultiplication.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testMultiplication.o src/test/testMultiplication.cpp 
	
$(OBJPATH)testParse.o: src/test/testParse.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testParse.o src/test/testParse.cpp 
	
$(OBJPATH)testPrimes.o: src/test/testPrimes.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testPrimes.o src/test/testPrimes.cpp 
	
$(OBJPATH)testAll.o: src/testAll.cpp
	$(CC) $(OBJFLAGS) $(OBJPATH)testAll.o src/testAll.cpp 
	
	


clean: 
	rm -f $(OBJPATH)*.o *.exe
