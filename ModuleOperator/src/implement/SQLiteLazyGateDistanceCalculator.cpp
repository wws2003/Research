/*
 * SQLiteLazyGateDistanceCalculator.cpp
 *
 *  Created on: Dec 12, 2015
 *      Author: pham
 */

#include "SQLiteLazyGateDistanceCalculator.h"
#include <istream>

const char* SQLiteLazyGateDistanceCalculator::MATRIX_TABLE_NAME = "matrices";
const char* SQLiteLazyGateDistanceCalculator::MATRIX_VALUES_ROW_NAME = "vals";
const char* SQLiteLazyGateDistanceCalculator::MATRIX_LABEL_ROW_NAME = "label";

/*--------- Private methods' declarations--------- */

void readComplexVal(std::istream &inputStream, ComplexVal* pCVal) ;

/*--------- End of private methods' declarations--------- */

SQLiteLazyGateDistanceCalculator::SQLiteLazyGateDistanceCalculator(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator,
		MatrixFactoryPtr pMatrixFactory,
		std::string dbFileName,
		int matrixSize) : LazyGateDistanceCalculatorImpl(pMatrixDistanceCalculator, NullPtr, NullPtr) {

	m_pMatrixFactory = pMatrixFactory;
	m_matrixSize = matrixSize;
	m_dbOpennedHere = true;
	openDB(dbFileName);
	prepareSelectStatements();
}

SQLiteLazyGateDistanceCalculator::SQLiteLazyGateDistanceCalculator(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator,
		MatrixFactoryPtr pMatrixFactory,
		sqlite3* pDb,
		int matrixSize) : LazyGateDistanceCalculatorImpl(pMatrixDistanceCalculator, NullPtr, NullPtr) {

	m_pMatrixFactory = pMatrixFactory;
	m_matrixSize = matrixSize;
	m_dbOpennedHere = false;
	m_pDb = pDb;
	prepareSelectStatements();
}

SQLiteLazyGateDistanceCalculator::~SQLiteLazyGateDistanceCalculator() {
	sqlite3_finalize(m_pStatement);
	if (m_dbOpennedHere) {
		closeDB();
	}
}

void SQLiteLazyGateDistanceCalculator::provideMatrixToGate(GatePtr pGate) {
	MatrixPtr pMatrix = getMatrixFromLabel(pGate->getLabelStr());
	pGate->setMatrix(pMatrix);
}

void SQLiteLazyGateDistanceCalculator::openDB(std::string dbFileName) {
	int rc;

	rc = sqlite3_open_v2(dbFileName.c_str(), &m_pDb, SQLITE_OPEN_READONLY, NULL);
	if (rc) {
		fprintf(stderr, "Can not open DB due to error %d\n", rc);
		perror("Can not open database");
		throw (1);
	}
}

void SQLiteLazyGateDistanceCalculator::prepareSelectStatements() {
	char selectQuery[512];
	sprintf(selectQuery, "SELECT %s FROM %s WHERE (%s = ?);",
			MATRIX_VALUES_ROW_NAME,
			MATRIX_TABLE_NAME,
			MATRIX_LABEL_ROW_NAME);

	sqlite3_prepare_v2(m_pDb, selectQuery,
			strlen(selectQuery),
			&m_pStatement, NULL);
}

MatrixPtr SQLiteLazyGateDistanceCalculator::getMatrixFromLabel(std::string label) {

	std::istringstream iss(queryMatrixByLabel(label));

	if(iss.str().empty()) {
		return NullPtr;
	}

	//Read matrix 's values
	int arraySize = m_matrixSize * m_matrixSize;
	for(int i = 0; i < arraySize; i++) {
		readComplexVal(iss, &m_valBuffer[i]);
	}

	//Generate matrix instance
	MatrixPtr pMatrix = m_pMatrixFactory->getMatrixFromValues(m_matrixSize, m_matrixSize, m_valBuffer, ROW_SPLICE, "");

	return pMatrix;
}

std::string SQLiteLazyGateDistanceCalculator::queryMatrixByLabel(std::string label) {

	int rc = sqlite3_bind_text(m_pStatement,
			1,
			label.c_str(),
			-1,
			SQLITE_STATIC);

	if(rc != SQLITE_OK) {
		fprintf(stderr, "Error bind query text %s due to %s\n", sqlite3_errmsg(m_pDb), sqlite3_errstr(rc));
		throw (rc);
	}

	int result = sqlite3_step(m_pStatement);

	if(result == SQLITE_ROW) {
		const char* ret = (const char*) sqlite3_column_text(m_pStatement, 0);
		std::string valStr(ret);
		sqlite3_reset(m_pStatement);
		return valStr;
	}

	sqlite3_reset(m_pStatement);
	return "";
}

void SQLiteLazyGateDistanceCalculator::closeDB() {
	sqlite3_close(m_pDb);
}

/*--------- Private methods' definition--------- */

void readComplexVal(std::istream &inputStream, ComplexVal* pCVal) {
#if MPFR_REAL
	int defaultPrecision = inputStream.precision();
	//Increase precision of output stream for mpfr
	inputStream.precision(REAL_IO_PRECISION);
	mreal_t rVal;
	inputStream >> rVal;
	mreal_t iVal;
	inputStream >> iVal;
	pCVal->real(rVal);
	pCVal->imag(iVal);
	//Restore default precision of output stream
	inputStream.precision(defaultPrecision);

#else
	inputStream.read((char*)(pCVal), sizeof(ComplexVal));
#endif
}

/*--------- End of private methods' definition--------- */


