/*
 * TestSuite.cpp
 *
 *  Created on: May 14, 2015
 *      Author: pham
 */

#include "TestSuite.h"
#include "MatrixTraceDistanceCalculator.h"
#include "CoordinateOnOrthonormalBasisCalculatorImpl.h"
#include "MatrixRealInnerProductByTraceImpl.h"
#include "CoordinateOnOrthonormalBasisCalculatorImpl.cpp"
#include "SpecialUnitaryMatrixCoordinateMapper.h"
#include <iostream>
#include <cstdio>

void initPauliMatrices(MatrixPtrVector& pPauliMatrices);
void releasePauliMatrices(MatrixPtrVector& pPauliMatrices);
void calculateSpecialUnitaryFromTracelessHermitianCoordinates(MatrixOperatorPtr pMatrixOperator, std::vector<double> coordinates, MatrixPtrVector pHermitianBasis, MatrixPtrRef pU);

TestSuite::TestSuite() {
	m_pMatrixFactory = new SimpleDenseMatrixFactoryImpl();
	m_pMatrixOperator = new SampleMatrixOperator(m_pMatrixFactory);
	m_pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
	m_pMatrixRealInnerProductCalculator = new MatrixRealInnerProductByTraceImpl(m_pMatrixOperator);
	initPauliMatrices(m_pPauliMatrices);
	m_pMatrixRealCoordinateOnOrthonormalBasisCalculator = new CoordinateOnOrthonormalBasisCalculatorImpl<MatrixPtr, double>(m_pMatrixRealInnerProductCalculator, m_pPauliMatrices);
	m_pSpecialUnitaryCoordinateCalculator = new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, m_pMatrixRealCoordinateOnOrthonormalBasisCalculator);
}

TestSuite::~TestSuite() {
	delete m_pSpecialUnitaryCoordinateCalculator;
	delete m_pMatrixRealCoordinateOnOrthonormalBasisCalculator;
	releasePauliMatrices(m_pPauliMatrices);
	delete m_pMatrixRealInnerProductCalculator;
	delete m_pMatrixDistanceCalculator;
	delete m_pMatrixOperator;
	delete m_pMatrixFactory;
}

void TestSuite::test() {
	testMatrixGenerator();
	testMatrixAdd();
	testMatrixMultiply();
	testMatrixExp();
	testMatrixInverse();
	testMultiplyScalar();
	testConjugate();
	testTranspose();
	testTrace();
	testPower();
	testSqrt();
	testEig();
	testDistanceCalculator();
	testMatrixTraceInnerProduct();
	testCoordinateOnOrthonormalBasisCalculator();
	testTracelessHermitianBasis();
	testSpecializeUnitary();
	testSimpleUnitaryCoordinateMapper();
	testSpecialUnitaryCoordinateMapper();
}

