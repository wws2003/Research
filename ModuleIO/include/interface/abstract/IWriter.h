/*
 * IWriter.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef IWRITER_H_
#define IWRITER_H_

#include "IOCommon.h"
#include <ostream>

template<typename T>
class IWriter {
public:
	virtual ~IWriter(){};

	virtual void write(T element, std::ostream& outputStream) = 0;
};



#endif /* IWRITER_H_ */
