/*
 * DummyElementDecomposer.h
 *
 *  Created on: Oct 18, 2015
 *      Author: pham
 */

#ifndef DUMMYELEMENTDECOMPOSER_H_
#define DUMMYELEMENTDECOMPOSER_H_

#include "IDecomposer.h"
#include "AlgoCommon.h"

template<typename T>
class DummyElementDecomposer : public IDecomposer<T> {
public:
	DummyElementDecomposer(){};
	virtual ~DummyElementDecomposer(){};

	//Override
	virtual void decomposeElement(T element,
			std::vector<T>& subElements,
			int nbSubElements = 0);
};




#endif /* DUMMYELEMENTDECOMPOSER_H_ */