void TestSuite::testMatrixGenerator() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	array[0] = ComplexVal(1,0) * inverSqrt2;
	array[1] = ComplexVal(3,0) * inverSqrt2;
	array[2] = ComplexVal(1,0) * inverSqrt2;
	array[3] = ComplexVal(-1,0) * inverSqrt2;

	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(array, ROW_SPLICE, 2, 2, "H");

	assert(pMatrixH->getLabel().compare("H") == 0);

	assert(pMatrixH->getValue(0,0) == array[0]);
	assert(pMatrixH->getValue(0,1) == array[1]);
	assert(pMatrixH->getValue(1,0) == array[2]);
	assert(pMatrixH->getValue(1,1) == array[3]);

	int nbRows, nbColumns;
	pMatrixH->getSize(nbRows, nbColumns);

	assert(nbRows == 2 && nbColumns == 2);

	_destroy(pMatrixH);

	delete[] array;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testMatrixAdd() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	array1[0] = ComplexVal(1,0) * inverSqrt2;
	array1[1] = ComplexVal(1,0) * inverSqrt2;
	array1[2] = ComplexVal(1,0) * inverSqrt2;
	array1[3] = ComplexVal(-1,0) * inverSqrt2;

	MatrixPtr pMatrix1 = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 2, 2, "H1");

	ComplexValArray array2 = new ComplexVal[4];
	array2[0] = ComplexVal(1,0) * inverSqrt2;
	array2[1] = ComplexVal(1,0) * inverSqrt2;
	array2[2] = ComplexVal(1,0) * inverSqrt2;
	array2[3] = ComplexVal(-1,0) * inverSqrt2;

	MatrixPtr pMatrix2 = new SimpleDenseMatrixImpl(array2, ROW_SPLICE, 2, 2, "H2");

	MatrixPtr pSumMatrix;
	m_pMatrixOperator->add(pMatrix1, pMatrix2, pSumMatrix);

	assert(pSumMatrix != NullPtr);

	int nbRows, nbColumns;
	pSumMatrix->getSize(nbRows, nbColumns);

	assert(nbRows == 2 && nbColumns == 2);

	assert(pSumMatrix->getValue(0,0) == array1[0] + array2[0]);
	assert(pSumMatrix->getValue(0,1) == array1[1] + array2[1]);
	assert(pSumMatrix->getValue(1,0) == array1[2] + array2[2]);
	assert(pSumMatrix->getValue(1,1) == array1[3] + array2[3]);

	delete pSumMatrix;
	delete pMatrix2;
	delete[] array2;
	delete pMatrix1;
	delete[] array1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testMatrixMultiply() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	array1[0] = ComplexVal(1,0) * inverSqrt2;
	array1[1] = ComplexVal(1,0) * inverSqrt2;
	array1[2] = ComplexVal(1,0) * inverSqrt2;
	array1[3] = ComplexVal(-1,0) * inverSqrt2;

	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 2, 2, "H");

	MatrixPtr pI = NullPtr;

	m_pMatrixOperator->multiply(pMatrixH, pMatrixH, pI);

	assert(pI != NullPtr);

	int nbRows, nbColumns;
	pI->getSize(nbRows, nbColumns);

	assert(nbRows == 2 && nbColumns == 2);

	assert(norm(pI->getValue(0,0) - ComplexVal(1,0)) < 1e-30);
	assert(norm(pI->getValue(0,1) - ComplexVal(0,0)) < 1e-30);
	assert(norm(pI->getValue(1,0) - ComplexVal(0,0)) < 1e-30);
	assert(norm(pI->getValue(1,1) - ComplexVal(1,0)) < 1e-30);

	delete pI;
	delete pMatrixH;
	delete[] array1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testMatrixExp() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[4];
	ComplexVal i = ComplexVal(0,1);

	array1[0] = i / 2.0;
	array1[1] = i / 2.0;
	array1[2] = i / 2.0;
	array1[3] = -i / 2.0;

	MatrixPtr pMatrix1 = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 2, 2, "H1");

	MatrixPtr pExpMatrix;

	m_pMatrixOperator->exponential(pMatrix1, pExpMatrix);

	assert(pExpMatrix != NullPtr);

	int nbRows, nbColumns;
	pExpMatrix->getSize(nbRows, nbColumns);

	assert(nbRows == 2 && nbColumns == 2);

	assert(norm(m_pMatrixOperator->det(pExpMatrix) - ComplexVal(1,0)) < 1e-30);

	delete pExpMatrix;
	delete pMatrix1;
	delete[] array1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testMatrixInverse() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	array1[0] = ComplexVal(1,0) * inverSqrt2;
	array1[1] = ComplexVal(1,0) * inverSqrt2;
	array1[2] = ComplexVal(1,0) * inverSqrt2;
	array1[3] = ComplexVal(-1,0) * inverSqrt2;

	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 2, 2, "H");

	MatrixPtr pInverseH = NullPtr;
	m_pMatrixOperator->inverse(pMatrixH, pInverseH);

	assert(pInverseH != NullPtr);

	int nbRows, nbColumns;
	pInverseH->getSize(nbRows, nbColumns);

	assert(nbRows == 2 && nbColumns == 2);

	assert(norm(pInverseH->getValue(0,0) - array1[0]) < 1e-30);
	assert(norm(pInverseH->getValue(0,1) - array1[1]) < 1e-30);
	assert(norm(pInverseH->getValue(1,0) - array1[2]) < 1e-30);
	assert(norm(pInverseH->getValue(1,1) - array1[3]) < 1e-30);

	delete pInverseH;
	delete pMatrixH;
	delete[] array1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testMultiplyScalar() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array0 = new ComplexVal[4];

	array0[0] = 1;
	array0[1] = 1;
	array0[2] = 1;
	array0[3] = -1;

	ComplexValArray array1 = new ComplexVal[4];
	ComplexVal i = ComplexVal(0,1);

	array1[0] = i / 2.0;
	array1[1] = i / 2.0;
	array1[2] = i / 2.0;
	array1[3] = -i / 2.0;

	MatrixPtr pMatrix0 = new SimpleDenseMatrixImpl(array0, ROW_SPLICE, 2, 2, "H1");

	MatrixPtr pMultipliedByScalar = NullPtr;
	m_pMatrixOperator->multiplyScalar(pMatrix0, ComplexVal(0,0.50), pMultipliedByScalar);

	assert(norm(pMultipliedByScalar->getValue(0,0) - array1[0]) < 1e-30);
	assert(norm(pMultipliedByScalar->getValue(0,1) - array1[1]) < 1e-30);
	assert(norm(pMultipliedByScalar->getValue(1,0) - array1[2]) < 1e-30);
	assert(norm(pMultipliedByScalar->getValue(1,1) - array1[3]) < 1e-30);

	delete pMultipliedByScalar;
	delete[] array1;
	delete pMatrix0;
	delete[] array0;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testConjugate() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[4];

	//[1 3+i; 2+i 4-i] -> [1 2-i;3-i 4+i]

	array1[0] = 1.0;
	array1[1] = ComplexVal(3,1);
	array1[2] = ComplexVal(2,1);
	array1[3] = ComplexVal(4,-1);

	MatrixPtr pMatrix1 = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 2, 2, "H1");

	MatrixPtr pConjugate = NullPtr;

	m_pMatrixOperator->conjugateTranpose(pMatrix1, pConjugate);

	assert(pConjugate != NullPtr);

	int nbRows, nbColumns;
	pConjugate->getSize(nbRows, nbColumns);

	assert(nbRows == 2 && nbColumns == 2);

	assert(norm(pConjugate->getValue(0,0) - ComplexVal(1,0)) < 1e-30);
	assert(norm(pConjugate->getValue(0,1) - ComplexVal(2,-1)) < 1e-30);
	assert(norm(pConjugate->getValue(1,0) - ComplexVal(3,-1)) < 1e-30);
	assert(norm(pConjugate->getValue(1,1) - ComplexVal(4,1)) < 1e-30);

	delete pConjugate;
	delete pMatrix1;
	delete[] array1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testTranspose() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[4];

	//[1 3+i; 2+i 4-i] -> [1 2+i;3+i 4-i]

	array1[0] = 1.0;
	array1[1] = ComplexVal(3,1);
	array1[2] = ComplexVal(2,1);
	array1[3] = ComplexVal(4,-1);

	MatrixPtr pMatrix1 = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 2, 2, "H1");

	MatrixPtr pTranspose = NullPtr;

	m_pMatrixOperator->transpose(pMatrix1, pTranspose);

	assert(pTranspose != NullPtr);

	int nbRows, nbColumns;
	pTranspose->getSize(nbRows, nbColumns);

	assert(nbRows == 2 && nbColumns == 2);

	assert(norm(pTranspose->getValue(0,0) - ComplexVal(1,0)) < 1e-30);
	assert(norm(pTranspose->getValue(0,1) - ComplexVal(2,1)) < 1e-30);
	assert(norm(pTranspose->getValue(1,0) - ComplexVal(3,1)) < 1e-30);
	assert(norm(pTranspose->getValue(1,1) - ComplexVal(4,-1)) < 1e-30);

	delete pTranspose;
	delete pMatrix1;
	delete[] array1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testTrace() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[9];

	//[1 3+i  2+i; 4-i 5 + 2i 1 + 3i; 3 9 10-3i]

	array1[0] = 1.0;
	array1[1] = ComplexVal(3,1);
	array1[2] = ComplexVal(2,1);
	array1[3] = ComplexVal(4,-1);
	array1[4] = ComplexVal(5,2);
	array1[5] = ComplexVal(1,3);
	array1[6] = 3.0;
	array1[7] = 9.0;
	array1[8] = ComplexVal(10,-3);

	MatrixPtr pMatrix1 = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 3, 3, "H1");

	ComplexVal trace = m_pMatrixOperator->trace(pMatrix1);

	assert(trace == array1[0] + array1[4] + array1[8]);

	delete pMatrix1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testPower() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[9];

	//[1 3+i  2+i; 4-i 5 + 2i 1 + 3i; 3 9 10-3i]

	array1[0] = 1.0;
	array1[1] = ComplexVal(3,1);
	array1[2] = ComplexVal(2,1);
	array1[3] = ComplexVal(4,-1);
	array1[4] = ComplexVal(5,2);
	array1[5] = ComplexVal(1,3);
	array1[6] = 3.0;
	array1[7] = 9.0;
	array1[8] = ComplexVal(10,-3);

	MatrixPtr pMatrix1 = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 3, 3, "H1");

	MatrixPtr pPower3 = NullPtr;

	m_pMatrixOperator->power(pMatrix1, 3, pPower3);

	assert(norm(pPower3->getValue(0,0) - ComplexVal(252,99)) < 1e-30);
	assert(norm(pPower3->getValue(0,1) - ComplexVal(409,340)) < 1e-30);
	assert(norm(pPower3->getValue(0,2) - ComplexVal(302,226)) < 1e-30);
	assert(norm(pPower3->getValue(1,0) - ComplexVal(330,298)) < 1e-30);
	assert(norm(pPower3->getValue(1,1) - ComplexVal(438,802)) < 1e-30);
	assert(norm(pPower3->getValue(1,2) - ComplexVal(354,607)) < 1e-30);
	assert(norm(pPower3->getValue(2,0) - ComplexVal(957,-276)) < 1e-30);
	assert(norm(pPower3->getValue(2,1) - ComplexVal(1911,3)) < 1e-30);
	assert(norm(pPower3->getValue(2,2) - ComplexVal(1288,-159)) < 1e-30);

	delete pPower3;
	delete pMatrix1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSqrt() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[4];

	//[7 10;15 22]

	array1[0] = 7.0;
	array1[1] = 10.0;
	array1[2] = 15.0;
	array1[3] = 22.0;

	MatrixPtr pMatrix1 = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 2, 2, "H1");

	MatrixPtr pSqrt = NullPtr;

	//[1.5667    1.7408; 2.6112    4.1779]
	m_pMatrixOperator->sqrt(pMatrix1, pSqrt);

	assert(norm(pSqrt->getValue(0,0) - ComplexVal(1.5667,0)) < 1e-4);
	assert(norm(pSqrt->getValue(0,1) - ComplexVal(1.7408,0)) < 1e-4);
	assert(norm(pSqrt->getValue(1,0) - ComplexVal(2.6112,0)) < 1e-4);
	assert(norm(pSqrt->getValue(1,1) - ComplexVal(4.1779,0)) < 1e-4);

	delete pSqrt;
	delete pMatrix1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testEig() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray arrayX = new ComplexVal[4];

	//[0 1; 1 0]

	arrayX[0] = 0.0;
	arrayX[1] = 1.0;
	arrayX[2] = 1.0;
	arrayX[3] = 0.0;

	MatrixPtr pX = new SimpleDenseMatrixImpl(arrayX, ROW_SPLICE, 2, 2, "X");

	ComplexVector eigenValues;
	MatrixPtr pEigenVectors = NullPtr;

	m_pMatrixOperator->eig(pX, eigenValues, pEigenVectors);

	assert(eigenValues.size() == 2);

	assert(std::abs(eigenValues[0] - 1.0) < 1e-15 || std::abs(eigenValues[0] + 1.0) < 1e-15);
	assert(std::abs(eigenValues[0] + eigenValues[1]) < 1e-30);

	assert(std::abs(pEigenVectors->getValue(0,0) - 1/sqrt(2.0)) < 1e-15 || std::abs(pEigenVectors->getValue(0,0) + 1/sqrt(2.0))< 1e-15);
	assert(std::abs(pEigenVectors->getValue(0,0) + pEigenVectors->getValue(0,1)) < 1e-15);
	assert(std::abs(pEigenVectors->getValue(1,0) - 1/sqrt(2.0)) < 1e-15);
	assert(std::abs(pEigenVectors->getValue(1,1) - 1/sqrt(2.0)) < 1e-15);

	delete pEigenVectors;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testDistanceCalculator() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	double inverSqrt2 = 1 / sqrt(2);
	ComplexVal arrayH[] = {ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(1,0) * inverSqrt2, ComplexVal(-1,0) * inverSqrt2};
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexVal arrayT[] = {ComplexVal(1,0), 0.0, 0.0, std::exp(ComplexVal(0,1) * M_PI / 4.0)};
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	MatrixPtr pI = m_pMatrixFactory->getIdentityMatrix(2);

	double dHI = m_pMatrixDistanceCalculator->distance(pMatrixH, pI);
	double dHH = m_pMatrixDistanceCalculator->distance(pMatrixH, pMatrixH);
	double dTI = m_pMatrixDistanceCalculator->distance(pMatrixT, pI);

	assert(std::abs(dHH) < 1e-30);
	assert(std::abs(dHI - 2.0) < 1e-30);
	assert(std::abs(dTI - 0.76537) < 1e-5);

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testMatrixTraceInnerProduct() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray array1 = new ComplexVal[4];

	//[1 4-i; 4 + i 2]

	array1[0] = 1.0;
	array1[1] = ComplexVal(4, -1);
	array1[2] = ComplexVal(4, 1);
	array1[3] = 2.0;

	MatrixPtr pH1 = new SimpleDenseMatrixImpl(array1, ROW_SPLICE, 2, 2, "H1");

	ComplexValArray array2 = new ComplexVal[4];

	//[3 2+i; 2-i 2]

	array2[0] = 3.0;
	array2[1] = ComplexVal(2, 1);
	array2[2] = ComplexVal(2, -1);
	array2[3] = 2.0;

	MatrixPtr pH2 = new SimpleDenseMatrixImpl(array2, ROW_SPLICE, 2, 2, "H1");

	double traceInnerProduct = 0;
	m_pMatrixRealInnerProductCalculator->innerProduct(pH1, pH2, traceInnerProduct);

	assert(std::abs(traceInnerProduct - 21.0) < 1e-30);

	delete pH2;
	delete pH1;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testCoordinateOnOrthonormalBasisCalculator() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray arrayH = new ComplexVal[4];

	//H = 3 * X + 4 *Y -5 *Z

	arrayH[0] = -5.0;
	arrayH[1] = ComplexVal(3, -4);
	arrayH[2] = ComplexVal(3, 4);
	arrayH[3] = 5.0;

	MatrixPtr pH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H1");

	MatrixRealCoordinatePtr pHRealCoordinate = NullPtr;
	m_pMatrixRealCoordinateOnOrthonormalBasisCalculator->calulateElementCoordinate(pH, pHRealCoordinate);

	std::vector<double> hCoordinates = pHRealCoordinate->getCoordinates();

	assert(hCoordinates.size() == 3);
	assert(std::abs(hCoordinates[0] - 3) < 1e-30);
	assert(std::abs(hCoordinates[1] - 4) < 1e-30);
	assert(std::abs(hCoordinates[2] - -5) < 1e-30);

	delete pHRealCoordinate;
	delete pH;

	MatrixRealCoordinatePtr pZRealCoordinate = NullPtr;
	m_pMatrixRealCoordinateOnOrthonormalBasisCalculator->calulateElementCoordinate(m_pPauliMatrices[2], pZRealCoordinate);

	std::vector<double> zCoordinates = pZRealCoordinate->getCoordinates();

	assert(zCoordinates.size() == 3);
	assert(std::abs(zCoordinates[0] - 0) < 1e-30);
	assert(std::abs(zCoordinates[1] - 0) < 1e-30);
	assert(std::abs(zCoordinates[2] - 1) < 1e-30);

	delete pZRealCoordinate;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testTracelessHermitianBasis() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	MatrixPtrVector pBasis2;
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(2, pBasis2);

	assert(pBasis2.size() == 3);

	//X
	assert(m_pMatrixDistanceCalculator->distance(pBasis2[0], m_pPauliMatrices[0]) < 1e-8);

	//Y
	assert(m_pMatrixDistanceCalculator->distance(pBasis2[1], m_pPauliMatrices[1]) < 1e-8);

	//Z
	assert(m_pMatrixDistanceCalculator->distance(pBasis2[2], m_pPauliMatrices[2]) < 1e-8);

	releasePauliMatrices(pBasis2);

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSpecializeUnitary() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexVal arrayCNOT[] = {1.0, 0.0, 0.0, 0.0
			, 0.0, 1.0, 0.0, 0.0
			, 0.0, 0.0, 0.0, 1.0
			, 0.0, 0.0, 1.0, 0.0};

	MatrixPtr pCNOT = new SimpleDenseMatrixImpl(arrayCNOT, ROW_SPLICE, 4, 4, "CNOT");
	MatrixPtr pSCNOT = NullPtr;
	m_pMatrixOperator->specialUnitaryFromUnitary(pCNOT, pSCNOT);

	for(int r = 0; r < 4; r++) {
		for(int c = 0; c < 4; c++) {
			ComplexVal v = pSCNOT->getValue(r, c);
			if((r == 0 && c == 0) || (r == 1 && c == 1) || (r == 2 && c == 3) || (r == 3 && c == 2)) {
				assert(norm(v -  ComplexVal(1/sqrt(2), -1/sqrt(2))) < 1e-20);
			}
			else {
				assert(norm(v) < 1e-20);
			}
		}
	}

	delete(pCNOT);
	delete(pSCNOT);

	ComplexVal arrayT[] = {ComplexVal(1,0), 0.0, 0.0, std::exp(ComplexVal(0,1) * M_PI / 4.0)};
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");
	MatrixPtr pMatrixST = NullPtr;

	m_pMatrixOperator->specialUnitaryFromUnitary(pMatrixT, pMatrixST);

	ComplexVal st00 = pMatrixST->getValue(0,0);
	ComplexVal st01 = pMatrixST->getValue(0,1);
	ComplexVal st10 = pMatrixST->getValue(1,0);
	ComplexVal st11 = pMatrixST->getValue(1,1);

	assert(norm(st00 -  std::exp(ComplexVal(0,-1) * M_PI / 8.0)) < 1e-20);
	assert(norm(st01 -  0.0) < 1e-20);
	assert(norm(st10 -  0.0) < 1e-20);
	assert(norm(st11 -  std::exp(ComplexVal(0,1) * M_PI / 8.0)) < 1e-20);

	delete pMatrixST;
	delete pMatrixT;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSimpleUnitaryCoordinateMapper() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	ComplexValArray arrayH = new ComplexVal[4];

	//H = 0.3 * X + 0.4 *Y - 0.5 *Z

	arrayH[0] = -0.5;
	arrayH[1] = ComplexVal(0.3, -0.4);
	arrayH[2] = ComplexVal(0.3, 0.4);
	arrayH[3] = 0.5;

	MatrixPtr pH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H1");

	MatrixPtr pHi = NullPtr;
	m_pMatrixOperator->multiplyScalar(pH, ComplexVal(0,1), pHi);

	MatrixPtr pU = NullPtr;
	m_pMatrixOperator->exponential(pHi, pU);

	MatrixRealCoordinatePtr pURealCoordinate = NullPtr;

	m_pSpecialUnitaryCoordinateCalculator->calulateElementCoordinate(pU, pURealCoordinate);
	std::vector<double> uCoordinates = pURealCoordinate->getCoordinates();

	assert(uCoordinates.size() == 3);
	assert(std::abs(uCoordinates[0] - 0.3) < 1e-15);
	assert(std::abs(uCoordinates[1] - 0.4) < 1e-15);
	assert(std::abs(uCoordinates[2] - -0.5) < 1e-15);

	delete pURealCoordinate;

	delete pU;

	delete pHi;

	delete[] arrayH;
	delete pH;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void TestSuite::testSpecialUnitaryCoordinateMapper() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;

	MatrixPtrVector pBasis4;
	m_pMatrixOperator->getTracelessHermitianMatricesBasis(4, pBasis4);

	assert(pBasis4.size() == 15);


	std::vector<double> coordArr = {0.25,0.5,0.5,0.5,0.5,0.2,0.5,0.5,0.3,0.5,0.5,0.1,-0.5,0.5,0.5};
	//std::vector<double> coordArr = {2.0,1.0,3.0,1.0,-1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

	MatrixPtr pU = NullPtr;

	calculateSpecialUnitaryFromTracelessHermitianCoordinates(m_pMatrixOperator, coordArr, pBasis4, pU);

	MatrixRealCoordinateCalculatorPtr pHermitianCoordinateCalculator = new CoordinateOnOrthonormalBasisCalculatorImpl<MatrixPtr, double>(m_pMatrixRealInnerProductCalculator, pBasis4);
	MatrixRealCoordinateCalculatorPtr pSpecialUnitaryCoordinateCalculator = new SpecialUnitaryMatrixCoordinateMapper(m_pMatrixOperator, pHermitianCoordinateCalculator);

	MatrixRealCoordinatePtr pURealCoordinate = NullPtr;

	pSpecialUnitaryCoordinateCalculator->calulateElementCoordinate(pU, pURealCoordinate);
	std::vector<double> uCoordinates = pURealCoordinate->getCoordinates();

	assert(uCoordinates.size() == 15);

	bool maintainInitialCoordinateFlag = true;
	for(int i = 0; i < 15; i++) {
		if(!abs(uCoordinates[i] - coordArr[i]) < 1e-15) {
			maintainInitialCoordinateFlag = false;
		}
	}

	//Handle the case some adjustments have been applied to keep the traceless property
	if(!maintainInitialCoordinateFlag) {
		std::cout << "Need further verification..." << std::endl;
		/*for(int i = 0; i < 15; i++) {
			printf("uCoord[%d] = %lf\n", i, uCoordinates[i]);
		}*/
		MatrixPtr pU1 = NullPtr;
		calculateSpecialUnitaryFromTracelessHermitianCoordinates(m_pMatrixOperator, uCoordinates, pBasis4, pU1);
		assert(m_pMatrixDistanceCalculator->distance(pU1, pU) < 1e-8);
	}

	delete pURealCoordinate;

	delete pSpecialUnitaryCoordinateCalculator;

	delete pHermitianCoordinateCalculator;

	delete pU;

	releasePauliMatrices(pBasis4);

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void initPauliMatrices(MatrixPtrVector& pPauliMatrices) {
	//Pauli X = [0 1;1 0]
	//Pauli Y = [0 -i;i 0]
	//Pauli Z = [1 0;0 -1]
	ComplexValArray arrayX = new ComplexVal[4];

	//[0 1; 1 0]

	arrayX[0] = 0.0;
	arrayX[1] = 1.0;
	arrayX[2] = 1.0;
	arrayX[3] = 0.0;

	MatrixPtr pX = new SimpleDenseMatrixImpl(arrayX, ROW_SPLICE, 2, 2, "X");
	pPauliMatrices.push_back(pX);

	ComplexValArray arrayY = new ComplexVal[4];

	//[0 -i;i 0]

	arrayY[0] = 0.0;
	arrayY[1] = ComplexVal(0, -1);
	arrayY[2] = ComplexVal(0, 1);
	arrayY[3] = 0.0;

	MatrixPtr pY = new SimpleDenseMatrixImpl(arrayY, ROW_SPLICE, 2, 2, "Y");
	pPauliMatrices.push_back(pY);

	ComplexValArray arrayZ = new ComplexVal[4];

	//[1 0;0 -1]

	arrayZ[0] = 1.0;
	arrayZ[1] = 0.0;
	arrayZ[2] = 0.0;
	arrayZ[3] = -1.0;

	MatrixPtr pZ = new SimpleDenseMatrixImpl(arrayZ, ROW_SPLICE, 2, 2, "Z");
	pPauliMatrices.push_back(pZ);
}

