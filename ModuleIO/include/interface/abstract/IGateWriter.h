/*
 * IGateWriter.h
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#ifndef IGATEWRITER_H_
#define IGATEWRITER_H_

#include "Gate.h"
#include "MathConceptsCommon.h"

#include <ostream>

class IGateWriter {
public:
	virtual ~IGateWriter(){};

	virtual void writeGate(GatePtr pGate, std::ostream& ouputStream) = 0;
};




#endif /* IGATEWRITER_H_ */
