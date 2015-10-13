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
class SampleRealCoordinateWriterImpl : public ICoordinateWriter<T, mreal_t>{
public:
	SampleRealCoordinateWriterImpl(std::string endStr = "\r\n");
	virtual ~SampleRealCoordinateWriterImpl(){};

	virtual void writeCoordinate(const Coordinate<T,mreal_t>& coordinate, std::ostream& outputStream);

private:
	std::string m_endStr;
};

template<typename T>
SampleRealCoordinateWriterImpl<T>::SampleRealCoordinateWriterImpl(std::string endStr) {
	m_endStr = endStr;
}

template<typename T>
void SampleRealCoordinateWriterImpl<T>::writeCoordinate(const Coordinate<T,mreal_t>& coordinate, std::ostream& outputStream) {
	std::string delimeter = ",";
	for(std::vector<mreal_t>::const_iterator cIter = coordinate.getCoordinates().begin(); cIter != coordinate.getCoordinates().end(); cIter++) {
		if(cIter == coordinate.getCoordinates().begin()) {
			outputStream << *cIter ;
		}
		else {
			outputStream << delimeter << *cIter ;
		}
	}
	outputStream << m_endStr;
}


#endif /* SAMPLEREALCOORDINATEWRITERIMPL_HPP_ */
