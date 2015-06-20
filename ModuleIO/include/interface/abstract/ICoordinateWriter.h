/*
 * ICoordinateWriter.h
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#ifndef ICOORDINATEWRITER_H_
#define ICOORDINATEWRITER_H_

#include "MathConceptsCommon.h"
#include <ostream>

template<typename T, typename F>
class ICoordinateWriter {
public:
	virtual ~ICoordinateWriter(){};

	virtual void writeCoordinate(const Coordinate<T,F>& coordinate, std::ostream& ouputStream) = 0;
};



#endif /* ICOORDINATEWRITER_H_ */
