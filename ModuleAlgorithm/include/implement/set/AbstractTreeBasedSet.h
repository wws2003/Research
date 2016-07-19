/*
 * AbstractTreeBasedSet.h
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#ifndef ABSTRACTTREEBASEDSET_H_
#define ABSTRACTTREEBASEDSET_H_

#include "ISet.h"
#include "AlgoInternal.h"
#include "IRangeSearchTree.h"
#include <map>

template<typename T, typename F>
class AbstractTreeBasedSet: public ISet<T> {
public:
	AbstractTreeBasedSet(const std::vector<T>& pivots,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			mreal_t distanceToConsiderAsOne,
			RangeSearchTreePtr<T, F> pRangeSearchTree,
			const std::vector<F>& coordinateRanges);

	virtual ~AbstractTreeBasedSet();

	//Override
	bool isUnique(T element);

	//Override
	void addElement(T element);

	//Override
	void getAllUniqueElements(std::vector<T>& elements);

protected:
	//Does not require distance type must be mreal_t
	virtual void calculateDistancesToPivots(T element, std::vector<F>& distances) = 0;

	virtual std::string getElementDistanceCacheKey(T element) = 0;

	std::vector<T> m_pivots;
	DistanceCalculatorPtr<T> m_pDistanceCalculator;
	mreal_t m_distanceToConsiderAsOne;

private:
	RangeSearchTreePtr<T, F> m_pRangeSearchTree;
	std::vector<F> m_coordinateRanges;

	typedef std::map<std::string, std::vector<F> > DistanceCache;
	DistanceCache m_distanceCache;

	int m_uniqueCheckCounter;
	int m_distanceCalculateCounter;
};


#endif /* ABSTRACTTREEBASEDSET_H_ */
