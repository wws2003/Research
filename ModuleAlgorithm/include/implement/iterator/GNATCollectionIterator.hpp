/*
 * GNATCollectionIterator.hpp
 *
 *  Created on: Jun 28, 2015
 *      Author: pham
 */

#ifndef GNATCOLLECTIONITERATOR_HPP_
#define GNATCOLLECTIONITERATOR_HPP_

#include "GNATCollectionImpl.h"

template<typename T>
class GNATCollectionIterator: public IIterator<T> {
public:
	GNATCollectionIterator(GNATCollectionImpl<T>* pCollection);

	virtual ~GNATCollectionIterator(){};

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
	bool isCurrentSubCollectionDone();

	void nextInCurrentSubCollection();

	bool isAllSubCollectionsDone();

	void nextCollection();

	bool isCollectionUnStructuredBufferDone();

	void nextInUnStructuredBuffer();

	bool isCollectionNoSplitPointsDone();

	void nextInSplitPoints();

	enum BrowsingState {
		BROWSING_SUB_COLLECTION = 1,
		BROWSING_UNSTRUCTURED_BUFFER = 2,
		BROWSING_SPLIT_POINTS = 3
	};

	GNATCollectionImpl<T>* m_pCollection;

	int m_browsingSubCollectionIndex;
	IteratorPtr<T> m_pBrowsingSubCollectionIter;

	typename UnstructuredBuffer<T>::iterator m_browsingUnStructuredBufferIter;

	typename SplitPointSet<T>::iterator m_browsingSplitPointsIter;

	BrowsingState m_browsingState;
};

template<typename T>
GNATCollectionIterator<T>::GNATCollectionIterator(GNATCollectionImpl<T>* pCollection) {
	m_pCollection = pCollection;
	toBegin();
}

template<typename T>
void GNATCollectionIterator<T>::toBegin() {
	m_browsingSubCollectionIndex = 0;

	m_pBrowsingSubCollectionIter = m_pCollection->m_subCollections.size() > 0 ? m_pCollection->m_subCollections[0]->getIteratorPtr() : NullPtr;
	m_browsingUnStructuredBufferIter = m_pCollection->m_unStructeredBuffer.begin();
	m_browsingSplitPointsIter = m_pCollection->m_splitPoints.begin();

	while(isCurrentSubCollectionDone() && !isAllSubCollectionsDone()) {
		nextCollection();
	}

	if(!(isAllSubCollectionsDone() && isCurrentSubCollectionDone())) {
		m_browsingState = BROWSING_SUB_COLLECTION;
	}
	else {
		if(!isCollectionUnStructuredBufferDone()) {
			m_browsingState = BROWSING_UNSTRUCTURED_BUFFER;
		}
		else {
			m_browsingState = BROWSING_SPLIT_POINTS;
		}
	}

}

template<typename T>
void GNATCollectionIterator<T>::next() {
	BrowsingState nextState = m_browsingState;

	switch(m_browsingState) {

	case BROWSING_SUB_COLLECTION:
		if(!isCurrentSubCollectionDone()) {
			nextInCurrentSubCollection();
		}

		while(isCurrentSubCollectionDone() && !isAllSubCollectionsDone()) {
			nextCollection();
		}

		if(isAllSubCollectionsDone() && isCurrentSubCollectionDone()) {
			if(!isCollectionUnStructuredBufferDone()) {
				nextState = BROWSING_UNSTRUCTURED_BUFFER;
			}
			else {
				nextState = BROWSING_SPLIT_POINTS;
			}
		}

		break;

	case BROWSING_UNSTRUCTURED_BUFFER:
		if(!isCollectionUnStructuredBufferDone()) {
			nextInUnStructuredBuffer();
		}
		else {
			nextState = BROWSING_SPLIT_POINTS;
		}
		break;

	case BROWSING_SPLIT_POINTS:
		if(!isCollectionNoSplitPointsDone()) {
			nextInSplitPoints();
		}
		break;
	}

	m_browsingState = nextState;
}

template<typename T>
void GNATCollectionIterator<T>::prev() {
	//TODO Implement
}

template<typename T>
bool GNATCollectionIterator<T>::isDone() {
	bool result = false;
	switch(m_browsingState) {

	case BROWSING_SUB_COLLECTION:
		result = isCurrentSubCollectionDone() && isAllSubCollectionsDone() && isCollectionUnStructuredBufferDone() && isCollectionNoSplitPointsDone();
		break;

	case BROWSING_UNSTRUCTURED_BUFFER:
		result = isCollectionUnStructuredBufferDone() && isCollectionNoSplitPointsDone();
		break;

	case BROWSING_SPLIT_POINTS:
		result = isCollectionNoSplitPointsDone();
		break;

	default:
		break;
	}

	return result;
}

template<typename T>
T GNATCollectionIterator<T>::getObj() {

	T element = NullPtr;
	switch(m_browsingState) {

	case BROWSING_SUB_COLLECTION:
		element = m_pBrowsingSubCollectionIter->getObj();
		break;

	case BROWSING_UNSTRUCTURED_BUFFER:
		element = *m_browsingUnStructuredBufferIter;
		break;

	case BROWSING_SPLIT_POINTS:
		element = *m_browsingSplitPointsIter;
		break;

	default:
		return NullPtr;
	}

	assert(element != NullPtr);

	return element;
}

template<typename T>
bool GNATCollectionIterator<T>::isCurrentSubCollectionDone() {
	return m_pBrowsingSubCollectionIter == NullPtr || m_pBrowsingSubCollectionIter->isDone();
}

template<typename T>
void GNATCollectionIterator<T>::nextInCurrentSubCollection() {
	m_pBrowsingSubCollectionIter->next();
}

template<typename T>
bool GNATCollectionIterator<T>::isAllSubCollectionsDone() {
	return m_browsingSubCollectionIndex + 1 >= m_pCollection->m_subCollections.size();
}

template<typename T>
void GNATCollectionIterator<T>::nextCollection() {
	m_browsingSubCollectionIndex++;
	m_pBrowsingSubCollectionIter = m_pCollection->m_subCollections[m_browsingSubCollectionIndex]->getIteratorPtr();
}

template<typename T>
bool GNATCollectionIterator<T>::isCollectionUnStructuredBufferDone() {
	return m_browsingUnStructuredBufferIter == m_pCollection->m_unStructeredBuffer.end();
}

template<typename T>
void GNATCollectionIterator<T>::nextInUnStructuredBuffer() {
	m_browsingUnStructuredBufferIter++;
}

template<typename T>
bool GNATCollectionIterator<T>::isCollectionNoSplitPointsDone() {
	return m_browsingSplitPointsIter == m_pCollection->m_splitPoints.end();
}

template<typename T>
void GNATCollectionIterator<T>::nextInSplitPoints() {
	m_browsingSplitPointsIter++;
}
#endif /* GNATCOLLECTIONITERATOR_HPP_ */
