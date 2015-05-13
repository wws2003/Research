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
	MatrixXcd eigenMat1 = toEigenMatrix(pm1);
	MatrixXcd eigenMat2 = toEigenMatrix(pm2);
	MatrixXcd eigSum = eigenMat1 + eigenMat2;

	prSum = fromEigenMatrix(eigSum);

}

void SampleMatrixOperator::multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prProduct) {
	MatrixXcd eigenMat1 = toEigenMatrix(pm1);
	MatrixXcd eigenMat2 = toEigenMatrix(pm2);
	MatrixXcd eigProduct = eigenMat1 * eigenMat2;

	prProduct = fromEigenMatrix(eigProduct, pm1->getLabel() + pm2->getLabel());
}

void SampleMatrixOperator::subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSubtract) {
	MatrixXcd eigenMat1 = toEigenMatrix(pm1);
	MatrixXcd eigenMat2 = toEigenMatrix(pm2);
	MatrixXcd eigSubstract = eigenMat1 - eigenMat2;

	prSubtract = fromEigenMatrix(eigSubstract);
}

ComplexVal SampleMatrixOperator::det(MatrixPtr pm) {
	MatrixXcd eigenMat = toEigenMatrix(pm);
	return eigenMat.determinant();
}

void SampleMatrixOperator::inverse(MatrixPtr pm, MatrixPtrRef prInverseMatrix) {
	MatrixXcd eigenMat = toEigenMatrix(pm);
	MatrixXcd eigInverse = eigenMat.inverse();
	prInverseMatrix = fromEigenMatrix(eigInverse);
}

void SampleMatrixOperator::exponential(MatrixPtr pm, MatrixPtrRef prExpMatrix) {
	MatrixXcd eigenMat = toEigenMatrix(pm);
	MatrixXcd eigenExpMat(eigenMat.rows(), eigenMat.cols());

	//Wrong implementation. This calculate exponential of each element!
	//eigenExpMat = eigenMat.array().exp();
	eigenExpMat = eigenMat.exp();

	prExpMatrix = fromEigenMatrix(eigenExpMat);
}

void SampleMatrixOperator::log(MatrixPtr pm, MatrixPtrRef prLog) {
	MatrixXcd eigenMat = toEigenMatrix(pm);
	MatrixXcd eigenLogMat(eigenMat.rows(), eigenMat.cols());
	eigenLogMat = eigenMat.log();
	prLog = fromEigenMatrix(eigenLogMat);
}

void SampleMatrixOperator::eig(MatrixPtr pm, VectorPtrRef prEigVals, MatrixPtrRef prEigVects) {
	//TODO Implement
}

Eigen::MatrixXcd SampleMatrixOperator::toEigenMatrix(MatrixPtr pMatrix) {
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);
	MatrixXcd m(nbRows, nbColumns);

	for(int i = 0; i < nbRows; i++) {
		for(int j = 0; j < nbColumns; j++) {
			m(i,j) = pMatrix->getValue(i, j);
		}
	}

	return m;
}

MatrixPtr SampleMatrixOperator::fromEigenMatrix(Eigen::MatrixXcd eigenMatrix, std::string label) {
	int nbRows = eigenMatrix.rows();
	int nbCols = eigenMatrix.cols();

	ComplexValArray arr = new ComplexVal[nbRows * nbCols];
	Map<MatrixXcd>(arr, nbRows, nbCols) = eigenMatrix;

	MatrixPtr pMatrix = m_pMatrixFactory->getMatrixFromValues(nbRows, nbCols, arr, label);

	return pMatrix;
}


