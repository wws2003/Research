/*
 * ISearchSpaceConstructor.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef ISEARCHSPACECONSTRUCTOR_H_
#define ISEARCHSPACECONSTRUCTOR_H_

#include "AlgoCommon.h"
#include "MathConceptsCommon.h"

class ISearchSpaceConstructor {
public:
	virtual ~ISearchSpaceConstructor(){};

	virtual void constructSearchSpace(MatrixCollectionPtr pMatrixCollection, MatrixCollectionPtr pUniversalSet, int maxSequenceLength) = 0;
};


#endif /* ISEARCHSPACECONSTRUCTOR_H_ */
