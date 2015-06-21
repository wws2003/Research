/*
 * ISearchSpaceEvaluator.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef ISEARCHSPACEEVALUATOR_H_
#define ISEARCHSPACEEVALUATOR_H_

#include "AlgoCommon.h"
#include "MathConceptsCommon.h"

template<typename T>
class ISearchSpaceEvaluator {
public:

	virtual ~ISearchSpaceEvaluator(){};

	/**
	 *Evaluate given matrix/gate collection, possibly about running time to find approximation, etc.
	 */
	virtual void evaluateCollection(CollectionPtr<T> pCollection) = 0;

	/**
	 *Evaluate given matrix/gate approximator, possibly about running time to get approximation, etc.
	 */
	virtual void evaluateApproximator(ApproximatorPtr<T> pApproximator, CollectionPtr<T> pCoreCollection) = 0;
};


#endif /* ISEARCHSPACEEVALUATOR_H_ */
