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

void LabelOnlyMatrixWriterImpl::write(MatrixPtr pMatrix, std::ostream& outputStream) {
	outputStream << "Matrix label:" << pMatrix->getLabel() << std::endl;
}



