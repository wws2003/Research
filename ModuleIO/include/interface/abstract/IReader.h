/*
 * IReader.h
 *
 *  Created on: Jul 25, 2015
 *      Author: pham
 */

#ifndef IREADER_H_
#define IREADER_H_

#include "IOCommon.h"
#include <istream>

template<typename T>
class IReader {
public:
	virtual ~IReader(){};

	virtual void read(T& element, std::istream &inputStream) = 0;
};



#endif /* IREADER_H_ */
