/*
 * IElementSetLog.h
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#ifndef IELEMENTSETLOG_H_
#define IELEMENTSETLOG_H_

#include "AlgoInternal.h"

template<typename T>
class IElementSetLog {
public:
	virtual ~IElementSetLog(){};

	virtual void reset(int nbSubSet) = 0;

	virtual void setQuery(T query) = 0;

	virtual void addElementSet(const std::vector<T>& partialElementsBuffer) = 0;

	virtual void flush(std::string logFolderName) = 0;

	typedef std::vector<T> Record;
	typedef std::vector<Record> RecordSet;

	//For future purpose
	virtual void load(std::string logFolderName, RecordSet& elementSet, Record& query) = 0;
};

#endif /* IELEMENTSETLOG_H_ */
