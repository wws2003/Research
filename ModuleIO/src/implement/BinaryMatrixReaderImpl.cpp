/*
 * BinaryMatrixReaderImpl.cpp
 *
 *  Created on: Aug 9, 2015
 *      Author: pham
 */

#include "BinaryMatrixReaderImpl.h"
#include "ComplexValIO.h"

#define LABEL_MAX_SIZE 256

BinaryMatrixReaderImpl::BinaryMatrixReaderImpl(MatrixFactoryPtr pMatrixFactory) {
	m_pMatrixFactory = pMatrixFactory;
}

void BinaryMatrixReaderImpl::read(MatrixPtr& rpMatrix, std::istream &inputStream) {

	//Read label length
	int labelLength;
	inputStream.read((char*)&labelLength, sizeof(int));

	//Read label
	char label[LABEL_MAX_SIZE];
	inputStream.read((char*)label, labelLength);

	int nbRows, nbColumns;

	//Read nb rows
	inputStream.read((char*)&nbRows, sizeof(int));

	//Read nb columns
	inputStream.read((char*)&nbColumns, sizeof(int));

	//Read array
	int arraySize = nbRows * nbColumns;
	for(int i = 0; i < arraySize; i++) {
		readComplexVal(inputStream, &m_valBuffer[i]);
	}
	endOfReadComplexVals(inputStream);

	//Generate matrix instance
	std::string labelStr(label, labelLength);
	rpMatrix = m_pMatrixFactory->getMatrixFromValues(nbRows, nbColumns, m_valBuffer, ROW_SPLICE, labelStr);

}
