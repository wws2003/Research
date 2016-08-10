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

	//Calculate query coordinate ranges. Consider coords = {targets to pivots}
	std::vector<F> distancesToPivots;
	calculateDistancesToPivotsWithCache(element, distancesToPivots);
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
	calculateDistancesToPivotsWithCache(element, distancesToPivots);

	Coordinate<T,F> coord(element, distancesToPivots);

	m_pRangeSearchTree->insert(coord);
}

template<typename T, typename F>
void AbstractTreeBasedSet<T,F>::getAllUniqueElements(std::vector<T>& elements) {
	//TODO Implement
}

//MARK: Protected methods
template<typename T, typename F>
void AbstractTreeBasedSet<T,F>::calculateDistancesToPivotsWithCache(T element,
		std::vector<F>& distances) {

	std::string elementCacheKey = getElementDistanceCacheKey(element);
	if(m_distanceCache.checkCachedElement(elementCacheKey, distances)) {
		return;
	}
	else {
		calculateDistancesToPivots(element, distances);
		m_distanceCache.addToCache(elementCacheKey, distances);
	}
}

//MARK: Private class
template<typename T, typename F>
AbstractTreeBasedSet<T,F>::DistanceCache::DistanceCache() : m_rememeberedElementKey(""){

}

template<typename T, typename F>
bool AbstractTreeBasedSet<T,F>::DistanceCache::checkCachedElement(std::string elementKey, std::vector<F>& distances) {
	if(elementKey == m_rememeberedElementKey) {
		distances = m_rememberedDistances;
		return true;
	}
	return false;
}

template<typename T, typename F>
void AbstractTreeBasedSet<T,F>::DistanceCache::addToCache(std::string elementKey, const std::vector<F>& distances) {
	m_rememeberedElementKey = elementKey;
	m_rememberedDistances = distances;
}
