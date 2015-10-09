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
#include "eigen3/unsupported/Eigen/MPRealSupport"
#include <iostream>

using namespace Eigen;

#if MPFR_REAL
#define NOISE_THRESOLD 1e-33
typedef std::complex<long double> LongComplexVal;
typedef Matrix<LongComplexVal,Dynamic,Dynamic>  MatrixXcld;

void fromMpToLd(const MatrixXcmp& mMp, MatrixXcld& mLd);
void fromLdToMp(const MatrixXcld& mLd, MatrixXcmp& mMp);
#else
#define NOISE_THRESOLD 1e-12
#endif

#define real_abs(x) (x >= 0.0 ? (x) : -(x))

SampleMatrixOperator::SampleMatrixOperator(MatrixFactoryPtr pMatrixFactory) : m_pMatrixFactory(pMatrixFactory) {

}

void SampleMatrixOperator::add(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSum) {
	MatrixXcmp eigenMat1, eigenMat2;
	toEigenMatrix(pm1, eigenMat1);
	toEigenMatrix(pm2, eigenMat2);
	MatrixXcmp eigSum = eigenMat1 + eigenMat2;

	prSum = fromEigenMatrix(eigSum);

}

void SampleMatrixOperator::multiply(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prProduct) {
	MatrixXcmp eigenMat1, eigenMat2;
	toEigenMatrix(pm1, eigenMat1);
	toEigenMatrix(pm2, eigenMat2);
	MatrixXcmp eigProduct = eigenMat1 * eigenMat2;

	prProduct = fromEigenMatrix(eigProduct, pm1->getLabel() + pm2->getLabel());
}

void SampleMatrixOperator::multiplyScalar(MatrixPtr pm, ComplexVal scalar, MatrixPtrRef prProduct) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);
	eigenMat *= scalar;
	prProduct = fromEigenMatrix(eigenMat);
}

void SampleMatrixOperator::subtract(MatrixPtr pm1, MatrixPtr pm2, MatrixPtrRef prSubtract) {
	MatrixXcmp eigenMat1, eigenMat2;
	toEigenMatrix(pm1, eigenMat1);
	toEigenMatrix(pm2, eigenMat2);
	MatrixXcmp eigSubstract = eigenMat1 - eigenMat2;

	prSubtract = fromEigenMatrix(eigSubstract);
}

ComplexVal SampleMatrixOperator::det(MatrixPtr pm) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);
	return eigenMat.determinant();
}

ComplexVal SampleMatrixOperator::trace(MatrixPtr pm) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);
	return eigenMat.trace();
}

void SampleMatrixOperator::power(MatrixPtr pm, int exponent, MatrixPtrRef prPower) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);

	MatrixXcmp mpPowerMat;
#if MPFR_REAL
	//Temporary has to convert to long double matrix then convert back to mpfr
	//because eigen hasn't supported mpfr in power operation yet
	MatrixXcld ldMat;
	fromMpToLd(eigenMat, ldMat);
	MatrixXcld ldPowerMat = ldMat.pow(exponent);
	fromLdToMp(ldPowerMat, mpPowerMat);
#else
	mpPowerMat = eigenMat.pow(exponent);
#endif

	prPower = fromEigenMatrix(mpPowerMat);
}

void SampleMatrixOperator::sqrt(MatrixPtr pm, MatrixPtrRef prSqrt) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);
	MatrixXcmp eigenSqrt = eigenMat.sqrt();
	prSqrt = fromEigenMatrix(eigenSqrt);
}


void SampleMatrixOperator::conjugateTranpose(MatrixPtr pm, MatrixPtrRef prConjugate) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);
	eigenMat.transposeInPlace();
	MatrixXcmp conjugateEigenMat = eigenMat.conjugate();
	prConjugate = fromEigenMatrix(conjugateEigenMat);
}

void SampleMatrixOperator::transpose(MatrixPtr pm, MatrixPtrRef prTranspose) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);

	eigenMat.transposeInPlace();

	prTranspose = fromEigenMatrix(eigenMat);
}

void SampleMatrixOperator::inverse(MatrixPtr pm, MatrixPtrRef prInverseMatrix) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);
	MatrixXcmp eigInverse = eigenMat.inverse();
	prInverseMatrix = fromEigenMatrix(eigInverse);
}

void SampleMatrixOperator::exponential(MatrixPtr pm, MatrixPtrRef prExpMatrix) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);

	MatrixXcmp mpExpMat;
#if MPFR_REAL
	//Temporary has to convert to long double matrix then convert back to mpfr
	//because eigen hasn't supported mpfr in exp operation yet
	MatrixXcld ldMat;
	fromMpToLd(eigenMat, ldMat);
	MatrixXcld ldExpMat = ldMat.exp();
	fromLdToMp(ldExpMat, mpExpMat);
#else
	mpExpMat = eigenMat.exp();
#endif

	prExpMatrix = fromEigenMatrix(mpExpMat);
}

void SampleMatrixOperator::log(MatrixPtr pm, MatrixPtrRef prLog) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);

	MatrixXcmp mpLogMat;
