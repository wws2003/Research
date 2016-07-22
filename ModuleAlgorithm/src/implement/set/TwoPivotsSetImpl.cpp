/*
 * TwoPivotsSetImpl.cpp
 *
 *  Created on: Jul 17, 2016
 *      Author: pham
 */

#include "TwoPivotsSetImpl.h"

#define DISTANCE_TO_CONSIDER_AS_ONE (1e-7)

template<typename T>
TwoPivotsSetImpl<T>::TwoPivotsSetImpl(DistanceCalculatorPtr<T> pDistanceCalculator) {
	m_pivot1 = NullPtr;
	m_pivot2 = NullPtr;
	m_pDistanceCalculator = pDistanceCalculator;
}

template<typename T>
TwoPivotsSetImpl<T>::~TwoPivotsSetImpl() {
	for(typename ElementDistance2DTable::iterator dIter = m_distanceTable.begin(); dIter != m_distanceTable.end();) {
		ElementDistanceMapPtr psubMap = dIter->second;
		for(typename ElementDistanceMap::iterator dSubIter = psubMap->begin(); dSubIter != psubMap->end();) {
			std::vector<T>* pElementVector = dSubIter->second;
			_destroy(pElementVector);
			dSubIter = psubMap->erase(dSubIter);
		}
		_destroy(psubMap);
		dIter = m_distanceTable.erase(dIter);
	}
}

template<typename T>
bool TwoPivotsSetImpl<T>::isUnique(T seqElement) {
	if(!arePivotReady()) {
		return true;
	}
	mreal_t distanceToPivot1;
	mreal_t distanceToPivot2;

	distanceToPivots(seqElement, distanceToPivot1, distanceToPivot2);

	mreal_t eps = 3e-7;

	typename ElementDistance2DTable::const_iterator dLowerIter = m_distanceTable.lower_bound(distanceToPivot1 - eps);
	typename ElementDistance2DTable::const_iterator dUpperIter2 = m_distanceTable.upper_bound(distanceToPivot1 + eps);

	int distanceCalculateCounter = 0;
	for(auto dIter = dLowerIter; dIter != dUpperIter2; dIter++) {
		ElementDistanceMapPtr pSubMap = dIter->second;

		typename ElementDistanceMap::const_iterator dSubLowerIter = pSubMap->lower_bound(distanceToPivot2 - eps);
		typename ElementDistanceMap::const_iterator dSubUpperIter = pSubMap->upper_bound(distanceToPivot2 + eps);

		for(auto dSubIter = dSubLowerIter; dSubIter != dSubUpperIter; dSubIter++) {
			std::vector<T>* pElementsWithSameDistanceToPivot = dSubIter->second;

			for(T element : *pElementsWithSameDistanceToPivot) {
				distanceCalculateCounter++;
				//Check if there is one element too close to the target element so that can deal as one
				if(m_pDistanceCalculator->distance(element, seqElement) <= DISTANCE_TO_CONSIDER_AS_ONE) {
					return false;
				}
			}
		}
	}

	/*std::cout << "---For " << pSeqGate->getLabelStr()
			//<< " distance to pivot 1 = " << mreal::toDouble(distanceToPivot1)
			//<< " distance to pivot 2 = " << mreal::toDouble(distanceToPivot2)
			<< " Number of distance to calculated " << distanceCalculateCounter << "\n";
	 */
	return true;
}

template<typename T>
void TwoPivotsSetImpl<T>::addElement(T seqElement) {
	if(!arePivotReady()) {
		updatePivots(seqElement);
		if(arePivotReady()) {
			addElementWhenPivotsReady(m_pivot1);
			addElementWhenPivotsReady(m_pivot2);
		}
	}
	else {
		addElementWhenPivotsReady(seqElement);
	}
}

template<typename T>
void TwoPivotsSetImpl<T>::getAllUniqueElements(std::vector<T>& elements) {
	elements.clear();
	for(typename ElementDistance2DTable::iterator dIter = m_distanceTable.begin(); dIter != m_distanceTable.end(); dIter++) {
		ElementDistanceMapPtr psubMap = dIter->second;
		for(typename ElementDistanceMap::iterator dSubIter = psubMap->begin(); dSubIter != psubMap->end(); dSubIter++) {
			std::vector<T>* pElementVector = dSubIter->second;
			for(T element : *pElementVector) {
				elements.push_back(element);
			}
		}
		_destroy(psubMap);
		dIter = m_distanceTable.erase(dIter);
	}
}

template<typename T>
bool TwoPivotsSetImpl<T>::arePivotReady() const {
	return m_pivot1 != NullPtr && m_pivot2 != NullPtr;
}

template<typename T>
void TwoPivotsSetImpl<T>::distanceToPivots(T element, mreal_t& d1, mreal_t& d2) const {
	d1 = element == m_pivot1 ? 0.0 : m_pDistanceCalculator->distance(element, m_pivot1);
	d2 = element == m_pivot2 ? 0.0 : m_pDistanceCalculator->distance(element, m_pivot2);
}

template<typename T>
void TwoPivotsSetImpl<T>::addElementWhenPivotsReady(T element) {
	mreal_t distanceToPivot1;
	mreal_t distanceToPivot2;

	distanceToPivots(element, distanceToPivot1, distanceToPivot2);

	typename ElementDistance2DTable::iterator dIter = m_distanceTable.find(distanceToPivot1);

	std::vector<T>* pElementsWithSameDistanceToPivot2 = NullPtr;

	if(dIter == m_distanceTable.end()) {
		ElementDistanceMapPtr pSubMap = new ElementDistanceMap();
		pElementsWithSameDistanceToPivot2 = new std::vector<T>();
		pSubMap->insert(std::pair<mreal_t, std::vector<T>* >(distanceToPivot2, pElementsWithSameDistanceToPivot2));
		m_distanceTable.insert(std::pair<mreal_t, ElementDistanceMapPtr>(distanceToPivot1, pSubMap));
	}
	else {
		ElementDistanceMapPtr pSubMap = dIter->second;
		typename ElementDistanceMap::iterator dSubIter = pSubMap->find(distanceToPivot2);
		if(dSubIter == pSubMap->end()) {
			pElementsWithSameDistanceToPivot2 = new std::vector<T>();
			pSubMap->insert(std::pair<mreal_t, std::vector<T>* >(distanceToPivot2, pElementsWithSameDistanceToPivot2));
		}
		else {
			pElementsWithSameDistanceToPivot2 = dSubIter->second;
		}
	}

	pElementsWithSameDistanceToPivot2->push_back(element);
}

template<typename T>
void TwoPivotsSetImpl<T>::updatePivots(T pivot) {
	if(m_pivot1 == NullPtr) {
		m_pivot1 = pivot;
		return;
	}
	if(m_pivot2 == NullPtr) {
		m_pivot2 = pivot;
		return;
	}
}


