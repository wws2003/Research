/*
 * SampleMatrixWritter.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#include "SampleMatrixWriter.h"
#include "IMatrix.h"

SampleMatrixWriterImpl::SampleMatrixWriterImpl() {

}

void SampleMatrixWriterImpl::writeMatrix(MatrixPtr pMatrix, std::ostream& outputStream) {
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);
	outputStream << "Matrix label:" << pMatrix->getLabel() << std::endl;
	for(int i = 0; i < nbRows; i++) {
		for(int j = 0; j < nbColumns; j++) {
			ComplexVal val = pMatrix->getValue(i,j);
			outputStream << "(" << val.real() << "," << val.imag() << ")	";
		}
		outputStream << std::endl;
	}
}


