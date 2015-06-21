/*
 * SampleMatrixWriter.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef SAMPLEMATRIXWRITER_H_
#define SAMPLEMATRIXWRITER_H_

#include "IWriter.h"
#include "IOCommon.h"

class SampleMatrixWriterImpl : public IMatrixWriter {
public:
	SampleMatrixWriterImpl();

	virtual ~SampleMatrixWriterImpl(){};
	virtual void write(MatrixPtr pMatrix, std::ostream& ouputStream);
};


#endif /* SAMPLEMATRIXWRITER_H_ */
