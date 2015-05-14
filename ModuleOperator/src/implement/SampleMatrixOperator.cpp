/*
 * SampleMatrixOperator.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: pham
 */

#include "SampleMatrixOperator.h"
#include "IMatrixFactory.h"
#include "eigen3/Eigen/Dense"
#include "eigen3/unsupported/Eigen/src/MatrixFunctions/MatrixExponential.h"
#include "eigen3/unsupported/Eigen/MatrixFunctions"
#include <iostream>

using namespace Eigen;

SampleMatrixOperator::SampleMatrixOperator(MatrixFactoryPtr pMatrixFactory) : m_pMatrixFactory(pMatrixFactory) {

}

void SampleMatrixOperator::add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSum) {
	MatrixXcd eigenMat1, eigenMat2;
	toEigenMatrix(pm1, eigenMat1);
	toEigenMatrix(pm2, eigenMat2);
	MatrixXcd eigSum = eigenMat1 + eigenMat2;

	prSum = fromEigenMatrix(eigSum);

}

void SampleMatrixOperator::multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prProduct) {
	MatrixXcd eigenMat1, eigenMat2;
	toEigenMatrix(pm1, eigenMat1);
	toEigenMatrix(pm2, eigenMat2);
	MatrixXcd eigProduct = eigenMat1 * eigenMat2;

	prProduct = fromEigenMatrix(eigProduct, pm1->getLabel() + pm2->getLabel());
}

void SampleMatrixOperator::multiplyScalar(MatrixPtr pm, ComplexVal scalar, MatrixPtrRef prProduct) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	eigenMat *= scalar;
	prProduct = fromEigenMatrix(eigenMat);
}

void SampleMatrixOperator::subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSubtract) {
	MatrixXcd eigenMat1, eigenMat2;
	toEigenMatrix(pm1, eigenMat1);
	toEigenMatrix(pm2, eigenMat2);
	MatrixXcd eigSubstract = eigenMat1 - eigenMat2;

	prSubtract = fromEigenMatrix(eigSubstract);
}

ComplexVal SampleMatrixOperator::det(MatrixPtr pm) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	return eigenMat.determinant();
}

ComplexVal SampleMatrixOperator::trace(MatrixPtr pm) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	return eigenMat.trace();
}

void SampleMatrixOperator::power(MatrixPtr pm, int exponent, MatrixPtrRef prPower) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	MatrixXcd eigenPower = eigenMat.pow(exponent);
	prPower = fromEigenMatrix(eigenPower);
}

void SampleMatrixOperator::sqrt(MatrixPtr pm, MatrixPtrRef prSqrt) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	MatrixXcd eigenSqrt = eigenMat.sqrt();
	prSqrt = fromEigenMatrix(eigenSqrt);
}


void SampleMatrixOperator::conjugateTranpose(MatrixPtr pm, MatrixPtrRef prConjugate) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	eigenMat.transposeInPlace();
	MatrixXcd conjugateEigenMat = eigenMat.conjugate();
	prConjugate = fromEigenMatrix(conjugateEigenMat, pm->getLabel() + "'");
}

void SampleMatrixOperator::transpose(MatrixPtr pm, MatrixPtrRef prTranspose) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);

	eigenMat.transposeInPlace();

	prTranspose = fromEigenMatrix(eigenMat, pm->getLabel() + "*");
}

void SampleMatrixOperator::inverse(MatrixPtr pm, MatrixPtrRef prInverseMatrix) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	MatrixXcd eigInverse = eigenMat.inverse();
	prInverseMatrix = fromEigenMatrix(eigInverse);
}

void SampleMatrixOperator::exponential(MatrixPtr pm, MatrixPtrRef prExpMatrix) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	MatrixXcd eigenExpMat(eigenMat.rows(), eigenMat.cols());

	//Wrong implementation. This calculate exponential of each element!
	//eigenExpMat = eigenMat.array().exp();
	eigenExpMat = eigenMat.exp();

	prExpMatrix = fromEigenMatrix(eigenExpMat);
}

void SampleMatrixOperator::log(MatrixPtr pm, MatrixPtrRef prLog) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	MatrixXcd eigenLogMat(eigenMat.rows(), eigenMat.cols());
	eigenLogMat = eigenMat.log();
	prLog = fromEigenMatrix(eigenLogMat);
}

void SampleMatrixOperator::eig(MatrixPtr pm, VectorPtrRef prEigVals, MatrixPtrRef prEigVects) {
	//TODO Implement
}

void SampleMatrixOperator::toEigenMatrix(MatrixPtr pMatrix, MatrixXcd& rEigenMat) {
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);

	rEigenMat = MatrixXcd(nbRows, nbColumns);

	for(int i = 0; i < nbRows; i++) {
		for(int j = 0; j < nbColumns; j++) {
			rEigenMat(i,j) = pMatrix->getValue(i, j);
		}
	}
}

MatrixPtr SampleMatrixOperator::fromEigenMatrix(MatrixXcd& eigenMatrix, std::string label) {
	int nbRows = eigenMatrix.rows();
	int nbCols = eigenMatrix.cols();

	ComplexValArray arr = new ComplexVal[nbRows * nbCols];
	Map<MatrixXcd>(arr, nbRows, nbCols) = eigenMatrix;

	MatrixPtr pMatrix = m_pMatrixFactory->getMatrixFromValues(nbRows, nbCols, arr, COLUMN_SPLICE, label);

	return pMatrix;
}


