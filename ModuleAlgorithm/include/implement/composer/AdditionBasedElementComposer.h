/*
 * AdditionBasedElementComposer.h
 *
 *  Created on: Dec 6, 2015
 *      Author: pham
 */

#ifndef ADDITIONBASEDELEMENTCOMPOSER_H_
#define ADDITIONBASEDELEMENTCOMPOSER_H_

#include "IComposer.h"
#include "AlgoInternal.h"
#include "AlgoCommon.h"
#include "ICombiner.h"
#include "IElementComparator.h"

template<typename T>
class AdditionBasedElementComposer: public IComposer<T> {
public:
	AdditionBasedElementComposer(ComparatorPtr<T> pElementComparator,
			CombinerPtr<T> pCombiner,
			T epsilonElement,
			int maxResultsNumber);

	virtual ~AdditionBasedElementComposer(){};

	//Override
	IteratorPtr<T> composeApproximations(const BuildingBlockBuckets<T>& buildingBlockBuckets,
			T target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon);

private:

	struct WrapperComparator {
		WrapperComparator(ComparatorPtr<T> pElementComparator) {
			m_pElementComparator = pElementComparator;
		}
		bool operator()(const T& lhs, const T& rhs)
		{
			return m_pElementComparator->lessThan(lhs, rhs);
		}
		ComparatorPtr<T> m_pElementComparator;
	};

	class SortedVectorArray {
	public:
		SortedVectorArray(WrapperComparator& comparator) : m_wrapperComparator(comparator){};

		//Init vectors fields
		void initByVectors(const BuildingBlockBuckets<T>& buildingBlockBuckets);

		/**
		 * Return maximum element can be retrieved by accumulating elements
		 *  in vectors (one element each vector) upto lastVectorIndex
		 *
		 */
		const T& getMaxAccumulated(int lastVectorIndex) const;

		/**
		 * Return minimum element can be retrieved by accumulating elements
		 *  in vectors (one element each vector) upto lastVectorIndex
		 *
		 */
		const T& getMinAccumulated(int lastVectorIndex) const;

		/**
		 * Return lower and upper bound when looking up target in [min, max] on vector indexed by vector index
		 */
		void getRangeInVector(int vectorIndex, T min, T max, int& lowerBound, int& upperBound) const;

		//Retrieve immutable vector by vector index
		const std::vector<T>& getVector(int vectorIndex) const;

		int getNbVectors() const;

	private:
		WrapperComparator& m_wrapperComparator;

		std::vector<std::vector<T> > m_vectors;
		std::vector<T> m_minAccumulated;
		std::vector<T> m_maxAccumulated;

	};

	void findCompositionsInRange(const SortedVectorArray& sortedVectorArray,
			int vectorIndex,
			std::vector<T>& partialTermElements,
			const T& partialTarget,
			const T& target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			std::vector<T>& resultBuffer);

	void evaluateCombination(const std::vector<T>& partialElements,
			T target,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t epsilon,
			std::vector<T>& resultBuffer);

	void composeCandidate(const std::vector<T>& partialElements, T& result);

	WrapperComparator m_wrapperComparator;

	CombinerPtr<T> m_pCombiner;
	int m_maxResultsNumber;

	T m_epsilonElement;
};



#endif /* ADDITIONBASEDELEMENTCOMPOSER_H_ */
