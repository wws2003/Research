/*
 * SampleMatrixOperator.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: pham
 */

#include "SampleMatrixOperator.h"
#include "IMatrixFactory.h"
#include "eigen3/Eigen/Dense"

using namespace Eigen;

SampleMatrixOperator::SampleMatrixOperator(MatrixFactoryPtr pMatrixFactory) : m_pMatrixFactory(pMatrixFactory) {

}

void SampleMatrixOperator::add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSum) {
	//TODO Think over: Should check size here or use a kind of interceptor instead ?
	MatrixXcd eigenMat1 = toEigenMatrix(pm1);
	MatrixXcd eigenMat2 = toEigenMatrix(pm2);
	MatrixXcd eigSum = eigenMat1 + eigenMat2;

	prSum = fromEigenMatrix(eigSum);

}

void SampleMatrixOperator::multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prProduct) {
	MatrixXcd eigenMat1 = toEigenMatrix(pm1);
	MatrixXcd eigenMat2 = toEigenMatrix(pm2);
	MatrixXcd eigProduct = eigenMat1 * eigenMat2;

	prProduct = fromEigenMatrix(eigProduct);
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

void SampleMatrixOperator::inverse(SquareMatrixPtr pm, SquareMatrixPtrRef prInverseMatrix) {
	MatrixXcd eigenMat = toEigenSquareMatrix(pm);
	MatrixXcd eigInverse = eigenMat.inverse();
	prInverseMatrix = fromEigenSquareMatrix(eigInverse);
}

void SampleMatrixOperator::exponential(SquareMatrixPtr pm, SquareMatrixPtrRef prExpMatrix) {
	MatrixXcd eigenMat = toEigenSquareMatrix(pm);
	MatrixXcd eigenExpMat(eigenMat.rows(), eigenMat.cols());
	eigenExpMat = eigenMat.array().exp();
	prExpMatrix = fromEigenSquareMatrix(eigenExpMat);
}

void SampleMatrixOperator::log(SquareMatrixPtr pm, SquareMatrixPtr prLog) {
	MatrixXcd eigenMat = toEigenSquareMatrix(pm);
	MatrixXcd eigenLogMat(eigenMat.rows(), eigenMat.cols());
	eigenLogMat = eigenMat.array().log();
	prLog = fromEigenSquareMatrix(eigenLogMat);
}

void SampleMatrixOperator::eig(SquareMatrixPtr pm, VectorPtrRef prEigVals, SquareMatrixPtrRef prEigVects) {
	//TODO Implement
}

Eigen::MatrixXcd SampleMatrixOperator::toEigenMatrix(MatrixPtr pMatrix) {
	//TODO Implement
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);
	MatrixXcd m(nbRows, nbColumns);
	return m;
}

MatrixPtr SampleMatrixOperator::fromEigenMatrix(Eigen::MatrixXcd eigenMatrix) {
	//TODO Implement
	return NullPtr;
}

MatrixXcd SampleMatrixOperator::toEigenSquareMatrix(SquareMatrixPtr pMatrix) {
	//TODO Implement
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);
	MatrixXcd m(nbRows, nbColumns);
	return m;
}

SquareMatrixPtr SampleMatrixOperator::fromEigenSquareMatrix(Eigen::MatrixXcd eigenMatrix) {
	//TODO Implement
	return NullPtr;
}



