/*
 * IComposerEvaluator.h
 *
 *  Created on: Feb 3, 2016
 *      Author: pham
 */

#ifndef ICOMPOSEREVALUATOR_H_
#define ICOMPOSEREVALUATOR_H_

#include "IComposer.h"
#include "AlgoCommon.h"

template<typename T>
class IComposerEvaluator {
public:
	virtual ~IComposerEvaluator(){};

	virtual void evaluateComposers(ComposerPtr<T> pEvaluateComposer,
			ComposerPtr<T> pStandardComposer) = 0;
};


#endif /* IGATEDECOMPOSEREVALUATOR_H_ */
