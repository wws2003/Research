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
#include <map>

typedef Eigen::Matrix<ComplexVal, Eigen::Dynamic, Eigen::Dynamic>  MatrixXcmp;

class SampleMatrixOperator: public IMatrixOperator {
public:
	SampleMatrixOperator(MatrixFactoryPtr pMatrixFactory);
	virtual ~SampleMatrixOperator() {};

	virtual void add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pSum) ;
	virtual void multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pProduct) ;
	virtual void multiplyScalar(MatrixPtr pm, ComplexVal scalar, MatrixPtrRef pProduct);
	virtual void subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef pSubtract) ;

	virtual ComplexVal det(MatrixPtr pm) ;
	virtual ComplexVal trace(MatrixPtr pm);
	virtual void power(MatrixPtr pm, int exponent, MatrixPtrRef prPower);
	virtual void sqrt(MatrixPtr pm, MatrixPtrRef prSqrt);

	virtual void conjugateTranpose(MatrixPtr pm, MatrixPtrRef prConjugate);
	virtual void transpose(MatrixPtr pm, MatrixPtrRef prTranspose);

	virtual void inverse(MatrixPtr pm, MatrixPtrRef ppInverseMatrix) ;
	virtual void exponential(MatrixPtr pm, MatrixPtrRef ppExpMatrix) ;
	virtual void log(MatrixPtr pm, MatrixPtrRef prLog);
	virtual void eig(MatrixPtr pm, ComplexVectorRef ppEigVals, MatrixPtrRef ppEigVects);

	void sumProduct(const MatrixPtrVector& matrixVector, const ComplexVector& scalaVector, MatrixPtrRef pSum);

	void unitaryDediagonalize(MatrixPtr pU, const ComplexVectorRef diag, MatrixPtrRef prRoot);

	void specialUnitaryFromUnitary(MatrixPtr pU, MatrixPtrRef prSU);

	void getTracelessHermitianMatricesBasis(int dimension, MatrixPtrVector& pBasis) ;

	void getRotationMatrix(MatrixPtr pBasis, mreal_t rotationAngle, MatrixPtrRef pRotationMatrix);

private:
	void toEigenMatrix(MatrixPtr pMatrix, MatrixXcmp& rEigenMat);
	MatrixPtr fromEigenMatrix(MatrixXcmp& eigenMatrix, std::string label = "");
	void fromEigenMatrix(MatrixXcmp& eigenMatrix, int coloumn, ComplexVectorRef rComplexVector);

	MatrixFactoryPtr m_pMatrixFactory;

	typedef std::map<int, MatrixPtrVector> HermitianBasisMap;
	HermitianBasisMap m_hermitianBasisMap;
};



#endif /* SAMPLEMATRIXOPERATOR_H_ */
