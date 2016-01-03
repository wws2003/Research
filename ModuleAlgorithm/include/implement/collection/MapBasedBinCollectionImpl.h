/*
 * MapBasedBinCollectionImpl.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef MAPBASEDBINCOLLECTIONIMPL_H_
#define MAPBASEDBINCOLLECTIONIMPL_H_

#include "AlgoInternal.h"
#include "OperatorCommon.h"
#include "IBinCollection.h"
#include "ICoordinateCalculator.h"
#include "Coordinate.hpp"
#include <map>
#include <algorithm>
#include "GBin.hpp"

template<typename T>
using BinBinaryPatternMap = std::map<BinBinaryPattern, BinPtr<T> >;

template<typename T>
using BinBinaryPatternMapPtr = BinBinaryPatternMap<T>* ;

template<typename T>
using PatternBinPair = std::pair<BinBinaryPattern, BinPtr<T> > ;

template<typename T>
class MapBasedBinCollectionImpl : public IBinCollection<T> {
public:

	MapBasedBinCollectionImpl(RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator);

	virtual ~MapBasedBinCollectionImpl();

	//Override
	virtual void addTarget(T target);

	//Override
	virtual void addElement(T element, int targetIndex = 0);

	//Override
	virtual CollectionSize_t size();

	//Override
	virtual void clear();

	//Override
	virtual void restructureBins();

	//Override
	virtual BinIteratorPtr<T> getBinIteratorPtr();

	//Override
	virtual void findBinSetsShouldBeCombined(std::vector<BinPtrVector<T> >& binSets, bin_combination_prior_t prior_threshold) ;

protected:
	//Override
	virtual bin_combination_prior_t calculateBinCombinationPriority(BinPtrVector<T> bins);

	virtual BinBinaryPattern calculateBinPattern(T element, int targetIndex);

private:
	std::vector<T> m_targets;
	std::vector<real_coordinate_t> m_targetCoords;
	BinBinaryPatternMap<T> m_binMap;

	RealCoordinateCalculatorPtr<T> m_pRealCoordinateCalculator;

	class InnerBinIterator : IIterator<BinPtr<T> > {
	public:
		InnerBinIterator(BinBinaryPatternMapPtr<T> pBinMap);

		//Go to the beginning (point to first element)
		virtual void toBegin();

		//Go to next element pointer
		virtual void next();

		//Go to previous pointer
		virtual void prev();

		//Check if the last element has been checked
		virtual bool isDone();

		virtual BinPtr<T> getObj();

	private:
		typename BinBinaryPatternMap<T>::iterator m_currentIter;
		BinBinaryPatternMapPtr<T> m_pBinMap;
	};

	BinIteratorPtr<T> m_pInnerBinIterator;
};



#endif /* MAPBASEDBINCOLLECTIONIMPL_H_ */
