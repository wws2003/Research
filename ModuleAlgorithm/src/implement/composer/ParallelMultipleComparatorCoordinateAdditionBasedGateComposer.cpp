/*
 * ParallelMultipleComparatorCoordinateAdditionBasedGateComposer.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#include "ParallelMultipleComparatorCoordinateAdditionBasedGateComposer.h"
#include "GateCoordinateCombiningTask.h"
#include "ParallelMultipleComparatorAdditionBasedElementComposer.cpp"

template class ParallelMultipleComparatorAdditionBasedElementComposer<GateRealCoordinate>;

ParallelMultipleComparatorCoordinateAdditionBasedGateComposer::ParallelMultipleComparatorCoordinateAdditionBasedGateComposer(ComparatorPtr<GateRealCoordinate> pElementComparator,
		CombinerPtr<GateRealCoordinate> pCombiner,
		GateRealCoordinate epsilonElement,
		int maxResultsNumber,
		const std::vector<ComparatorPtr<GateRealCoordinate> >& secondaryComparators,
		const std::vector<GateRealCoordinate> secondaryEpsilonElements,
		TaskExecutorPtr<LookupResult<GateRealCoordinate> > pTaskExecutor) : ParallelMultipleComparatorAdditionBasedElementComposer<GateRealCoordinate> (pElementComparator,
				pCombiner,
				epsilonElement,
				maxResultsNumber,
				secondaryComparators,
				secondaryEpsilonElements,
				pTaskExecutor) {

}

TaskPtr<LookupResult<GateRealCoordinate> > ParallelMultipleComparatorCoordinateAdditionBasedGateComposer::generateCombiningTask(const std::vector<GateRealCoordinate>& partialElements) {
	return TaskPtr<LookupResult<GateRealCoordinate> >(new GateCoordinateCombiningTask(partialElements, m_pCombiner, false));
}
