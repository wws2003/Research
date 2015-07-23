/*
 * SampleMatrixOperator.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: pham
 */

#include "SampleMatrixOperator.h"
#include "IMatrixFactory.h"
#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Eigenvalues"
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
	prConjugate = fromEigenMatrix(conjugateEigenMat);
}

void SampleMatrixOperator::transpose(MatrixPtr pm, MatrixPtrRef prTranspose) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);

	eigenMat.transposeInPlace();

	prTranspose = fromEigenMatrix(eigenMat);
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

void SampleMatrixOperator::eig(MatrixPtr pm, ComplexVectorRef rEigVals, MatrixPtrRef prEigVects) {
	MatrixXcd eigenMat;
	toEigenMatrix(pm, eigenMat);
	ComplexEigenSolver<MatrixXcd> ces(eigenMat, true);
	MatrixXcd eigenVectors = ces.eigenvectors();
	MatrixXcd eigenValues = ces.eigenvalues();
	prEigVects = fromEigenMatrix(eigenVectors);
	fromEigenMatrix(eigenValues, 0, rEigVals);
}

void SampleMatrixOperator::unitaryDediagonalize(MatrixPtr pU, const ComplexVectorRef diag, MatrixPtrRef prRoot) {
	int nbRows, nbColumns;
	pU->getSize(nbRows, nbColumns);

	MatrixXcd eigenU;
	toEigenMatrix(pU, eigenU);

	MatrixXcd eigenInverseU = eigenU.transpose().conjugate();

	MatrixXcd eigenDiag = MatrixXcd::Zero(nbRows, nbColumns);

	for(int i = 0; i < nbRows; i++) {
		eigenDiag(i,i) = diag[i];
	}

	MatrixXcd eigenProduct = eigenU * eigenDiag * eigenInverseU;

	prRoot = fromEigenMatrix(eigenProduct, "");
}

void SampleMatrixOperator::specialUnitaryFromUnitary(MatrixPtr pU, MatrixPtrRef prSU) {
	MatrixXcd eigenU;
	toEigenMatrix(pU, eigenU);
	ComplexVal detU = eigenU.determinant();

	//detU = +-1

	if(std::abs(detU.real() - 1.0) < 1e-12) {
		prSU = fromEigenMatrix(eigenU, pU->getLabel());
	}
	else {
		int nbRows, nbColumns;
		pU->getSize(nbRows, nbColumns);
		ComplexVal detU = det(pU);
		ComplexVal c = std::pow(detU, -1.0 / (nbRows));
		//ComplexVal c = std::exp(ComplexVal(0,-1) * M_PI / (nbRows + 0.0));
		MatrixXcd eigenSU = eigenU * c;
		prSU = fromEigenMatrix(eigenSU, pU->getLabel());
	}
}

void SampleMatrixOperator::getTracelessHermitianMatricesBasis(int dimension, MatrixPtrVector& pBasis) {
	//Ref. http://mathworld.wolfram.com/GeneralizedGell-MannMatrix.html

	pBasis.clear();

	ComplexValArray pBuffer = new ComplexVal[dimension * dimension];
	std::fill(pBuffer, pBuffer + dimension * dimension, ComplexVal(0.0, 0.0));

	//Symmetric basis
	for(int r = 0; r < dimension; r++) {
		for(int c = r + 1; c < dimension; c++) {
			//sym[r][c] = sym[c][r] = 1
			int indexRC = r * dimension + c;
			int indexCR = c * dimension + r;
			pBuffer[indexRC] = 1.0;
			pBuffer[indexCR] = 1.0;

			MatrixPtr pSymmetricBasis = m_pMatrixFactory->getMatrixFromValues(dimension, dimension, pBuffer, ROW_SPLICE, "");
			pBasis.push_back(pSymmetricBasis);

			//Restore buffer
			pBuffer[indexRC] = 0.0;
			pBuffer[indexCR] = 0.0;
		}
	}
	//Asymmetric basis
	for(int r = 0; r < dimension; r++) {
		for(int c = r + 1; c < dimension; c++) {
			//asym[r][c] = -i
			//asym[c][r] = i
			int indexRC = r * dimension + c;
			int indexCR = c * dimension + r;
			pBuffer[indexRC] = ComplexVal(0, -1);
			pBuffer[indexCR] = ComplexVal(0, 1);

			MatrixPtr pAsymmetricBasis = m_pMatrixFactory->getMatrixFromValues(dimension, dimension, pBuffer, ROW_SPLICE, "");
			pBasis.push_back(pAsymmetricBasis);

			//Restore buffer
			pBuffer[indexRC] = 0.0;
			pBuffer[indexCR] = 0.0;
		}
	}
	//Diagonal basis
	for(int l = 0; l < dimension - 1; l++) {
		//diag(l) = sqrt(2 / ((l + 1) * (l + 2))) * diag(1,1...-(l+1) (at index l),0..0)
		double pref = std::sqrt(2.0 / ((l + 1) * (l + 2)));

		for(int j = 0; j <= l; j++) {
			int indexJ = j * dimension + j;
			pBuffer[indexJ] = ComplexVal(pref, 0.0);
		}

		int indexL = (l + 1) * dimension + l + 1;
		pBuffer[indexL] = ComplexVal(-pref * (l + 1.0), 0);

		MatrixPtr pDiagonalBasis = m_pMatrixFactory->getMatrixFromValues(dimension, dimension, pBuffer, ROW_SPLICE, "");
		pBasis.push_back(pDiagonalBasis);

		//Restore buffer
		std::fill(pBuffer, pBuffer + dimension * dimension, ComplexVal(0.0, 0.0));
	}

	delete[] pBuffer;
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

	delete[] arr;

	return pMatrix;
}

void SampleMatrixOperator::fromEigenMatrix(MatrixXcd& eigenMatrix, int column, ComplexVectorRef rComplexVector) {
	int numberOfEigenValues = eigenMatrix.rows();
	for(int i = 0; i < numberOfEigenValues; i++) {
		ComplexVal val = eigenMatrix.col(column)[i];
		rComplexVector.push_back(val);
	}
}


