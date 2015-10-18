/*
 * IDecomposer.h
 *
 *  Created on: Oct 18, 2015
 *      Author: pham
 */

#ifndef IDECOMPOSER_H_
#define IDECOMPOSER_H_

#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include <vector>

template<typename T>
class IDecomposer {
public:
	virtual ~IDecomposer(){};

	/**
	 *Decompose an element (a gate / a matrix) into a number of sub-elements.
	 *For instace GC-decompose by Dawson in SK algorithm
	 */
	virtual void decomposeElement(T element, std::vector<T>& subElements, int nbSubElements = 0);
};



#endif /* IDECOMPOSER_H_ */
