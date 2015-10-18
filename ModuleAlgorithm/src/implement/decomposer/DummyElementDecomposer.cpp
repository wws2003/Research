/*
 * DummyElementDecomposer.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: pham
 */

#include "DummyElementDecomposer.h"

template<typename T>
void DummyElementDecomposer<T>::decomposeElement(T element,
		std::vector<T>& subElements,
		int nbSubElements) {
	//Just return the element itself in a 1-element vector
	subElements.clear();
	subElements.push_back(element);
}


