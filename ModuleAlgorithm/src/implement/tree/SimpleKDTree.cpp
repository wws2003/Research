/*
 * SimpleKDTree.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#include "SimpleKDTree.h"

template<typename T, typename F>
SimpleKDTree<T,F>::SimpleKDTree(int dimension, F rootCoord) {
	//Set a default root
	std::vector<F> coords(dimension, rootCoord);
	m_dataCoord = Coordinate<T, F>(NullPtr, coords);
	m_compareIndex = 0;
	m_pLeftChild = NULL;
	m_pRightChild = NULL;
}

template<typename T, typename F>
SimpleKDTree<T,F>::SimpleKDTree(Coordinate<T, F> dataCoord, int compareIndex) {
	m_dataCoord = dataCoord;
	m_compareIndex = compareIndex;
	m_pLeftChild = NULL;
	m_pRightChild = NULL;
}

template<typename T, typename F>
SimpleKDTree<T,F>::~SimpleKDTree() {
	init();
}

template<typename T, typename F>
void SimpleKDTree<T,F>::init() {
	if(m_pLeftChild != NULL) {
		delete m_pLeftChild;
	}
	if(m_pRightChild != NULL) {
		delete m_pRightChild;
	}
}

template<typename T, typename F>
void SimpleKDTree<T,F>::insert(Coordinate<T, F> coordinate) {
	unsigned int nbDimension = m_dataCoord.getCoordinates().size();//Suppose this value is fixed
	F valueToCompare = coordinate.getCoordinates()[m_compareIndex];
	F nodeValueToCompare = m_dataCoord.getCoordinates()[m_compareIndex];

	SimpleKDTree** ppChild;
	ppChild = (valueToCompare <= nodeValueToCompare) ? &m_pLeftChild : &m_pRightChild;

	if(*ppChild == NULL) {
		*ppChild = new SimpleKDTree(coordinate, (m_compareIndex + 1) % nbDimension);
	}
	else {
		(*ppChild)->insert(coordinate);
	}
}

template<typename T, typename F>
void SimpleKDTree<T,F>::query(const Coordinate<T, F>& query,
		const std::vector<F>& ranges,
		std::vector<Coordinate<T, F> >& results) {

	if(isMatched(query, ranges)) {
		results.push_back(m_dataCoord);
	}

	F lowerValueToCompare = query.getCoordinates()[m_compareIndex] - ranges[m_compareIndex];
	F upperValueToCompare = query.getCoordinates()[m_compareIndex] + ranges[m_compareIndex];
	F nodeLowerValueToCompare = m_dataCoord.getCoordinates()[m_compareIndex];

	if(m_pLeftChild != NULL && lowerValueToCompare <= nodeLowerValueToCompare) {
		m_pLeftChild->query(query, ranges, results);
	}

	if(m_pRightChild != NULL && upperValueToCompare > nodeLowerValueToCompare) {
		m_pRightChild->query(query, ranges, results);
	}
}

template<typename T, typename F>
bool SimpleKDTree<T,F>::isMatched(const Coordinate<T, F>& query, const std::vector<F>& ranges) {
	unsigned int nbDimension = m_dataCoord.getCoordinates().size();
	std::vector<F> nodeCoords = m_dataCoord.getCoordinates();
	std::vector<F> queryCoords = query.getCoordinates();

	for(unsigned int i = 0; i < nbDimension; i++) {
		if(nodeCoords[i] < queryCoords[i] - ranges[i] || nodeCoords[i] > queryCoords[i] + ranges[i]) {
			return false;
		}
	}
	return true;
}


