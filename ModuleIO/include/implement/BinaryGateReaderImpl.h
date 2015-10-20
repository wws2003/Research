/*
 * BinaryGateReaderImpl.h
 *
 *  Created on: Aug 9, 2015
 *      Author: pham
 */

#ifndef BINARYGATEREADERIMPL_H_
#define BINARYGATEREADERIMPL_H_

#include "IReader.h"
#include "IOCommon.h"
#include "Gate.h"
#include "IMatrix.h"
#include "MathConceptsCommon.h"

class BinaryGateReaderImpl: public IReader<GatePtr> {
public:
	BinaryGateReaderImpl(MatrixReaderPtr pMatrixReader = NullPtr);

	virtual ~BinaryGateReaderImpl(){};

	virtual void read(GatePtr& rpGate, std::istream &inputStream);

private:
	MatrixReaderPtr m_pMatrixReader;
};


#endif /* BINARYGATEREADERIMPL_H_ */