void releasePauliMatrices(MatrixPtrVector& pPauliMatrices) {
	for(MatrixPtrVector::iterator pIter = pPauliMatrices.begin(); pIter != pPauliMatrices.end();) {
		MatrixPtr pMatrix = *pIter;
		pIter = pPauliMatrices.erase(pIter);
		delete pMatrix;
	}
}

void calculateSpecialUnitaryFromTracelessHermitianCoordinates(MatrixOperatorPtr pMatrixOperator, std::vector<double> coordinates, MatrixPtrVector pHermitianBasis, MatrixPtrRef pU) {
	ComplexVal zeroArr[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	MatrixPtr pH = new SimpleDenseMatrixImpl(zeroArr, ROW_SPLICE, 4, 4, "Z");

	//H = 1/2 * (basis4[0]+ basis4[1] + ... + basis4[14])
	for(int i = 0; i < 15; i++) {
		MatrixPtr pPartialSum = NullPtr;
		pMatrixOperator->multiplyScalar(pHermitianBasis[i], ComplexVal(coordinates[i], 0), pPartialSum);
		MatrixPtr pSum = NullPtr;
		pMatrixOperator->add(pH, pPartialSum, pSum);
		delete pPartialSum;
		delete pH;
		pH = pSum;
	}

	MatrixPtr pHi = NullPtr;
	pMatrixOperator->multiplyScalar(pH, ComplexVal(0,1), pHi);

	pMatrixOperator->exponential(pHi, pU);

	delete pHi;

	delete pH;
}
