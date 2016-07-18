/*
 * AbstractTreeBasedSet.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#include "AbstractTreeBasedSet.h"
#include <iostream>

template<typename T, typename F>
AbstractTreeBasedSet<T,F>::AbstractTreeBasedSet(const std::vector<T>& pivots,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		mreal_t distanceToConsiderAsOne,
		RangeSearchTreePtr<T, F> pRangeSearchTree,
		const std::vector<F>& coordinateRanges) {

	m_pivots = pivots;
	m_pDistanceCalculator = pDistanceCalculator;
	m_distanceToConsiderAsOne = distanceToConsiderAsOne;
	m_pRangeSearchTree = pRangeSearchTree;
	m_coordinateRanges = coordinateRanges;
	m_distanceCalculateCounter = 0;
	m_uniqueCheckCounter = 0;
}


template<typename T, typename F>
AbstractTreeBasedSet<T,F>::~AbstractTreeBasedSet() {
	std::cout << "----Check unique " << m_uniqueCheckCounter << " cases\n";
	std::cout << "----Calculate " << m_distanceCalculateCounter << " distances\n";
}

template<typename T, typename F>
bool AbstractTreeBasedSet<T,F>::isUnique(T element) {
	m_uniqueCheckCounter++;
	//Currently don't have mechanism to remember result

	//Calculate query coordinate ranges. Consider coords = {targets to pivots}
	std::vector<F> distancesToPivots;
	calculateDistancesToPivots(element, distancesToPivots);
	Coordinate<T,F> coord(element, distancesToPivots);

	//Find elements in ranges
	std::vector<Coordinate<T,F> > nearCoordinates;
	m_pRangeSearchTree->query(coord, m_coordinateRanges, nearCoordinates);

	//Check elements in ranged coordinates
	for(Coordinate<T,F> rangedCoord : nearCoordinates) {
		T rangedElement = rangedCoord.getElement();
		m_distanceCalculateCounter++;
		if(m_pDistanceCalculator->distance(element, rangedElement) <= m_distanceToConsiderAsOne) {
			return false;
		}
	}

	return true;
}

template<typename T, typename F>
void AbstractTreeBasedSet<T,F>::addElement(T element) {
	//Calculate query coordinate ranges
	std::vector<F> distancesToPivots;
	calculateDistancesToPivots(element, distancesToPivots);
	Coordinate<T,F> coord(element, distancesToPivots);

	m_pRangeSearchTree->insert(coord);
}

template<typename T, typename F>
void AbstractTreeBasedSet<T,F>::getAllUniqueElements(std::vector<T>& elements) {
	//TODO Implement
}

