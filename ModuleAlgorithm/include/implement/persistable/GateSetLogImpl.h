/*
 * GateSetLogImpl.h
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#ifndef GATESETLOGIMPL_H_
#define GATESETLOGIMPL_H_

#include "IElementSetLog.h"
#include "Gate.h"
#include <vector>
#include <fstream>

class GateSetLogImpl: public IElementSetLog<GatePtr> {
public:
	GateSetLogImpl();

	virtual ~GateSetLogImpl(){};

	void saveQuery(GatePtr query);

	void saveElementSets(std::vector<IteratorPtr<GatePtr> > elementSets);

	void flush(std::string logFolderName);

	void load(std::string logFolderName, RecordSet& elementSet, Record& query);

private:
	void prepareFolder(std::string logFolderName);

	std::string getQueryRecordFileName(std::string logFolderName);

	std::string getRecordFileName(std::string logFolderName, int recordIndex);

	void flushRecord(const Record& record, std::string fileName);

	void flushGateMatrixSize(MatrixPtr pMatrix, std::ostream& fstream);

	void flushGateLabel(GatePtr pGate, std::ostream& fstream);

	void flushGateMatrixValues(MatrixPtr pMatrix, std::ostream& fstream);

	const static std::string ELEMENT_DELIMETER;
	const static std::string NEW_LINE;
	const static int OUT_PRECESION;
	RecordSet m_elementRecordSet;
	Record m_queryRecord;
};



#endif /* GATESETLOGIMPL_H_ */
