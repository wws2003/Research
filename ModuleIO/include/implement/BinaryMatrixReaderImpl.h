/*
 * BinaryMatrixReaderImpl.h
 *
 *  Created on: Aug 9, 2015
 *      Author: pham
 */

#ifndef BINARYMATRIXREADERIMPL_H_
#define BINARYMATRIXREADERIMPL_H_

#include "IReader.h"
#include "IOCommon.h"
#include "IMatrixFactory.h"
#include "MathConceptsCommon.h"

class BinaryMatrixReaderImpl: public IReader<MatrixPtr> {
public:
	BinaryMatrixReaderImpl(MatrixFactoryPtr pMatrixFactory);

	virtual ~BinaryMatrixReaderImpl(){};

	virtual void read(MatrixPtr& rpMatrix, std::istream &inputStream);

private:
	//FIXME Only handle upto 16x16 matrices
	ComplexVal m_valBuffer[256];
	MatrixFactoryPtr m_pMatrixFactory;
};


#endif /* BINARYMATRIXREADERIMPL_H_ */
