/*
 * ComposerBasedSK2ApproximatorContainerImpl.h
 *
 *  Created on: Jan 6, 2016
 *      Author: pham
 */

#ifndef COMPOSERBASEDSK2APPROXIMATORCONTAINERIMPL_H_
#define COMPOSERBASEDSK2APPROXIMATORCONTAINERIMPL_H_

#include "SK2ApproximatorContainerImpl.h"

class ComposerBasedSK2ApproximatorContainerImpl : public SK2ApproximatorContainerImpl {
public:
	ComposerBasedSK2ApproximatorContainerImpl(SKApproximatorConfig config,
			CollectionConfig coreCollectionConfig,
			CoordinateAdditionalBasedComposerConfig cadbConfig,
			ComposerBasedApproximatorConfig composerBasedHelperApproximatorConfig);

	//Override
	GateApproximatorPtr getGateApproximator();

	virtual ~ComposerBasedSK2ApproximatorContainerImpl();

private:
	void initHelperApproximatorContainer(ComposerBasedApproximatorConfig composerBasedHelperApproximatorConfig);

	void releaseHelperApproximatorContainer();

	ApproximatorContainerPtr m_pHelperApproximatorContainer;
};

#endif /* COMPOSERBASEDSK2APPROXIMATORCONTAINERIMPL_H_ */
