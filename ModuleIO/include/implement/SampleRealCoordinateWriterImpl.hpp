/*
 * SampleRealCoordinateWriterImpl.hpp
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#ifndef SAMPLEREALCOORDINATEWRITERIMPL_HPP_
#define SAMPLEREALCOORDINATEWRITERIMPL_HPP_

#include "ICoordinateWriter.h"
#include "Coordinate.hpp"

template<typename T>
class SampleRealCoordinateWriterImpl : public ICoordinateWriter<T, double>{
public:
	virtual ~SampleRealCoordinateWriterImpl(){};

	virtual void writeCoordinate(const Coordinate<T,double>& coordinate, std::ostream& outputStream);
};

template<typename T>
void SampleRealCoordinateWriterImpl<T>::writeCoordinate(const Coordinate<T,double>& coordinate, std::ostream& outputStream) {
	std::string delimeter = ",";
	for(std::vector<double>::const_iterator cIter = coordinate.getCoordinates().begin(); cIter != coordinate.getCoordinates().end(); cIter++) {
		outputStream << *cIter << delimeter ;
	}
	outputStream << std::endl;
}


#endif /* SAMPLEREALCOORDINATEWRITERIMPL_HPP_ */
