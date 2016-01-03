/*
 * DictionaryOrderCoordinateComparator.hpp
 *
 *  Created on: Dec 7, 2015
 *      Author: pham
 */

#ifndef DICTIONARYORDERCOORDINATECOMPARATOR_HPP_
#define DICTIONARYORDERCOORDINATECOMPARATOR_HPP_

#include "Coordinate.hpp"
#include "IElementComparator.h"
#include "AlgoInternal.h"

template<typename T>
class DictionaryOrderCoordinateComparator: public IElementComparator<RealCoordinate<T> > {
public:
	//Override
	bool lessThan(const RealCoordinate<T>& lhs, const RealCoordinate<T>& rhs);
};

template<typename T>
bool DictionaryOrderCoordinateComparator<T>::lessThan(const RealCoordinate<T>& lhs, const RealCoordinate<T>& rhs) {
	const std::vector<mreal_t>& lCoords = lhs.getCoordinates();
	const std::vector<mreal_t>& rCoords = rhs.getCoordinates();

	size_t coordSize = lCoords.size();

	for(unsigned int i = 0; i < coordSize; i++) {
		if(lCoords[i] < rCoords[i]) {
			return true;
		}
		if(rCoords[i] < lCoords[i]) {
			return false;
		}
	}

	return false;
}



#endif /* DICTIONARYORDERCOORDINATECOMPARATOR_HPP_ */
