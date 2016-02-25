/*
 * SortedVectorArray.hpp
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#ifndef SORTEDVECTORARRAY_HPP_
#define SORTEDVECTORARRAY_HPP_

#include "IElementComparator.h"
#include "AlgoCommon.h"
#include "AlgoInternal.h"

template<typename T>
class SortedVectorArray {
public:
	SortedVectorArray(WrapperComparator<T>& comparator) : m_wrapperComparator(comparator){};

	//Init vectors fields
	void initByVectors(const BuildingBlockBuckets<T>& buildingBlockBuckets);

	//Init accumulated values
	void initMaxMinAccumulatedValues();

	/**
	 * Return maximum element can be retrieved by accumulating elements
	 *  in vectors (one element each vector) upto lastVectorIndex
	 *
	 */
	const T& getMaxAccumulated(int lastVectorIndex) const;

	/**
	 * Return minimum element can be retrieved by accumulating elements
	 *  in vectors (one element each vector) upto lastVectorIndex
	 *
	 */
	const T& getMinAccumulated(int lastVectorIndex) const;

	/**
	 * Return lower and upper bound when looking up target in [min, max] on vector indexed by vector index
	 */
	void getRangeInVector(int vectorIndex, T min, T max, int& lowerBound, int& upperBound) const;

	//Retrieve immutable vector by vector index
	const std::vector<T>& getVector(int vectorIndex) const;

	int getNbVectors() const;

private:
	void printDebugInfo() const;

	WrapperComparator<T>& m_wrapperComparator;

	std::vector<std::vector<T> > m_vectors;
	std::vector<T> m_minAccumulated;
	std::vector<T> m_maxAccumulated;

};

template<typename T>
void SortedVectorArray<T>::initByVectors(const BuildingBlockBuckets<T>& buildingBlockBuckets) {
	size_t nbVector = buildingBlockBuckets.size();

	m_vectors.reserve(nbVector);
	m_maxAccumulated.reserve(nbVector);
	m_minAccumulated.reserve(nbVector);

	//Add elements to vectors
	//The counter to calculate how many combinations need to check in the worst case
	combination_counter_t nbMaximumCombination = 1;
	for(unsigned int i = 0; i < nbVector; i++) {
		IteratorPtr<T> pBuildingBlockIter = buildingBlockBuckets[i];
		std::vector<T> buildingBlockVector;

		if(pBuildingBlockIter != NullPtr) {
			while(!pBuildingBlockIter->isDone()) {
				buildingBlockVector.push_back(pBuildingBlockIter->getObj());
				pBuildingBlockIter->next();
			}
			pBuildingBlockIter->toBegin();
		}
		m_vectors.push_back(buildingBlockVector);//FIXME: No way to reserve size ?

		nbMaximumCombination *= buildingBlockVector.size();
	}

	//Sort vectors
	for(unsigned int j = 0; j < nbVector; j++) {
		std::sort(m_vectors[j].begin(), m_vectors[j].end(), m_wrapperComparator);
	}

	//Calculate max, min accumulated
	initMaxMinAccumulatedValues();

	//For debug purpose, print out accumlated max, min
	printDebugInfo();
}

template<typename T>
void SortedVectorArray<T>::initMaxMinAccumulatedValues() {
	m_maxAccumulated.push_back(m_vectors[0][m_vectors[0].size() - 1]);
	m_minAccumulated.push_back(m_vectors[0][0]);

	size_t nbVectorSet = m_vectors.size();
	for(unsigned int j = 1; j < nbVectorSet; j++) {
		T maxAcc = m_maxAccumulated[j - 1];
		T minAcc = m_minAccumulated[j - 1];
		if(!m_vectors[j].empty()) {
			maxAcc = m_vectors[j][m_vectors[j].size() - 1] + m_maxAccumulated[j - 1];
			minAcc = m_vectors[j][0] + m_minAccumulated[j - 1];
		}
		m_maxAccumulated.push_back(maxAcc);
		m_minAccumulated.push_back(minAcc);
	}
}

template<typename T>
const T& SortedVectorArray<T>::getMaxAccumulated(int lastVectorIndex) const {
	return m_maxAccumulated[lastVectorIndex];
}

template<typename T>
const T& SortedVectorArray<T>::getMinAccumulated(int lastVectorIndex) const{
	return m_minAccumulated[lastVectorIndex];
}

template<typename T>
void SortedVectorArray<T>::getRangeInVector(int vectorIndex,
		T min,
		T max,
		int& lowerBound,
		int& upperBound) const {

	const std::vector<T>& vector = m_vectors[vectorIndex];
	typename std::vector<T>::const_iterator lowerIter = std::lower_bound(vector.begin(), vector.end(), min, m_wrapperComparator);

	if(lowerIter == vector.end()) {
		lowerBound = vector.size();
		upperBound = vector.size();
		return;
	}
	lowerBound = lowerIter - vector.begin();

	typename std::vector<T>::const_iterator upperIter = std::upper_bound(vector.begin(), vector.end(), max, m_wrapperComparator);

	if(upperIter != vector.end()) {
		upperBound = upperIter - vector.begin();
	}
	else {
		upperBound = vector.size();
	}
}

template<typename T>
const std::vector<T>& SortedVectorArray<T>::getVector(int vectorIndex) const{
	return m_vectors[vectorIndex];
}

template<typename T>
int SortedVectorArray<T>::getNbVectors() const{
	return m_vectors.size();
}

template<typename T>
void SortedVectorArray<T>::printDebugInfo() const {
#ifdef ABCE_DEBUG
	size_t nbVectorSet = m_vectors.size();

	auto printCoordFunc = [](real_coordinate_t coords) {
		std::cout << "-Coordinate:\n  ";
		for(mreal_t coord : coords) {
			std::cout << coord << " ";
		}
		std::cout << "\n";
	};
	//Print all vectors in every set
	for(unsigned int i = 0; i < nbVectorSet; i++) {
		std::cout << "Vector set " << i << "\n";
		std::for_each(m_vectors[i].begin(), m_vectors[i].end(), [&printCoordFunc](T coordElement) {
			printCoordFunc(coordElement.getCoordinates());
		}
		);
		std::cout << "--------------------\n";
	}
	for(unsigned int i = 0; i < nbVectorSet; i++) {
		std::cout << "Accumulated value upto vector set " << i << "\n";
		std::cout << "Accumulated max:\n";
		printCoordFunc(m_maxAccumulated[i].getCoordinates());
		std::cout << "Accumulated min:\n";
		printCoordFunc(m_minAccumulated[i].getCoordinates());
		std::cout << "--------------------\n";
	}
#endif
}

template<typename T>
using SortedVectorArrayPtr = SortedVectorArray<T>*;

template<typename T>
using SortedVectorArrayList = std::vector<SortedVectorArrayPtr<T> > ;

#endif /* SORTEDVECTORARRAY_HPP_ */
