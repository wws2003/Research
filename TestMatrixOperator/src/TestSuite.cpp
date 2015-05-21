/*
 * TestSuite.cpp
 *
 *  Created on: May 14, 2015
 *      Author: pham
 */

#include "TestSuite.h"
#include "MatrixTraceDistanceCalculator.h"

TestSuite::TestSuite() {
	m_pMatrixFactory = new SimpleDenseMatrixFactoryImpl();
	m_pMatrixOperator = new SampleMatrixOperator(m_pMatrixFactory);
	m_pMatrixDistanceCalculator = new MatrixTraceDistanceCalculator(m_pMatrixOperator);
}

TestSuite::~TestSuite() {
	delete m_pMatrixDistanceCalculator;
	delete m_pMatrixFactory;
	delete m_pMatrixOperator;
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
	testDistanceCalculator();
}

void TestSuite::testMatrixGenerator() {
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

}

void TestSuite::testMatrixAdd() {

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
}

void TestSuite::testMatrixMultiply() {

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
}

void TestSuite::testMatrixExp() {

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
}

void TestSuite::testMatrixInverse() {

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
}

void TestSuite::testMultiplyScalar() {

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
}

void TestSuite::testConjugate() {

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
}

void TestSuite::testTranspose() {

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
}

void TestSuite::testTrace() {
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
}

void TestSuite::testPower() {
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
}

void TestSuite::testSqrt() {
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
}

void TestSuite::testDistanceCalculator() {
	ComplexValArray arrayH = new ComplexVal[4];
	double inverSqrt2 = 1 / sqrt(2);
	arrayH[0] = ComplexVal(1,0) * inverSqrt2;
	arrayH[1] = ComplexVal(1,0) * inverSqrt2;
	arrayH[2] = ComplexVal(1,0) * inverSqrt2;
	arrayH[3] = ComplexVal(-1,0) * inverSqrt2;
	MatrixPtr pMatrixH = new SimpleDenseMatrixImpl(arrayH, ROW_SPLICE, 2, 2, "H");

	ComplexValArray arrayT = new ComplexVal[4];
	arrayT[0] = ComplexVal(1,0);
	arrayT[1] = 0.0;
	arrayT[2] = 0.0;
	arrayT[3] = std::exp(ComplexVal(0,1) * M_PI / 4.0);
	MatrixPtr pMatrixT = new SimpleDenseMatrixImpl(arrayT, ROW_SPLICE, 2, 2, "T");

	MatrixPtr pI = m_pMatrixFactory->getIdentityMatrix(2);

	double dHI = m_pMatrixDistanceCalculator->distance(pMatrixH, pI);
	double dHH = m_pMatrixDistanceCalculator->distance(pMatrixH, pMatrixH);
	double dTI = m_pMatrixDistanceCalculator->distance(pMatrixT, pI);

	assert(std::abs(dHH) < 1e-30);
	assert(std::abs(dHI - 2.0) < 1e-30);
	assert(std::abs(dTI - 0.76537) < 1e-5);
}

