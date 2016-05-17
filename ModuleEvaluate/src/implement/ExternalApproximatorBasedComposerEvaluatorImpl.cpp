/*
 * ExternalApproximatorBasedComposerEvaluatorImpl.cpp
 *
 *  Created on: May 16, 2016
 *      Author: pham
 */

#include "ExternalApproximatorBasedComposerEvaluatorImpl.h"

template<typename T>
ExternalApproximatorBasedComposerEvaluatorImpl<T>::ExternalApproximatorBasedComposerEvaluatorImpl(CollectionPtr<T> pCoreCollection,
			std::vector<mreal_t> partialEpsilons,
			mreal_t finalEpsilon) {

	m_pCoreCollection = pCoreCollection;
	m_partialEpsilons = partialEpsilons;
	m_finalEpsilon = finalEpsilon;
}

template<typename T>
void ExternalApproximatorBasedComposerEvaluatorImpl<T>::evaluateComposer(ComposerPtr<T> pEvaluateComposer,
			ApproximatorPtr<T> pExternalApproximator,
			const std::vector<T>& partialTargets,
			T finalTarget) {

	//Find building block buckets corresponding to partial targets
	BuildingBlockBuckets<T> buildingBlockBuckets;
	createApprxBuildingBlockBuckets(pExternalApproximator, partialTargets, buildingBlockBuckets);

	//Compose 1-st partial candidates to have locally best approximation

	//Composer building blocks buckets for globally best approximation
}

template<typename T>
void ExternalApproximatorBasedComposerEvaluatorImpl<T>::createApprxBuildingBlockBuckets(ApproximatorPtr<T> pExternalApproximator,
			const std::vector<T>& partialTargets,
			BuildingBlockBuckets<T> buildingBlockBuckets) {

}
