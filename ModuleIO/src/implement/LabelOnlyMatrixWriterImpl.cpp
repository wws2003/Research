/*
 * LabelOnlyMatrixWriterImpl.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: pham
 */

#include "LabelOnlyMatrixWriterImpl.h"
#include "IMatrix.h"

LabelOnlyMatrixWriterImpl::LabelOnlyMatrixWriterImpl() {

}

void LabelOnlyMatrixWriterImpl::writeMatrix(MatrixPtr pMatrix, std::ostream& outputStream) {
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);
	outputStream << "Matrix label:" << pMatrix->getLabel() << std::endl;
}



