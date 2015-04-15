/*
 * ILabelable.h
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#ifndef ILABELABLE_H_
#define ILABELABLE_H_

#include <string>

class ILabelable {
public:
	virtual ~ILabelable(){};

	virtual std::string getLabel() = 0;
};


#endif /* ILABELABLE_H_ */
