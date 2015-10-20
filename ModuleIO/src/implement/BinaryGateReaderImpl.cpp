/*
 * BinaryGateReaderImpl.cpp
 *
 *  Created on: Aug 9, 2015
 *      Author: pham
 */


#include "BinaryGateReaderImpl.h"
#define LABEL_MAX_SIZE 256

BinaryGateReaderImpl::BinaryGateReaderImpl(MatrixReaderPtr pMatrixReader) {
	m_pMatrixReader = pMatrixReader;
}

void BinaryGateReaderImpl::read(GatePtr& rpGate, std::istream &inputStream) {

	//Read size of label sequence
	size_t labelSeqSize;
	inputStream.read((char*)&labelSeqSize, sizeof(size_t));

	//Read label sequence
	LabelSeq labelSeq;
	for(unsigned int i = 0; i < labelSeqSize; i++) {
		//Write label length
		int labelLength;
		inputStream.read((char*)&labelLength, sizeof(int));

		//Read label
		char label[LABEL_MAX_SIZE];
		inputStream.read(label, labelLength);

		std::string labelStr(label, labelLength);
		labelSeq.push_back(labelStr);
	}

	//Read cost
	cost_t cost;
	inputStream.read((char*)&cost, sizeof(cost_t));

	//Read matrix
	MatrixPtr pMatrix = NullPtr;
	if(m_pMatrixReader != NullPtr) {
		m_pMatrixReader->read(pMatrix, inputStream);
	}

	//Create gate instance
	rpGate = new Gate(pMatrix, cost, labelSeq);
}



