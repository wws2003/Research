/*
 * SampleMatrixWriter.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef SAMPLEMATRIXWRITER_H_
#define SAMPLEMATRIXWRITER_H_

#include "IMatrixWriter.h"

class SampleMatrixWriterImpl : public IMatrixWriter {
public:
	SampleMatrixWriterImpl();

	virtual ~SampleMatrixWriterImpl(){};
	virtual void writeMatrix(MatrixPtr pMatrix, std::ostream& ouputStream);
};


#endif /* SAMPLEMATRIXWRITER_H_ */
