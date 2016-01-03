/*
 * SumCoordinateComparator.hpp
 *
 *  Created on: Dec 7, 2015
 *      Author: pham
 */

#ifndef SUMCOORDINATECOMPARATOR_HPP_
#define SUMCOORDINATECOMPARATOR_HPP_

#include "Coordinate.hpp"
#include "IElementComparator.h"
#include "AlgoInternal.h"

template<typename T>
class SumCoordinateComparator: public IElementComparator<RealCoordinate<T> > {
public:
	//Override
	bool lessThan(const RealCoordinate<T>& lhs, const RealCoordinate<T>& rhs);
};

template<typename T>
bool SumCoordinateComparator<T>::lessThan(const RealCoordinate<T>& lhs, const RealCoordinate<T>& rhs) {
	const std::vector<mreal_t>& lCoords = lhs.getCoordinates();
	const std::vector<mreal_t>& rCoords = rhs.getCoordinates();

	mreal_t lCoordSum = 0.0;
	mreal_t rCoordSum = 0.0;

	size_t coordSize = lCoords.size();

	for(unsigned int i = 0; i < coordSize; i++) {
		lCoordSum += lCoords[i];
		rCoordSum += rCoords[i];
	}
	return lCoordSum < rCoordSum;
}


#endif /* SUMCOORDINATECOMPARATOR_HPP_ */
