/*
 * FreeTester.h
 *
 *  Created on: Jun 23, 2015
 *      Author: pham
 */

#ifndef FREETESTER_H_
#define FREETESTER_H_

#include "AssemblerUtil.h"

class IntegratingTester {
public:
	IntegratingTester();

	//Write out elements of gate search space (gate collection)
	void freeTestShowGateSearchSpace(int nbQubits, int maxSequenceLength);

	//Evaluate gate search space (gate collection) by some simple query
	void freeTestEvaluateGateSearchSpace(int nbQubits, int maxSeqLength, double epsilon);

	virtual ~IntegratingTester();

private:

};

#endif /* FREETESTER_H_ */
