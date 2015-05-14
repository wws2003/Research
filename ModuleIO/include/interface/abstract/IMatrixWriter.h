/*
 * IMatrixWriter.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef IMATRIXWRITER_H_
#define IMATRIXWRITER_H_

#include "MathConceptsCommon.h"
#include <ostream>

class IMatrixWriter {
public:
	virtual ~IMatrixWriter(){};

	virtual void writeMatrix(MatrixPtr pMatrix, std::ostream& ouputStream) = 0;
};


#endif /* IMATRIXWRITER_H_ */
