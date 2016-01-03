/*
 * DummyLookupResultProcessor.h
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#ifndef VECTORBASEDLOOKUPRESULTPROCESSOR_H_
#define VECTORBASEDLOOKUPRESULTPROCESSOR_H_

#include "ILookupResultProcessor.h"
#include <set>

template<typename T>
class VectorBasedLookupResultProcessor: public ILookupResultProcessor<T> {
public:

	VectorBasedLookupResultProcessor(DistanceCalculatorPtr<T> pDistanceCalculator);

	virtual ~VectorBasedLookupResultProcessor(){};

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

	virtual void filterLookupResults(std::vector<LookupResult<T> >& lookupResults,
			DistanceCalculatorPtr<T> pDistanceCalculator);

	virtual bool isDuplicateFound(const std::vector<LookupResult<T> >& lookupResults,
			int nonDuplicateLastPivot,
			LookupResult<T> result,
			DistanceCalculatorPtr<T> pDistanceCalculator);

	virtual bool sameElement(const T& t1, const T& t2) const = 0;

private:

	std::vector<LookupResult<T> > m_storage;

	DistanceCalculatorPtr<T> m_pDistanceCalculator;
};



#endif /* DUMMYLOOKUPRESULTPROCESSOR_H_ */
