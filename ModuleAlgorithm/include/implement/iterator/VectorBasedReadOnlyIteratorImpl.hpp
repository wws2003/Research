/*
 * VectorBasedReadOnlyIteratorImpl.h
 *
 *  Created on: Jun 16, 2015
 *      Author: pham
 */

#ifndef VECTORBASEDREADONLYITERATORIMPL_H_
#define VECTORBASEDREADONLYITERATORIMPL_H_

#include "IIterator.h"
#include "AlgoCommon.h"
#include <vector>

template<typename T>
class VectorBasedReadOnlyIteratorImpl: public IIterator<T> {
public:
	VectorBasedReadOnlyIteratorImpl(const std::vector<T>& elements);

	virtual ~VectorBasedReadOnlyIteratorImpl(){};

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
	std::vector<T> m_elements;
	int m_counter;
};

template<typename T>
VectorBasedReadOnlyIteratorImpl<T>::VectorBasedReadOnlyIteratorImpl(const std::vector<T>& elements) {
	m_elements.insert(m_elements.end(), elements.begin(), elements.end());
	toBegin();
}

//Go to the beginning (point to first element)
template<typename T>
void VectorBasedReadOnlyIteratorImpl<T>::toBegin() {
	m_counter = 0;
}

//Go to next element pointer
template<typename T>
void VectorBasedReadOnlyIteratorImpl<T>::next() {
	m_counter++;
}

//Go to previous pointer
template<typename T>
void VectorBasedReadOnlyIteratorImpl<T>:: prev() {
	m_counter--;
}

//Check if the last element has been checked]
template<typename T>
bool VectorBasedReadOnlyIteratorImpl<T>::isDone() {
	return (m_counter > m_elements.size() - 1);
}

//Get currently pointed element
template<typename T>
T VectorBasedReadOnlyIteratorImpl<T>::getObj() {
	return m_elements[m_counter];
}




#endif /* VECTORBASESITERATORIMPL_H_ */
