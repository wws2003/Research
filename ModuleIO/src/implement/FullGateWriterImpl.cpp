/*
 * FullGateWriterImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "FullGateWriterImpl.h"
#include <cstdio>

#define PRINT_BUFFER_LENGTH 128

FullGateWriterImpl::FullGateWriterImpl() {

}

void FullGateWriterImpl::write(GatePtr pGate, std::ostream& outputStream) {
	std::string delimeter = "*";
	outputStream << "Gate:";
	for(LabelSeq::const_iterator lIter = pGate->getLabelSeq().begin(); lIter != pGate->getLabelSeq().end(); lIter++) {
		outputStream  << *lIter  << delimeter;
	}
	outputStream << std::endl;
	outputStream << "--Gate cost:" << pGate->getCost();
	outputStream << std::endl;
	outputStream << "--Gate matrix:" << std::endl;

	MatrixPtr pMatrix = pGate->getMatrix();
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);
	for(int i = 0; i < nbRows; i++) {
		for(int j = 0; j < nbColumns; j++) {
			char printfBuffer[PRINT_BUFFER_LENGTH];
			ComplexVal val = pMatrix->getValue(i,j);
			sprintf(printfBuffer, "[%.3f %s %.3f i] ", val.real(), val.imag() > 0 ? "+" : "-", std::abs(val.imag()));
			outputStream << printfBuffer;
		}
		outputStream << std::endl;
	}
}



