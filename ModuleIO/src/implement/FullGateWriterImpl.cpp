/*
 * FullGateWriterImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "FullGateWriterImpl.h"
#include <cstdio>

#define PRINT_BUFFER_LENGTH 128

void printVal(char* printBuffer, const ComplexVal& val);

FullGateWriterImpl::FullGateWriterImpl() {

}

void FullGateWriterImpl::write(GatePtr pGate, std::ostream& outputStream) {
	std::string delimeter = "*";
	outputStream << "Gate:";

	unsigned int nbSeq = pGate->getLabelSeq().size();
	for(unsigned int i = 0; i < nbSeq; i++) {
		outputStream  << pGate->getLabelSeq()[i];
		if(i < nbSeq - 1) {
			outputStream << delimeter;
		}
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
			printVal(printfBuffer, val);
			outputStream << printfBuffer;
		}
		outputStream << std::endl;
	}
}

void printVal(char* printfBuffer, const ComplexVal& val) {
	sprintf(printfBuffer, "[%.6f %s %.6f i] ", mreal::toDouble(val.real()), mreal::toDouble(val.imag()) > 0 ? "+" : "-", std::abs(mreal::toDouble(val.imag())));
}



