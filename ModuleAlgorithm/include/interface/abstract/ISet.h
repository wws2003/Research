/*
 * ISet.h
 *
 *  Created on: Jul 17, 2016
 *      Author: pham
 */

#ifndef ISET_H_
#define ISET_H_

//MARK: Possibly a simplified collection as a short hand

#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "MathConceptsCommon.h"
#include "IIterator.h"

template<typename T>
class IEqualChecker {
public:
	virtual ~IEqualChecker(){};

	virtual bool isEqual(const T& element1, const T& element2);
};

template<typename T>
class ISet {
public:
	virtual ~ISet(){};

	virtual bool isUnique(T element) = 0;

	virtual void addElement(T element) = 0;

	//Don't use iterator just for convenience
	virtual void getAllUniqueElements(std::vector<T>& elements) = 0;
};





#endif /* ISET_H_ */
