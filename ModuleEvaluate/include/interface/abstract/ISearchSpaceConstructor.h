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

template<typename T>
class ISearchSpaceConstructor {
public:
	virtual ~ISearchSpaceConstructor(){};

	/**
	 * Construct search space (matrix or gate collection) given
	 * - Collection of universal matrices/gates (such as H,T for SU(2))
	 * - Maximum sequence length of matrices in search space
	 */
	virtual void constructSearchSpace(CollectionPtr<T> pCollection, CollectionPtr<T> pUniversalSet, int maxSequenceLength) = 0;
};


#endif /* ISEARCHSPACECONSTRUCTOR_H_ */
