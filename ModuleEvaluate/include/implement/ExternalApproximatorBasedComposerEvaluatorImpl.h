/*
 * ExternalApproximatorBasedComposerEvaluatorImpl.h
 *
 *  Created on: May 16, 2016
 *      Author: pham
 */

#ifndef EXTERNALAPPROXIMATORBASEDCOMPOSEREVALUATORIMPL_H_
#define EXTERNALAPPROXIMATORBASEDCOMPOSEREVALUATORIMPL_H_

#include "IExternalApproximatorBasedComposerEvaluator.h"
#include "ICollection.h"

template<typename T>
class ExternalApproximatorBasedComposerEvaluatorImpl: public IExternalApproximatorBasedComposerEvaluator<T> {
public:
	ExternalApproximatorBasedComposerEvaluatorImpl(CollectionPtr<T> pCoreCollection,
			std::vector<mreal_t> partialEpsilons,
			mreal_t finalEpsilon);

	virtual ~ExternalApproximatorBasedComposerEvaluatorImpl(){};

	//Implement
	void evaluateComposer(ComposerPtr<T> pEvaluateComposer,
			ApproximatorPtr<T> pExternalApproximator,
			const std::vector<T>& partialTargets,
			T finalTarget);

private:
	void createApprxBuildingBlockBuckets(ApproximatorPtr<T> pExternalApproximator,
			const std::vector<T>& partialTargets,
			BuildingBlockBuckets<T> buildingBlockBuckets);

	CollectionPtr<T> m_pCoreCollection;
	std::vector<mreal_t> m_partialEpsilons;
	mreal_t m_finalEpsilon;
};



#endif /* EXTERNALAPPROXIMATORBASEDCOMPOSEREVALUATORIMPL_H_ */
