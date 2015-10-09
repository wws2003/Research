//============================================================================
// Name        : TestMatrixOperator.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "TestSuite.h"

using namespace std;

int main() {
#if MPFR_REAL
	mpfr::mpreal::set_default_prec(256);
#endif

	TestSuite testSuite;
	testSuite.test();

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
