/*
 * SQLiteLazyGateDistanceCalculator.h
 *
 *  Created on: Dec 12, 2015
 *      Author: pham
 */

#ifndef SQLITELAZYGATEDISTANCECALCULATOR_H_
#define SQLITELAZYGATEDISTANCECALCULATOR_H_

#include "Gate.h"
#include "LazyGateDistanceCalculatorImpl.h"
#include "IMatrixFactory.h"
#include <sqlite3.h>

class SQLiteLazyGateDistanceCalculator : public LazyGateDistanceCalculatorImpl {
public:
	SQLiteLazyGateDistanceCalculator(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator,
			MatrixFactoryPtr pMatrixFactory,
			std::string dbFileName,
			int matrixSize);

	SQLiteLazyGateDistanceCalculator(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator,
			MatrixFactoryPtr pMatrixFactory,
			sqlite3* pDb,
			int matrixSize);

	virtual ~SQLiteLazyGateDistanceCalculator();

protected:
	void provideMatrixToGate(GatePtr pGate);

private:
	MatrixPtr getMatrixFromLabel(std::string label);

	void openDB(std::string dbFileName);

	void prepareSelectStatements();

	std::string queryMatrixByLabel(std::string label);

	void closeDB();

	MatrixFactoryPtr m_pMatrixFactory;
	int m_matrixSize;

	bool m_dbOpennedHere;
	sqlite3 *m_pDb;
	sqlite3_stmt * m_pStatement;

	static const char* MATRIX_TABLE_NAME;
	static const char* MATRIX_LABEL_ROW_NAME;
	static const char* MATRIX_VALUES_ROW_NAME;

	ComplexVal m_valBuffer[256];
};


#endif /* SQLITELAZYGATEDISTANCECALCULATOR_H_ */
