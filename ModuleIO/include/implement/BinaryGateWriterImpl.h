/*
 * BinaryGateWriterImpl.h
 *
 *  Created on: Aug 9, 2015
 *      Author: pham
 */

#ifndef BINARYGATEWRITERIMPL_H_
#define BINARYGATEWRITERIMPL_H_

#include "IOCommon.h"
#include "IWriter.h"
#include "Gate.h"
#include "IMatrix.h"

class BinaryGateWriterImpl : public IGateWriter {
public:
	BinaryGateWriterImpl(MatrixWriterPtr pMatrixWriter = NullPtr);

	virtual ~BinaryGateWriterImpl(){};
	virtual void write(GatePtr pGate, std::ostream& ouputStream);

protected:
	virtual void writeMatrix(std::string label, MatrixPtr pMatrix, std::ostream& ouputStream);

private:
	MatrixWriterPtr m_pMatrixWriter;
};


#endif /* BINARYGATEWRITERIMPL_H_ */
