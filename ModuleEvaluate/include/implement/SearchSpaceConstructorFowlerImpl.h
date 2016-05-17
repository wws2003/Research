/*
 * SearchSpaceConstructorFowlerImpl.h
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#ifndef SEARCHSPACECONSTRUCTORFOWLERIMPL_H_
#define SEARCHSPACECONSTRUCTORFOWLERIMPL_H_

#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "ISearchSpaceConstructor.h"
#include "ICombiner.h"

template<typename T>
class SearchSpaceConstructorFowlerImpl : public ISearchSpaceConstructor<T> {
public:
	SearchSpaceConstructorFowlerImpl(CombinerPtr<T> pCombiner);

	virtual ~SearchSpaceConstructorFowlerImpl(){};

	//Override
	virtual void constructSearchSpace(CollectionPtr<T> pCollection, CollectionPtr<T> pUniversalSet, int maxSequenceLength);

protected:
	void addNewSequencesByApplyingUniversalElements(T pSequence, IteratorPtr<T> pUniversalSetIter, CombinerPtr<T> pCombiner, std::vector<T>* ppCurrentMaxLengthSequencesBuffer, CollectionPtr<T> pCollection);

	//Possibly overriden by sub classes
	virtual bool isUnique(T appendedSequence) const;

	//Possibly overriden by sub classes
	virtual void addToUniqueList(T appendedSequence);

	virtual std::vector<T>* createCurrentMaxLengthSequences();

	virtual int getBaseCollectionMaxSequenceLength();

private:
	CombinerPtr<T> m_pCombiner;
};


#endif /* SEARCHSPACECONSTRUCTORFOWLERIMPL_H_ */
