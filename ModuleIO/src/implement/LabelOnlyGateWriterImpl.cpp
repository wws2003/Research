/*
 * LabelOnlyGateWriterImpl.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#include "LabelOnlyGateWriterImpl.h"
#include "Gate.h"

LabelOnlyGateWriterImpl::LabelOnlyGateWriterImpl(std::string endStr) {
	m_endStr = endStr;
}

void LabelOnlyGateWriterImpl::write(GatePtr pGate, std::ostream& outputStream) {
	std::string delimeter = "*";
	outputStream << "Gate:";
	for(LabelSeq::const_iterator lIter = pGate->getLabelSeq().begin(); lIter != pGate->getLabelSeq().end(); lIter++) {
		outputStream  << *lIter  << delimeter;
	}
	outputStream << m_endStr;
}



