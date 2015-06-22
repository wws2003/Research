/*
 * FullGateWriterImpl.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#include "FullGateWriterImpl.h"

FullGateWriterImpl::FullGateWriterImpl() {

}

void FullGateWriterImpl::write(GatePtr pGate, std::ostream& outputStream) {
	std::string delimeter = ".";
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
			ComplexVal val = pMatrix->getValue(i,j);
			outputStream << "(" << val.real() << "," << val.imag() << ")	";
		}
		outputStream << std::endl;
	}
}



