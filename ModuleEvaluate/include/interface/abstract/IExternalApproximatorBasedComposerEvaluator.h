/*
 * IExternalApproximatorBasedComposerEvaluator.h
 *
 *  Created on: May 16, 2016
 *      Author: pham
 */

#ifndef IEXTERNALAPPROXIMATORBASEDCOMPOSEREVALUATOR_H_
#define IEXTERNALAPPROXIMATORBASEDCOMPOSEREVALUATOR_H_

#include "IComposer.h"
#include "IApproximator.h"
#include "AlgoCommon.h"

template<typename T>
class IExternalApproximatorBasedComposerEvaluator {
public:
	virtual ~IExternalApproximatorBasedComposerEvaluator(){};

	virtual void evaluateComposer(ComposerPtr<T> pEvaluateComposer,
			ApproximatorPtr<T> pExternalApproximator,
			const std::vector<T>& partialTargets,
			T finalTarget) = 0;
};

#endif /* IEXTERNALAPPROXIMATORBASEDCOMPOSEREVALUATOR_H_ */
