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

void LabelOnlyGateWriterImpl::write(GatePtr pGate, std::ostream& outputStream) {
	std::string delimeter = ".";
	outputStream << "Gate:";
	for(label_t::const_iterator lIter = pGate->getLabel().begin(); lIter != pGate->getLabel().end(); lIter++) {
		outputStream  << *lIter  << delimeter;
	}
	outputStream << std::endl;
}



