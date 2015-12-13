/*
 * DuplicateMatrixLookupResultFilterImpl.cpp
 *
 *  Created on: Nov 27, 2015
 *      Author: pham
 */

#include "DuplicateMatrixLookupResultFilterImpl.h"
#include "DuplicateLookupResultFilterImpl.cpp"

template class DuplicateLookupResultFilterImpl<MatrixPtr>;

bool DuplicateMatrixLookupResultFilterImpl::sameElement(const MatrixPtr& pMatrix1, const MatrixPtr& pMatrix2) const {
	return *pMatrix1 == *pMatrix2;
}
