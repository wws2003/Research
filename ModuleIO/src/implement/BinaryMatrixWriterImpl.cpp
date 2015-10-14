/*
 * BinaryMatrixWriterImpl.cpp
 *
 *  Created on: Aug 9, 2015
 *      Author: pham
 */

#include "IMatrix.h"
#include "BinaryMatrixWriterImpl.h"
#include "ComplexValIO.h"

BinaryMatrixWriterImpl::BinaryMatrixWriterImpl(){

}

void BinaryMatrixWriterImpl::write(MatrixPtr pMatrix, std::ostream& outputStream) {

	std::string label = pMatrix->getLabel();

	//Write label length
	int labelLength = label.size();
	outputStream.write((const char*)&labelLength, sizeof(int));

	//Write label
	outputStream.write(label.c_str(), labelLength);

	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);

	//Write nb rows
	outputStream.write((const char*)&nbRows, sizeof(int));

	//Write nb columns
	outputStream.write((const char*)&nbColumns, sizeof(int));

	//Write array
	ComplexValArray matrixArray;
	pMatrix->toArray(matrixArray);
	int arraySize = nbRows * nbColumns;
	for(int i = 0; i < arraySize; i++) {
		writeComplexValue(matrixArray[i], outputStream);
	}

	//Release array
	delete[] matrixArray;

	outputStream.flush();
}


