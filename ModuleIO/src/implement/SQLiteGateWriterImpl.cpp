/*
 * SQLiteGateWriterImpl.cpp
 *
 *  Created on: Dec 12, 2015
 *      Author: pham
 */

#include "SQLiteGateWriterImpl.h"
#include  "ComplexValIO.h"
#include <cstdio>
#include <exception>
#include <sstream>

/*--------- Private methods' declarations--------- */
static int createTableCallback(void *NotUsed, int argc, char **argv, char **azColName);

std::string getMatrixValueString(MatrixPtr pMatrix);

/*--------- End of private methods' declarations--------- */

const char* SQLiteGateWriterImpl::MATRIX_TABLE_NAME = "matrices";
const char* SQLiteGateWriterImpl::MATRIX_LABEL_ROW_NAME = "label";
const char* SQLiteGateWriterImpl::MATRIX_VALUES_ROW_NAME = "vals";
const char* SQLiteGateWriterImpl::MATRIX_ROWS = "label, vals";

SQLiteGateWriterImpl::SQLiteGateWriterImpl(MatrixWriterPtr pMatrixWriter, std::string dbFileName) : BinaryGateWriterImpl(pMatrixWriter){
	m_dbOpennedHere = true;
	openDB(dbFileName);
	prepareInsertStatements();
}

SQLiteGateWriterImpl::SQLiteGateWriterImpl(MatrixWriterPtr pMatrixWriter, sqlite3* pDb) : BinaryGateWriterImpl(pMatrixWriter) {
	m_dbOpennedHere = false;
	m_pDb = pDb;
	createTableIfNotExist();
	prepareInsertStatements();
}

SQLiteGateWriterImpl::~SQLiteGateWriterImpl() {
	sqlite3_finalize(m_pStatement);
	if (m_dbOpennedHere) {
		closeDB();
	}
}

void SQLiteGateWriterImpl::writeMatrix(std::string label, MatrixPtr pMatrix, std::ostream& ouputStreams) {
	BinaryGateWriterImpl::writeMatrix(label, pMatrix, ouputStreams);
	try {
		writeMatrixToDB(label, pMatrix);
	}
	catch (int e) {
		fprintf(stderr, "Some problem occurred when try to write matrix to db. Error code = %d\n", e);
	}
}

void SQLiteGateWriterImpl::openDB(std::string dbFileName) {
	int rc;

	rc = sqlite3_open_v2(dbFileName.c_str(), &m_pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (rc) {
		fprintf(stderr, "Can not open DB due to error %d\n", rc);
		perror("Can not open database");
		throw (1);
	}

	createTableIfNotExist();
}

void SQLiteGateWriterImpl::createTableIfNotExist() {
	char *zErrMsg = 0;
	int rc;

	char createTableSQL[200];

	char tableFields[128];
	sprintf(tableFields, "%s TEXT UNIQUE NOT NULL, %s TEXT  NOT NULL",
			MATRIX_LABEL_ROW_NAME,
			MATRIX_VALUES_ROW_NAME);

	sprintf(createTableSQL, "CREATE TABLE IF NOT EXISTS %s (%s);",
			MATRIX_TABLE_NAME,
			tableFields);

	fprintf(stdout, "SQL command %s\n", createTableSQL);

	rc = sqlite3_exec(m_pDb, createTableSQL,
			createTableCallback,
			0,
			&zErrMsg);

	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: = %d, message = %s\n", rc, zErrMsg);
		sqlite3_free(zErrMsg);
		throw (2);
	}
	else {
		fprintf(stdout, "Table created successfully\n");
	}
}

void SQLiteGateWriterImpl::prepareInsertStatements() {
	char insertQuery[512];
	sprintf(insertQuery, "INSERT INTO %s (%s) VALUES (%s);",
			MATRIX_TABLE_NAME,
			MATRIX_ROWS,
			"?,?");

	sqlite3_prepare_v2(m_pDb, insertQuery,
			strlen(insertQuery),
			&m_pStatement, NULL);
}

void SQLiteGateWriterImpl::writeMatrixToDB(std::string label, MatrixPtr pMatrix) {
	std::string matrixValStr = getMatrixValueString(pMatrix);

	int rc = sqlite3_bind_text(m_pStatement, 1, label.c_str(), -1, SQLITE_STATIC);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "Error 1 %s due to %s\n", sqlite3_errmsg(m_pDb), sqlite3_errstr(rc));
		throw (rc);
	}
	rc = sqlite3_bind_text(m_pStatement, 2, matrixValStr.c_str(), -1, SQLITE_STATIC);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "Error 2 %s due to %s\n", sqlite3_errmsg(m_pDb), sqlite3_errstr(rc));
		throw (rc);
	}

	int result = sqlite3_step(m_pStatement);

	//Don't consider violate constrainst such as unique label a serious error
	//to forgive attempts add duplicated matrix
	if(result != SQLITE_DONE && result != SQLITE_CONSTRAINT) {
		fprintf(stderr, "Error 3 %s due to %s\n", sqlite3_errmsg(m_pDb), sqlite3_errstr(result));
		throw (result);
	}

	sqlite3_reset(m_pStatement);
}

void SQLiteGateWriterImpl::closeDB() {
	sqlite3_close(m_pDb);
}

/*----------Private methods implementation---------------*/

static int createTableCallback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

std::string getMatrixValueString(MatrixPtr pMatrix) {
	std::ostringstream ss;

	int nbRows, nbColumns;

	pMatrix->getSize(nbRows, nbColumns);

	//Write array
	ComplexValArray matrixArray;
	pMatrix->toArray(matrixArray);
	int arraySize = nbRows * nbColumns;
	for(int i = 0; i < arraySize; i++) {
		writeComplexValue(matrixArray[i], ss);
	}

	//Release array
	delete[] matrixArray;

	return ss.str();
}
