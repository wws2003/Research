/*
 * IOTestSuite.h
 *
 *  Created on: Aug 10, 2015
 *      Author: pham
 */

#ifndef IOTESTSUITE_H_
#define IOTESTSUITE_H_

#include "IWriter.h"
#include "IReader.h"
#include "IOCommon.h"
#include "IMatrixFactory.h"
#include <fstream>
#include <string>

class IOTestSuite {
public:
	IOTestSuite();
	void test();

	virtual ~IOTestSuite();

private:
	template<typename TPtr>
	void testReadWriteElement(WriterPtr<TPtr> pWriter, ReaderPtr<TPtr> pReader, std::ofstream& outputStream, std::ifstream& inputStream);

	void assertFilesOpen();

	void testMatrixReadWrite();
	void testGateReadWrite();
	void testGNATCollectionPersistence();

	void clearFiles();

	MatrixFactoryPtr m_pMatrixFactory;

	MatrixWriterPtr m_pMatrixWriter;
	MatrixReaderPtr m_pMatrixReader;
	GateWriterPtr m_pGateWriter;
	GateReaderPtr m_pGateReader;

	std::ofstream m_matrixOutputStream;
	std::ifstream m_matrixInputStream;

	std::ofstream m_gateOutputStream;
	std::ifstream m_gateInputStream;

	const static std::string MATRIX_FILE_NAME;
	const static std::string GATE_FILE_NAME;
	const static std::string GNAT_COLLECTION_FILE;
};

#endif /* IOTESTSUITE_H_ */
