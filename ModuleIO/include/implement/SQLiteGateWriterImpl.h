/*
 * SQLiteGateWriterImpl.h
 *
 *  Created on: Dec 12, 2015
 *      Author: pham
 */

#ifndef SQLITEGATEWRITERIMPL_H_
#define SQLITEGATEWRITERIMPL_H_

#include "BinaryGateWriterImpl.h"
#include <sqlite3.h>

class SQLiteGateWriterImpl : public BinaryGateWriterImpl {
public:
	SQLiteGateWriterImpl(MatrixWriterPtr pMatrixWriter, std::string dbFileName);

	SQLiteGateWriterImpl(MatrixWriterPtr pMatrixWriter, sqlite3* pDb);

	virtual ~SQLiteGateWriterImpl();

protected:
	void writeMatrix(std::string label, MatrixPtr pMatrix, std::ostream& ouputStream);

private:
	void openDB(std::string dbFileName);

	void createTableIfNotExist();

	void prepareInsertStatements();

	void writeMatrixToDB(std::string label, MatrixPtr pMatrix);

	void closeDB();

	bool m_dbOpennedHere;
	sqlite3 *m_pDb;
	sqlite3_stmt * m_pStatement;

	static const char* MATRIX_TABLE_NAME;
	static const char* MATRIX_LABEL_ROW_NAME;
	static const char* MATRIX_VALUES_ROW_NAME;
	static const char* MATRIX_ROWS;
};


#endif /* SQLITEGATEWRITERIMPL_H_ */
