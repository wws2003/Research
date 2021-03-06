/*
 * SampleComposerContainerImpl.h
 *
 *  Created on: Feb 4, 2016
 *      Author: pham
 */

#ifndef SAMPLECOMPOSERCONTAINERIMPL_H_
#define SAMPLECOMPOSERCONTAINERIMPL_H_

#include "Config.h"
#include "IEvaluatingComposerContainer.h"
#include "IComposerContainer.h"
#include "ApplicationCommon.h"
#include "IApproximatorContainer.h"

class SampleEvaluatingComposerContainerImpl: public IEvaluatingComposerContainer {
public:
	SampleEvaluatingComposerContainerImpl(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig);

	virtual ~SampleEvaluatingComposerContainerImpl();

	//Override
	GateComposerPtr getStandardGateComposer();

	//Override
	virtual GateComposerPtr getEvaluatedGateComposer();

protected:
	void wireDependencies(const CoordinateAdditionalBasedComposerConfig& cabConfig,
			const CollectionConfig& collectionConfig);

	void releaseDependencies();

	ComposerContainerPtr m_pStandardComposerContainer;
	ComposerContainerPtr m_pEvaluatedComposerContainer;
};

#endif /* SAMPLECOMPOSERCONTAINERIMPL_H_ */
