/*
 * LabelOnlyGateWriterImpl.h
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#ifndef LABELONLYGATEWRITERIMPL_H_
#define LABELONLYGATEWRITERIMPL_H_

#include "IGateWriter.h"

class LabelOnlyGateWriterImpl : public IGateWriter {
public:
	LabelOnlyGateWriterImpl();

	virtual ~LabelOnlyGateWriterImpl(){};
	virtual void writeGate(GatePtr pGate, std::ostream& ouputStream);
};


#endif /* LABELONLYGATEWRITERIMPL_H_ */
