/*
 * FullGateWriterImpl.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef FULLGATEWRITERIMPL_H_
#define FULLGATEWRITERIMPL_H_

#include "IOCommon.h"
#include "IWriter.h"
#include "Gate.h"

class FullGateWriterImpl : public IGateWriter {
public:
	FullGateWriterImpl();

	virtual ~FullGateWriterImpl(){};
	virtual void write(GatePtr pGate, std::ostream& ouputStream);
};



#endif /* FULLGATEWRITERIMPL_H_ */
