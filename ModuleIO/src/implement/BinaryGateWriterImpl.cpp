/*
 * BinaryGateWriterImpl.cpp
 *
 *  Created on: Aug 9, 2015
 *      Author: pham
 */

#include "BinaryGateWriterImpl.h"

BinaryGateWriterImpl::BinaryGateWriterImpl(MatrixWriterPtr pMatrixWriter) {
	m_pMatrixWriter = pMatrixWriter;
}

void BinaryGateWriterImpl::write(GatePtr pGate, std::ostream& ouputStream) {

	LabelSeq labelSeq = pGate->getLabelSeq();
	//Write size of label sequence
	size_t labelSeqSize = labelSeq.size();
	ouputStream.write((const char*)&labelSeqSize, sizeof(size_t));

	//Write label sequence
	for(LabelSeq::const_iterator lIter = labelSeq.begin(); lIter != labelSeq.end(); lIter++) {
		//Write label length
		int labelLength = lIter->size();
		ouputStream.write((const char*)&labelLength, sizeof(int));

		//Write label
		ouputStream.write(lIter->c_str(), labelLength);
	}

	//Write cost
	cost_t cost = pGate->getCost();
	ouputStream.write((const char*)&cost, sizeof(cost_t));

	//Write matrix
	writeMatrix(pGate->getLabelStr(), pGate->getMatrix(), ouputStream);

	ouputStream.flush();
}

void BinaryGateWriterImpl::writeMatrix(std::string label, MatrixPtr pMatrix, std::ostream& ouputStream) {
	if(m_pMatrixWriter != NullPtr) {
		m_pMatrixWriter->write(pMatrix, ouputStream);
	}
}

