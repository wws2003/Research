/*
 * ISearchSpaceEvaluator.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef ISEARCHSPACEEVALUATOR_H_
#define ISEARCHSPACEEVALUATOR_H_

#include "AlgoCommon.h"

class ISearchSpaceEvaluator {
public:
	virtual void evaluate(MatrixCollectionPtr pMatrixCollection) = 0;

	virtual ~ISearchSpaceEvaluator(){};

};


#endif /* ISEARCHSPACEEVALUATOR_H_ */
