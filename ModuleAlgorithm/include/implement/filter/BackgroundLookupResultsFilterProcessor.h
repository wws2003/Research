/*
 * BackgroundLookupResultsFilterProcessor.h
 *
 *  Created on: Dec 15, 2015
 *      Author: pham
 */

#ifndef BACKGROUNDLOOKUPRESULTSFILTERPROCESSOR_H_
#define BACKGROUNDLOOKUPRESULTSFILTERPROCESSOR_H_

#include "BackgroundLookupResultProcessor.h"
#include "IDistanceCalculator.h"
#include <map>
#include <set>

template<typename T>
class BackgroundLookupResultsFilterProcessor: public BackgroundLookupResultProcessor<T> {
public:
	BackgroundLookupResultsFilterProcessor(DistanceCalculatorPtr<T> pDistanceCalculator);

protected:

	//Override
	void processOnBackground(const LookupResult<T>& result);

	//Override
	void exportInternalContainerToResults(std::vector<LookupResult<T> >& results, bool toSortResults = true) const;

	//Override
	void resetInternalContainer();

private:
	typedef std::multiset<LookupResult<T>, DistanceComparator<T> > rmap;

	typename rmap::iterator checkDuplicateResult(const LookupResult<T>& result, bool& isDuplicate) const;

	rmap m_resultMap;

	DistanceCalculatorPtr<T> m_pDistanceCalculator;
};


#endif /* BACKGROUNDLOOKUPRESULTSFILTERPROCESSOR_H_ */
