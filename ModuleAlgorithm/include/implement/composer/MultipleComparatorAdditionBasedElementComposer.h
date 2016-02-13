/*
 * MultipleComparatorAdditionBasedElementComposer.h
 *
 *  Created on: Feb 12, 2016
 *      Author: pham
 */

#ifndef MULTIPLECOMPARATORADDITIONBASEDELEMENTCOMPOSER_H_
#define MULTIPLECOMPARATORADDITIONBASEDELEMENTCOMPOSER_H_

#include "AdditionBasedElementComposer.h"

template<typename T>
class MultipleComparatorAdditionBasedElementComposer: public AdditionBasedElementComposer<T> {
public:
	MultipleComparatorAdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
			CombinerPtr<T> pCombiner,
			T epsilonElement,
			int maxResultsNumber,
			const std::vector<ComparatorPtr<T> >& secondaryComparators,
			const std::vector<T> secondaryEpsilonElements);


	virtual ~MultipleComparatorAdditionBasedElementComposer(){};

protected:
	//Override
	void initSecondarySortedVectorArrays(typename AdditionBasedElementComposer<T>::SortedVectorArrayList& secondarySortedVectorArrays,
			const BuildingBlockBuckets<T>& buildingBlockBuckets);

	//Override
	bool quickEvaluate(const typename AdditionBasedElementComposer<T>::SortedVectorArrayList& secondarySortedVectorArrays,
				int vectorIndex,
				const T& rightMostElement,
				const T& partialTarget) const;

	//Override
	void releaseSecondarySortedVectorArrays(typename AdditionBasedElementComposer<T>::SortedVectorArrayList& sortedVectorArrays);

private:
	 std::vector<ComparatorPtr<T> > m_secondaryComparators;
	 std::vector<T> m_secondaryEpsilonElements;
};


#endif /* MULTIPLECOMPARATORADDITIONBASEDELEMENTCOMPOSER_H_ */