#if MPFR_REAL
	//Temporary has to convert to long double matrix then convert back to mpfr
	//because eigen hasn't supported mpfr in log operation yet
	MatrixXcld ldMat;
	fromMpToLd(eigenMat, ldMat);
	MatrixXcld ldLogMat = ldMat.log();
	fromLdToMp(ldLogMat, mpLogMat);
#else
	mpLogMat = eigenMat.log();
#endif

	prLog = fromEigenMatrix(mpLogMat);
}

void SampleMatrixOperator::eig(MatrixPtr pm, ComplexVectorRef rEigVals, MatrixPtrRef prEigVects) {
	MatrixXcmp eigenMat;
	toEigenMatrix(pm, eigenMat);
	ComplexEigenSolver<MatrixXcmp> ces(eigenMat, true);
	MatrixXcmp eigenVectors = ces.eigenvectors();
	MatrixXcmp eigenValues = ces.eigenvalues();
	prEigVects = fromEigenMatrix(eigenVectors);
	fromEigenMatrix(eigenValues, 0, rEigVals);
}

void SampleMatrixOperator::unitaryDediagonalize(MatrixPtr pU, const ComplexVectorRef diag, MatrixPtrRef prRoot) {
	int nbRows, nbColumns;
	pU->getSize(nbRows, nbColumns);

	MatrixXcmp eigenU;
	toEigenMatrix(pU, eigenU);

	MatrixXcmp eigenInverseU = eigenU.transpose().conjugate();

	MatrixXcmp eigenDiag = MatrixXcmp::Zero(nbRows, nbColumns);

	for(int i = 0; i < nbRows; i++) {
		eigenDiag(i,i) = diag[i];
	}

	MatrixXcmp eigenProduct = eigenU * eigenDiag * eigenInverseU;

	prRoot = fromEigenMatrix(eigenProduct, "");
}

void SampleMatrixOperator::specialUnitaryFromUnitary(MatrixPtr pU, MatrixPtrRef prSU) {
	MatrixXcmp eigenU;
	toEigenMatrix(pU, eigenU);
	ComplexVal detU = eigenU.determinant();

	//Check if detU = +1
	mreal_t detNoise = real_abs(detU.real() - 1.0);
	if(detNoise < NOISE_THRESOLD) {
		prSU = fromEigenMatrix(eigenU, pU->getLabel());
	}
	else {
		int nbRows, nbColumns;
		pU->getSize(nbRows, nbColumns);
		ComplexVal detU = det(pU);
		ComplexVal c = pow(detU, (mreal_t)(-1.0 / (nbRows + 0.0)));
		//ComplexVal c = std::exp(ComplexVal(0,-1) * M_PI / (nbRows + 0.0));
		MatrixXcmp eigenSU = eigenU * c;
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

void SampleMatrixOperator::toEigenMatrix(MatrixPtr pMatrix, MatrixXcmp& rEigenMat) {
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);

	rEigenMat = MatrixXcmp(nbRows, nbColumns);

	for(int i = 0; i < nbRows; i++) {
		for(int j = 0; j < nbColumns; j++) {
			rEigenMat(i,j) = pMatrix->getValue(i, j);
		}
	}
}

MatrixPtr SampleMatrixOperator::fromEigenMatrix(MatrixXcmp& eigenMatrix, std::string label) {
	int nbRows = eigenMatrix.rows();
	int nbCols = eigenMatrix.cols();

	ComplexValArray arr = new ComplexVal[nbRows * nbCols];
	Map<MatrixXcmp>(arr, nbRows, nbCols) = eigenMatrix;

	MatrixPtr pMatrix = m_pMatrixFactory->getMatrixFromValues(nbRows, nbCols, arr, COLUMN_SPLICE, label);

	delete[] arr;

	return pMatrix;
}

void SampleMatrixOperator::fromEigenMatrix(MatrixXcmp& eigenMatrix, int column, ComplexVectorRef rComplexVector) {
	int numberOfEigenValues = eigenMatrix.rows();
	for(int i = 0; i < numberOfEigenValues; i++) {
		ComplexVal val = eigenMatrix.col(column)[i];
		rComplexVector.push_back(val);
	}
}

#if MPFR_REAL
void fromMpToLd(const MatrixXcmp& mMp, MatrixXcld& mLd) {
	int rows = mMp.rows();
	int columns = mMp.cols();
	mLd = MatrixXcld::Zero(rows, columns);

	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < columns; j++) {
			std::complex<mreal_t> v = mMp(i,j);
			mLd(i,j) = LongComplexVal(v.real().toLDouble(), v.imag().toLDouble());
		}
	}
}

void fromLdToMp(const MatrixXcld& mLd, MatrixXcmp& mMp) {
	int rows = mLd.rows();
	int columns = mLd.cols();
	mMp = MatrixXcmp::Zero(rows, columns);

	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < columns; j++) {
			LongComplexVal v = mLd(i,j);
			mMp(i,j) = std::complex<mreal_t>(v.real(), v.imag());
		}
	}
}
#endif

