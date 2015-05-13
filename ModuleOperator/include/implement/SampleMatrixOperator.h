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

	virtual void inverse(MatrixPtr pm, MatrixPtrRef ppInverseMatrix) ;
	virtual void exponential(MatrixPtr pm, MatrixPtrRef ppExpMatrix) ;
	virtual void log(MatrixPtr pm, MatrixPtrRef prLog);
	virtual void eig(MatrixPtr pm, VectorPtrRef ppEigVals, MatrixPtrRef ppEigVects);


private:
	Eigen::MatrixXcd toEigenMatrix(MatrixPtr pMatrix);
	MatrixPtr fromEigenMatrix(Eigen::MatrixXcd eigenMatrix);

	MatrixFactoryPtr m_pMatrixFactory;
};



#endif /* SAMPLEMATRIXOPERATOR_H_ */
