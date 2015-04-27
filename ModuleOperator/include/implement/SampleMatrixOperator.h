/*
 * SampleMatrixOperator.h
 *
 *  Created on: Apr 23, 2015
 *      Author: pham
 */

#ifndef SAMPLEMATRIXOPERATOR_H_
#define SAMPLEMATRIXOPERATOR_H_

#include "IMatrixOperator.h"
#include "eigen3/Eigen/Dense"

class SampleMatrixOperator: public IMatrixOperator {
public:
	SampleMatrixOperator(MatrixFactoryPtr pMatrixFactory);
	virtual ~SampleMatrixOperator() {};

	virtual void add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pSum) ;
	virtual void multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pProduct) ;
	virtual void subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pSubtract) ;
	virtual ComplexVal det(MatrixPtr pm) ;

	virtual void inverse(SquareMatrixPtr pm, SquareMatrixPtrRef ppInverseMatrix) ;
	virtual void exponential(SquareMatrixPtr pm, SquareMatrixPtrRef ppExpMatrix) ;
	virtual void log(SquareMatrixPtr pm, SquareMatrixPtr prLog);
	virtual void eig(SquareMatrixPtr pm, VectorPtrRef ppEigVals, SquareMatrixPtrRef ppEigVects);


private:
	Eigen::MatrixXcd toEigenMatrix(MatrixPtr pMatrix);
	MatrixPtr fromEigenMatrix(Eigen::MatrixXcd eigenMatrix);

	Eigen::MatrixXcd toEigenSquareMatrix(SquareMatrixPtr pMatrix);
	SquareMatrixPtr fromEigenSquareMatrix(Eigen::MatrixXcd eigenMatrix);


	MatrixFactoryPtr m_pMatrixFactory;
};



#endif /* SAMPLEMATRIXOPERATOR_H_ */
