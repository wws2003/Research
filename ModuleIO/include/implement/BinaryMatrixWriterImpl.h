/*
 * BinaryMatrixWriter.h
 *
 *  Created on: Aug 9, 2015
 *      Author: pham
 */

#ifndef BINARYMATRIXWRITERIMPL_H_
#define BINARYMATRIXWRITERIMPL_H_


#include "IWriter.h"
#include "IOCommon.h"

class BinaryMatrixWriterImpl : public IMatrixWriter {
public:
	BinaryMatrixWriterImpl();

	virtual ~BinaryMatrixWriterImpl(){};
	virtual void write(MatrixPtr pMatrix, std::ostream& ouputStream);
};


#endif /* BINARYMATRIXWRITER_H_ */
