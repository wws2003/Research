/*
 * OneElementCoordinateComparator.hpp
 *
 *  Created on: Feb 12, 2016
 *      Author: pham
 */

#ifndef ONEELEMENTCOORDINATECOMPARATOR_HPP_
#define ONEELEMENTCOORDINATECOMPARATOR_HPP_

#include "IElementComparator.h"
#include "Coordinate.hpp"
#include <iostream>

//#define DEBUG_ONE_ELEMENT

template<typename T>
class OneElementCoordinateComparator: public IElementComparator<RealCoordinate<T> > {
public:
	OneElementCoordinateComparator(int comparedElementIndex) : m_comparedElementIndex(comparedElementIndex){};

	//Override
	bool lessThan(const RealCoordinate<T>& lhs, const RealCoordinate<T>& rhs);

private:
	int m_comparedElementIndex;
};

template<typename T>
bool OneElementCoordinateComparator<T>::lessThan(const RealCoordinate<T>& lhs, const RealCoordinate<T>& rhs) {
	const real_coordinate_t& lCoords = lhs.getCoordinates();
	const real_coordinate_t& rCoords = rhs.getCoordinates();

#ifdef DEBUG_ONE_ELEMENT
	//For debug
	std::cout << "-LCoord coordinates :\n  ";
	for(mreal_t coord : lCoords) {
		std::cout << coord << " ";
	}
	std::cout << "\n";

	std::cout << "-RCoord coordinates :\n  ";
	for(mreal_t coord : rCoords) {
		std::cout << coord << " ";
	}
	std::cout << "\n";
	std::cout << "Compare result on index " << m_comparedElementIndex << ": " << (lCoords[m_comparedElementIndex] < rCoords[m_comparedElementIndex]) << "\n";
	std::cout << "---------------------\n";
#endif

	return lCoords[m_comparedElementIndex] < rCoords[m_comparedElementIndex];
}



#endif /* ONEELEMENTCOORDINATECOMPARATOR_HPP_ */
