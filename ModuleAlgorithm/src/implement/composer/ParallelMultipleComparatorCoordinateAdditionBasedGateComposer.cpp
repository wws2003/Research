/*
 * ParallelMultipleComparatorCoordinateAdditionBasedGateComposer.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#include "ParallelMultipleComparatorCoordinateAdditionBasedGateComposer.h"
#include "GateCoordinateCombiningTask.h"
#include "GatesCoordinateCombinationVerifyTask.h"
#include "ParallelMultipleComparatorAdditionBasedElementComposer.cpp"

template class ParallelMultipleComparatorAdditionBasedElementComposer<GateRealCoordinate>;

ParallelMultipleComparatorCoordinateAdditionBasedGateComposer::ParallelMultipleComparatorCoordinateAdditionBasedGateComposer(ComparatorPtr<GateRealCoordinate> pElementComparator,
		CombinerPtr<GateRealCoordinate> pCombiner,
		GateRealCoordinate epsilonElement,
		int taskFutureBufferSize,
		const std::vector<ComparatorPtr<GateRealCoordinate> >& secondaryComparators,
		const std::vector<GateRealCoordinate> secondaryEpsilonElements,
		TaskExecutorPtr<LookupResult<GateRealCoordinate> > pTaskExecutor) : ParallelMultipleComparatorAdditionBasedElementComposer<GateRealCoordinate> (pElementComparator,
				pCombiner,
				epsilonElement,
				taskFutureBufferSize,
				secondaryComparators,
				secondaryEpsilonElements,
				pTaskExecutor) {

}

TaskPtr<LookupResult<GateRealCoordinate> > ParallelMultipleComparatorCoordinateAdditionBasedGateComposer::generateCombiningTask(const std::vector<GateRealCoordinate>& partialElements,
		GateRealCoordinate target,
		DistanceCalculatorPtr<GateRealCoordinate> pDistanceCalculator,
		mreal_t epsilon) {

	return TaskPtr<LookupResult<GateRealCoordinate> >(new GatesCoordinateCombinationVerifyTask(partialElements,
			m_pCombiner,
			pDistanceCalculator,
			target,
			epsilon,
			false));

	//return TaskPtr<LookupResult<GateRealCoordinate> >(new GateCoordinateCombiningTask(partialElements, m_pCombiner, false));
}
