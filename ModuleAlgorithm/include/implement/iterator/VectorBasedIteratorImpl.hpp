/*
 * VectorBasedIteratorImpl.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef VECTORBASEDITERATORIMPL_H_
#define VECTORBASEDITERATORIMPL_H_

#include "IIterator.h"
#include "AlgoCommon.h"
#include <vector>

template<typename T>
class VectorBasedIteratorImpl: public IIterator<T> {
public:
	VectorBasedIteratorImpl(std::vector<T>* pElements);

	virtual ~VectorBasedIteratorImpl(){};

	//Go to the beginning (point to first element)
	virtual void toBegin();

	//Go to next element pointer
	virtual void next();

	//Go to previous pointer
	virtual void prev();

	//Check if the last element has been checked
	virtual bool isDone();

	//Get currently pointed element
	virtual T getObj();

private:
	std::vector<T>* m_pElements;
	int m_counter;
};

template<typename T>
VectorBasedIteratorImpl<T>::VectorBasedIteratorImpl(std::vector<T>* pElements) {
	m_pElements->insert(m_pElements->end(), pElements->begin(), pElements->end());
	toBegin();
}

//Go to the beginning (point to first element)
template<typename T>
void VectorBasedIteratorImpl<T>::toBegin() {
	m_counter = 0;
}

//Go to next element pointer
template<typename T>
void VectorBasedIteratorImpl<T>::next() {
	m_counter++;
}

//Go to previous pointer
template<typename T>
void VectorBasedIteratorImpl<T>:: prev() {
	m_counter--;
}

//Check if the last element has been checked]
template<typename T>
bool VectorBasedIteratorImpl<T>::isDone() {
	return (m_counter > m_pElements->size() - 1);
}

//Get currently pointed element
template<typename T>
T VectorBasedIteratorImpl<T>::getObj() {
	return (*m_pElements)[m_counter];
}


#endif /* VECTORBASEDITERATORIMPL_H_ */
