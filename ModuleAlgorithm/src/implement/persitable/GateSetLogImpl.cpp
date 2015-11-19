/*
 * GateSetLog.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#include "GateSetLogImpl.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>

const std::string GateSetLogImpl::ELEMENT_DELIMETER = " ";
const std::string GateSetLogImpl::NEW_LINE = "\n";
const int GateSetLogImpl::OUT_PRECESION = 6;

GateSetLogImpl::GateSetLogImpl() {

}

void GateSetLogImpl::reset(int nbSubSet) {
	m_elementRecordSet.clear();
	for(int i = 0; i < nbSubSet; i++) {
		m_elementRecordSet.push_back(Record());
	}
	m_queryRecord.clear();
	m_queryRecord.push_back(NullPtr);
}

void GateSetLogImpl::setQuery(GatePtr pQuery) {
	m_queryRecord[0] = pQuery;
}

void GateSetLogImpl::addElementSet(const std::vector<GatePtr>& partialElementsBuffer) {
	for(unsigned int i = 0; i < partialElementsBuffer.size(); i++) {
		m_elementRecordSet[i].push_back(partialElementsBuffer[i]);
	}
}

void GateSetLogImpl::flush(std::string logFolderName) {
	prepareFolder(logFolderName);

	for(unsigned int i = 0; i < m_elementRecordSet.size(); i++) {
		std::string recordFileName = getRecordFileName(logFolderName, i);
		flushRecord(m_elementRecordSet[i], recordFileName);
	}
	std::string recordFileNameForQuery = getQueryRecordFileName(logFolderName);
	flushRecord(m_queryRecord, recordFileNameForQuery);
}

void GateSetLogImpl::load(std::string logFolderName, RecordSet& elementSet, Record& query) {
	//TODO Implement (for future purpose)
}

void GateSetLogImpl::prepareFolder(std::string logFolderName) {
	struct stat st = {0};

	if (stat(logFolderName.c_str(), &st) == -1) {
		mkdir(logFolderName.c_str(), 0766);
	}
}

std::string GateSetLogImpl::getRecordFileName(std::string logFolderName, int recordIndex) {
	std::stringstream ss;
	ss << logFolderName << "/partial_" << recordIndex << ".txt";
	return ss.str();
}

std::string GateSetLogImpl::getQueryRecordFileName(std::string logFolderName) {
	return logFolderName + "/" + "query.txt";
}

void GateSetLogImpl::flushRecord(const Record& record, std::string fileName) {
	std::ofstream fs(fileName, std::ofstream::out);
	fs << record.size() << "\n";
	bool sizeFlushed = false;
	for(unsigned int i = 0; i < record.size(); i++) {
		GatePtr pGate = record[i];
		if(pGate != NullPtr) {
			if(!sizeFlushed) {
				flushGateMatrixSize(pGate->getMatrix(), fs);
				sizeFlushed = true;
			}
			flushGateMatrixValues(pGate->getMatrix(), fs);
		}
	}
}

void GateSetLogImpl::flushGateMatrixSize(MatrixPtr pMatrix, std::ostream& fstream) {
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);
	fstream << nbRows << ELEMENT_DELIMETER << nbColumns << NEW_LINE;
}

void GateSetLogImpl::flushGateMatrixValues(MatrixPtr pMatrix, std::ostream& fstream) {
	fstream.precision(OUT_PRECESION);

	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);

	for(int row = 0; row < nbRows; row++) {
		for(int column = 0; column < nbColumns; column++) {
			ComplexVal val = pMatrix->getValue(row, column);
			double dReal = mreal::toDouble(val.real());
			double dImg = mreal::toDouble(val.imag());
			fstream << dReal << ELEMENT_DELIMETER << dImg << ELEMENT_DELIMETER;
		}
	}
	fstream << NEW_LINE;
}

