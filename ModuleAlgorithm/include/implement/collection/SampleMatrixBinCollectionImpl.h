/*
 * SampleMatrixBinCollectionImpl.h
 *
 *  Created on: Jun 15, 2015
 *      Author: pham
 */

#ifndef SAMPLEMATRIXBINCOLLECTIONIMPL_H_
#define SAMPLEMATRIXBINCOLLECTIONIMPL_H_

#include "IMatrixBinCollection.h"
#include <map>
#include <algorithm>

typedef std::map<BinPattern, MatrixBinPtr> BinPatternMap;
typedef BinPatternMap* BinPatternMapPtr;
typedef std::pair<BinPattern, MatrixBinPtr> PatternBinPair;

class SampleMatrixBinCollectionImpl : IMatrixBinCollection {
public:
	SampleMatrixBinCollectionImpl();

	virtual ~SampleMatrixBinCollectionImpl();

	virtual void addMatrix(MatrixPtr pMatrix, BinPattern binPattern);

	virtual void clear();

	virtual MatrixBinIteratorPtr getMatrixBinIteratorPtr();

	virtual MatrixBinIteratorPtr findBinsCloseToBin(MatrixBinPtr pMatrixBin, int distance);

private:
	BinPatternMap m_binMap;

	class InnerMatrixBinIterator : IIterator<MatrixBinPtr> {
	public:
		InnerMatrixBinIterator(BinPatternMapPtr pBinMap);

		//Go to the beginning (point to first element)
		virtual void toBegin();

		//Go to next element pointer
		virtual void next();

		//Go to previous pointer
		virtual void prev();

		//Check if the last element has been checked
		virtual bool isDone();

		virtual MatrixBinPtr getObj();

	private:
		BinPatternMap::iterator m_currentIter;
		BinPatternMapPtr m_pBinMap;
	};

	MatrixBinIteratorPtr m_pInnerMatrixBinIterator;

};


#endif /* SAMPLEMATRIXBINCOLLECTIONIMPL_H_ */
