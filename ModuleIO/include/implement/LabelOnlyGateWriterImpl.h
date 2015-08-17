/*
 * LabelOnlyGateWriterImpl.h
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#ifndef LABELONLYGATEWRITERIMPL_H_
#define LABELONLYGATEWRITERIMPL_H_

#include "IOCommon.h"
#include "IWriter.h"

class LabelOnlyGateWriterImpl : public IGateWriter {
public:
	LabelOnlyGateWriterImpl(std::string endStr = "\r\n");

	virtual ~LabelOnlyGateWriterImpl(){};
	virtual void write(GatePtr pGate, std::ostream& ouputStream);

private:
	std::string m_endStr;
};


#endif /* LABELONLYGATEWRITERIMPL_H_ */
