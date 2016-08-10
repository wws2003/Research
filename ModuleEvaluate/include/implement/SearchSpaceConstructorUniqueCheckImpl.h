/*
 * SearchSpaceConstructorUniqueCheckImpl.h
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
class SearchSpaceConstructorUniqueCheckImpl : public ISearchSpaceConstructor<T> {
public:
	SearchSpaceConstructorUniqueCheckImpl(CollectionPtr<T> pBaseCollection,
			int baseLength,
			CombinerPtr<T> pCombiner);

	SearchSpaceConstructorUniqueCheckImpl(CombinerPtr<T> pCombiner);

	virtual ~SearchSpaceConstructorUniqueCheckImpl(){};

	//Override
	virtual void constructSearchSpace(CollectionPtr<T> pCollection, CollectionPtr<T> pUniversalSet, int maxSequenceLength);

protected:
	void addNewSequencesByApplyingUniversalElements(T pSequence,
			IteratorPtr<T> pUniversalSetIter,
			CombinerPtr<T> pCombiner,
			std::vector<T>* ppCurrentMaxLengthSequencesBuffer,
			bool isMaxLengReached,
			CollectionPtr<T> pCollection);

	//Possibly overriden by sub classes
	virtual bool isUnique(T appendedSequence) const = 0;

	//Possibly overriden by sub classes
	virtual void addToUniqueList(T appendedSequence, bool isMaxLengReached) = 0;

	virtual bool isMaxLengthBaseElement(T baseElement) const = 0;

	int m_baseLength;

private:
	void processBaseCollection(CollectionPtr<T> pTargetCollection, std::vector<T>*& pCurrentMaxLengthSequences);

	CollectionPtr<T> m_pBaseCollection;

	CombinerPtr<T> m_pCombiner;
	int m_counter;
};


#endif /* SEARCHSPACECONSTRUCTORFOWLERIMPL_H_ */
