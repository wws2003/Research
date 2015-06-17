/*
 * InverseCancelationMultiplierMatrixCombiner.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#include "InverseCancelationMultiplierMatrixCombinerImpl.h"
#include <string>
#include <ctype.h>

bool InverseCancelationMultiplierMatrixCombinerImpl::shouldCombine(MatrixPtr pMatrix1, MatrixPtr pMatrix2) {
	std::string label1 = pMatrix1->getLabel();
	std::string label2 = pMatrix2->getLabel();

	int seqLength1 = label1.size();
	int seqLength2 = label2.size();

	if(seqLength1 < 1 || seqLength2 < 1) {
		return true;
	}

	char lastGateLabel1 = label1[seqLength1 - 1];
	char firstGateLabel2 = label2[0];

	//Apply the rule presenting inverse gates by corresponding lower-upper case
	return !(lastGateLabel1 != firstGateLabel2 && toupper(lastGateLabel1) == toupper(firstGateLabel2));
}




