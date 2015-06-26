//============================================================================
// Name        : TestEvaluator.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "IntegratingTester.h"
#include "TestSuite.h"

using namespace std;

#define UT 1

int main() {
	cout << "!!!Testing algorithm and evaluator!!!" << endl; // prints !!!Hello World!!!
#if UT
	TestSuite testSuite;
	testSuite.test();
#else
	IntegratingTester tester;
	//tester.freeTestShowGateSearchSpace(1, 4);
	tester.freeTestEvaluateGateSearchSpace(1, 15, 1);
#endif
	return 0;
}
