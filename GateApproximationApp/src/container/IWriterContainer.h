/*
 * IWriterContainer.h
 *
 *  Created on: Oct 26, 2015
 *      Author: pham
 */

#ifndef IWRITERCONTAINER_H_
#define IWRITERCONTAINER_H_

#include "IWriter.h"
#include "IOCommon.h"
#include "Gate.h"

class IWriterContainer {
public:
	virtual ~IWriterContainer(){};

	virtual GateWriterPtr getGateWriter() = 0;
};



#endif /* IWRITERCONTAINER_H_ */
