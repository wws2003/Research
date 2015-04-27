/*
 * IMatrixVectorMapper.h
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#ifndef IMATRIXVECTORMAPPER_H_
#define IMATRIXVECTORMAPPER_H_

class IMatrixVectorMapper {
public:
	virtual ~IMatrixVectorMapper(){};

	virtual void vectorToMatrix(VectorPtr pVector, MatrixPtrRef prMatrix) = 0;

	virtual void matrixToVector(MatrixPtr pMatrix, VectorPtrRef prVector) = 0;
};


#endif /* IMATRIXVECTORMAPPER_H_ */
