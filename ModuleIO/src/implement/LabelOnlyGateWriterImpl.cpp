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

	for(LabelSeq::const_iterator lIter = pGate->getLabelSeq().begin(); lIter != pGate->getLabelSeq().end(); lIter++) {
		if(lIter == pGate->getLabelSeq().begin()) {
			outputStream << *lIter  ;
		}
		else {
			outputStream << delimeter << *lIter;
		}
	}
	outputStream << m_endStr;
}



