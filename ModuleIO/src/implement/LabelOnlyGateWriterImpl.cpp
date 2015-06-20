/*
 * LabelOnlyGateWriterImpl.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#include "LabelOnlyGateWriterImpl.h"
#include "Gate.h"

LabelOnlyGateWriterImpl::LabelOnlyGateWriterImpl() {

}

void LabelOnlyGateWriterImpl::writeGate(GatePtr pGate, std::ostream& outputStream) {
	outputStream << "Gate label:" << pGate->getLabelStr() << std::endl;
}



