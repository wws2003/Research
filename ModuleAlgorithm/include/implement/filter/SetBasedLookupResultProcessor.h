/*
 * DummyLookupResultProcessor.h
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#ifndef SETBASEDLOOKUPRESULTPROCESSOR_H_
#define SETBASEDLOOKUPRESULTPROCESSOR_H_

#include "ILookupResultProcessor.h"
#include <set>

template<typename T>
class SetBasedLookupResultProcessor: public ILookupResultProcessor<T> {
public:

	SetBasedLookupResultProcessor(DistanceCalculatorPtr<T> pDistanceCalculator, bool isUniqueElements);

	virtual ~SetBasedLookupResultProcessor(){};

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

	virtual bool sameElement(const T& t1, const T& t2) const = 0;

private:
	bool m_isUniqueElements;
	rmap m_resultMap;
	DistanceCalculatorPtr<T> m_pDistanceCalculator;
};



#endif /* DUMMYLOOKUPRESULTPROCESSOR_H_ */
