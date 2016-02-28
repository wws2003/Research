/*
 * EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl.h
 *
 *  Created on: Feb 25, 2016
 *      Author: pham
 */

#ifndef EVALUATINGPARALLELCOORDINATEADDTIONBASEDCOMPOSERCONTAINERIMPL_H_
#define EVALUATINGPARALLELCOORDINATEADDTIONBASEDCOMPOSERCONTAINERIMPL_H_

#include "IEvaluatingComposerContainer.h"
#include "IComposerContainer.h"
#include "ApplicationCommon.h"
#include "Config.h"

class EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl: public IEvaluatingComposerContainer {
public:
	EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig,
			const ParallelConfig& parallelConfig);

	virtual ~EvaluatingParallelCoordinateAddtionBasedComposerContainerImpl();

	//Override
	GateComposerPtr getStandardGateComposer();

	//Override
	GateComposerPtr getEvaluatedGateComposer();

private:
	void wireDependencies(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig,
			const ParallelConfig& parallelConfig);

	void releaseDependencies();

	ComposerContainerPtr m_pStandardComposerContainer;
	ComposerContainerPtr m_pEvaluatedComposerContainer;
};

#endif /* EVALUATINGPARALLELCOORDINATEADDTIONBASEDCOMPOSERCONTAINERIMPL_H_ */
