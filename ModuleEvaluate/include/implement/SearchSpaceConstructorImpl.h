/*
 * SearchSpaceConstructorImpl.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef SEARCHSPACECONSTRUCTORIMPL_H_
#define SEARCHSPACECONSTRUCTORIMPL_H_

#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "ISearchSpaceConstructor.h"
#include "ICombiner.h"

template<typename T>
class SearchSpaceConstructorImpl : public ISearchSpaceConstructor<T> {
public:
	SearchSpaceConstructorImpl(CombinerPtr<T> pCombiner);
	virtual ~SearchSpaceConstructorImpl(){};

	//Override
	virtual void constructSearchSpace(CollectionPtr<T> pCollection, CollectionPtr<T> pUniversalSet, int maxSequenceLength);

private:
	CombinerPtr<T> m_pCombiner;
};


#endif /* SEARCHSPACECONSTRUCTORIMPL_H_ */
