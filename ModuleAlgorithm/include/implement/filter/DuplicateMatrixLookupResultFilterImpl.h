/*
 * DuplicateMatrixLookupResultFilterImpl.h
 *
 *  Created on: Nov 27, 2015
 *      Author: pham
 */

#ifndef DUPLICATEMATRIXLOOKUPRESULTFILTERIMPL_H_
#define DUPLICATEMATRIXLOOKUPRESULTFILTERIMPL_H_

#include "IMatrix.h"
#include "DuplicateLookupResultFilterImpl.h"

class DuplicateMatrixLookupResultFilterImpl: public DuplicateLookupResultFilterImpl<MatrixPtr> {
protected:
	bool sameElement(const MatrixPtr& pMatrix1, const MatrixPtr& pMatrix2) const;
};



#endif /* DUPLICATEMATRIXLOOKUPRESULTFILTERIMPL_H_ */
