/*
 * SimpleElementComposer.h
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#ifndef SIMPLEELEMENTCOMPOSER_H_
#define SIMPLEELEMENTCOMPOSER_H_

#include "IComposer.h"
#include "ICombiner.h"
#include "ICollection.h"
#include "IElementSetLog.h"

template<typename T>
class SimpleElementComposer: public IComposer<T> {
public:
	SimpleElementComposer(CombinerPtr<T> pCombiner,
			int maxResultsNumber,
			ElementSetLogPtr<T> pElementSetLog);

	virtual ~SimpleElementComposer(){};

	//Override
	virtual IteratorPtr<LookupResult<T> > composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			bool toSortResults);

private:
	void generateApproximations(std::vector<T>& partialElementsBuffer,
			int bucketIndex,
			const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			std::vector<LookupResult<T> >& resultBuffer);

	std::string getCurrentLogFolderName();

	void evaluateCombination(const std::vector<T>& partialElements,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			std::vector<LookupResult<T> >& resultBuffer);

	void composeCandidate(const std::vector<T>& partialElements, T& result);

	static const std::string LOG_ROOT_FOLDER;

	ElementSetLogPtr<T> m_pElementSetLog;
	CombinerPtr<T> m_pCombiner;
	int m_maxResultsNumber;

	combination_counter_t m_combinationCounter;

	int m_logFolderCounter;
};

#endif /* SIMPLEELEMENTCOMPOSER_H_ */
