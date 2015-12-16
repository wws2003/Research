/*
 * DummyLookupResultProcessor.h
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#ifndef DUMMYLOOKUPRESULTPROCESSOR_H_
#define DUMMYLOOKUPRESULTPROCESSOR_H_

#include "ILookupResultProcessor.h"
#include <set>

template<typename T>
class DummyLookupResultProcessor: public ILookupResultProcessor<T> {
public:
	enum storage_t {
		STRG_SET,
		STRG_VECTOR
	};

	DummyLookupResultProcessor(DistanceCalculatorPtr<T> pDistanceCalculator, storage_t storageType = STRG_VECTOR);

	virtual ~DummyLookupResultProcessor(){};

	//Override
	void init();

	//Override
	void reset();

	//Override
	void addLookupResultsBatch(const std::vector<LookupResult<T> >& result);

	//Override
	void addLookupResult(const LookupResult<T>& result);

	//Override
	void retrieveProcessedLookupResults(std::vector<LookupResult<T> >& results, bool toSortResults = true);

protected:
	typedef std::multiset<LookupResult<T>, DistanceComparator<T> > rmap;

	typename rmap::iterator checkDuplicateResult(const LookupResult<T>& result, bool& isDuplicate) const;

	virtual void filterLookupResults(std::vector<LookupResult<T> >& lookupResults,
			DistanceCalculatorPtr<T> pDistanceCalculator);

	virtual bool isDuplicateFound(const std::vector<LookupResult<T> >& lookupResults,
			int nonDuplicateLastPivot,
			LookupResult<T> result,
			DistanceCalculatorPtr<T> pDistanceCalculator);

	virtual bool sameElement(const T& t1, const T& t2) const = 0;

private:
	storage_t m_storageType;

	std::vector<LookupResult<T> > m_storage;

	rmap m_resultMap;

	DistanceCalculatorPtr<T> m_pDistanceCalculator;
};



#endif /* DUMMYLOOKUPRESULTPROCESSOR_H_ */
