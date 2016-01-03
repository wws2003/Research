/*
 * HarrowElementDecomposer.h
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#ifndef HARROWELEMENTDECOMPOSER_H_
#define HARROWELEMENTDECOMPOSER_H_

#include "IDecomposer.h"
#include "ICoordinateCalculator.h"
#include "OperatorCommon.h"
#include "Coordinate.hpp"

#define DEBUGGING 1

template<typename T>
class HarrowElementDecomposer: public IDecomposer<T> {
public:
	HarrowElementDecomposer(RealCoordinateCalculatorPtr<T> pCoordinateCalculator);
	virtual ~HarrowElementDecomposer(){};

	//Override
	void decomposeElement(T element, std::vector<T>& subElements, int nbSubElements = 0);

protected:
	//Abstract method inheritance may be just a temporary solution.
	//A composition approach should be more robust then inheritance
	virtual void getInverseElement(T element, T& inverseElement) = 0;

private:
	void calculateSphereCoord(T element, RealCoordinate<T>& sphCoord);

	void calculateElementFromSphereCoord(RealCoordinate<T>& sphCoord, T& element);

	void calculateFirstPartionSphereCoord(const RealCoordinate<T>& sphCoord, RealCoordinate<T>& firstPartitionSphCoord);

	void calculateSecondPartionSphereCoord(const RealCoordinate<T>& sphCoord, RealCoordinate<T>& secondPartitionSphCoord);

	void cart2Sph(const real_coordinate_t& cartCoord, real_coordinate_t& sphCoord);

	void sph2Cart(const real_coordinate_t& sphCoord, real_coordinate_t& cartCoord);

	RealCoordinateCalculatorPtr<T> m_pCoordinateCalculator;

#if DEBUGGING
void printCartFromSphCoord(std::string title, const real_coordinate_t& coord);
#endif
};



#endif /* HARROWELEMENTDECOMPOSER_H_ */
