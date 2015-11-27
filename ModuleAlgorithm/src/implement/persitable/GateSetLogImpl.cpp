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
	m_queryRecord.push_back(NullPtr);
}

void GateSetLogImpl::saveQuery(GatePtr pQuery) {
	m_queryRecord[0] = pQuery;
}

void GateSetLogImpl::saveElementSets(std::vector<IteratorPtr<GatePtr> > elementSets) {
	m_elementRecordSet.clear();
	for(unsigned int i = 0; i < elementSets.size(); i++) {
		Record record;
		IteratorPtr<GatePtr> pRecordIter = elementSets[i];

		if(pRecordIter != NullPtr) {
			while(!pRecordIter->isDone()) {
				record.push_back(pRecordIter->getObj());
				pRecordIter->next();
			}
			pRecordIter->toBegin();//Re-wind iterator for future purpose
		}

		m_elementRecordSet.push_back(record);
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
			//flushGateLabel(pGate, fs);
			flushGateMatrixValues(pGate->getMatrix(), fs);
		}
	}
}

void GateSetLogImpl::flushGateMatrixSize(MatrixPtr pMatrix, std::ostream& fstream) {
	int nbRows, nbColumns;
	pMatrix->getSize(nbRows, nbColumns);
	fstream << nbRows << ELEMENT_DELIMETER << nbColumns << NEW_LINE;
}

void GateSetLogImpl::flushGateLabel(GatePtr pGate, std::ostream& fstream) {
	std::string delimeter = "*";
	fstream << "Gate length:" << pGate->getLabelSeq().size() << ELEMENT_DELIMETER;
	for(LabelSeq::const_iterator lIter = pGate->getLabelSeq().begin(); lIter != pGate->getLabelSeq().end(); lIter++) {
		if(lIter == pGate->getLabelSeq().begin()) {
			fstream << *lIter  ;
		}
		else {
			fstream << delimeter << *lIter;
		}
	}
	fstream << ELEMENT_DELIMETER;
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

