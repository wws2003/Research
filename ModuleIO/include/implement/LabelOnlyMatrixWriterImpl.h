/*
 * LabelOnlyMatrixWriterImpl.h
 *
 *  Created on: Jun 17, 2015
 *      Author: pham
 */

#ifndef LABELONLYMATRIXWRITERIMPL_H_
#define LABELONLYMATRIXWRITERIMPL_H_

#include "IOCommon.h"
#include "IWriter.h"

class LabelOnlyMatrixWriterImpl : public IMatrixWriter {
public:
	LabelOnlyMatrixWriterImpl();

	virtual ~LabelOnlyMatrixWriterImpl(){};
	virtual void write(MatrixPtr pMatrix, std::ostream& ouputStream);
};



#endif /* LABELONLYMATRIXWRITERIMPL_H_ */
