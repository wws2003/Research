/*
 * ParallelMultipleComparatorCoordinateAdditionBasedGateComposer.h
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#ifndef PARALLELMULTIPLECOMPARATORCOORDINATEADDITIONBASEDGATECOMPOSER_H_
#define PARALLELMULTIPLECOMPARATORCOORDINATEADDITIONBASEDGATECOMPOSER_H_

#include "ParallelMultipleComparatorAdditionBasedElementComposer.h"
#include "Coordinate.hpp"
#include "Gate.h"
#include "AlgoInternal.h"
#include "ParallelCoordinateAdditionBasedGateComposer.h"

class ParallelMultipleComparatorCoordinateAdditionBasedGateComposer: public ParallelMultipleComparatorAdditionBasedElementComposer<GateRealCoordinate> {
public:
	ParallelMultipleComparatorCoordinateAdditionBasedGateComposer(ComparatorPtr<GateRealCoordinate> pElementComparator,
			CombinerPtr<GateRealCoordinate> pCombiner,
			GateRealCoordinate epsilonElement,
			int maxResultsNumber,
			const std::vector<ComparatorPtr<GateRealCoordinate> >& secondaryComparators,
			const std::vector<GateRealCoordinate> secondaryEpsilonElements,
			TaskExecutorPtr<LookupResult<GateRealCoordinate> > pTaskExecutor);

	virtual ~ParallelMultipleComparatorCoordinateAdditionBasedGateComposer(){};

	//Override
	TaskPtr<LookupResult<GateRealCoordinate> > generateCombiningTask(const std::vector<GateRealCoordinate>& partialElements);
};

#endif /* PARALLELMULTIPLECOMPARATORCOORDINATEADDITIONBASEDGATECOMPOSER_H_ */
