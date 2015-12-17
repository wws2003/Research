/*
 * AdaptiveElementComposer.h
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#ifndef ADAPTIVEELEMENTCOMPOSER_H_
#define ADAPTIVEELEMENTCOMPOSER_H_

#include "IComposer.h"
#include "AlgoInternal.h"
#include "AlgoCommon.h"
#include "IConverter.h"

template<typename T1, typename T2>
class AdaptiveElementComposer: public IComposer<T1> {
public:

	AdaptiveElementComposer(DistanceCalculatorPtr<T2> pDerivedDistanceCalculator,
			ComposerPtr<T2> pDerivedComposer,
			mreal_t derivedComposerEpsilon,
			ConverterPtr<T1, T2> pConverter);

	virtual ~AdaptiveElementComposer(){};

	//Override
	IteratorPtr<T1> composeApproximations(const BuildingBlockBuckets<T1>& buildingBlockBuckets,
			T1 target,
			DistanceCalculatorPtr<T1> pDistanceCalculator,
			mreal_t epsilon);

private:
	void convertToDerivedBuildingBlockBuckets(const BuildingBlockBuckets<T1>& buildingBlockBuckets, BuildingBlockBuckets<T2>& derivedBuildingBlockBuckets);

	void releaseDerivedBuildingBlockBuckets(BuildingBlockBuckets<T2>& derivedBuildingBlockBuckets);

	void releaseDerivedIter(IteratorPtr<T2>& pDerivedIter);

	void convertToOriginalIter(const IteratorPtr<T2>& pDerivedIter,
			IteratorPtr<T1>& pResultIter,
			T1 target,
			DistanceCalculatorPtr<T1> pDistanceCalculator,
			mreal_t epsilon);

	void convertToDerivedIter(const IteratorPtr<T1>& pOriginalIter,
				IteratorPtr<T2>& pDerivedIter);

	DistanceCalculatorPtr<T2> m_pDerivedDistanceCalculator;
	ComposerPtr<T2> m_pDerivedComposer;
	mreal_t m_derivedComposerEpsilon;

	ConverterPtr<T1, T2> m_pConverter;
};


#endif /* ADAPTIVEELEMENTCOMPOSER_H_ */