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
	virtual IteratorPtr<T> composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon);

private:
	void generateApproximations(std::vector<T>& partialElementsBuffer,
			int bucketIndex,
			const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			CollectionPtr<T> pResultsBuffer);

	std::string getCurrentLogFolderName();

	void evaluateCombination(const std::vector<T>& partialElements,
			T pQuery,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			CollectionPtr<T> pResultsBuffer);

	void composeCandidate(const std::vector<T>& partialElements, T& result);

	void output(const std::vector<T>& partialElements, T pQuery);

	ElementSetLogPtr<T> m_pElementSetLog;
	CombinerPtr<T> m_pCombiner;
	int m_maxResultsNumber;

	int m_logFolderCounter;
};

#endif /* SIMPLEELEMENTCOMPOSER_H_ */
