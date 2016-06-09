/*
 * SearchSpaceConstructorBufferBasedImpl.h
 *
 *  Created on: Jun 2, 2016
 *      Author: pham
 */

#ifndef SEARCHSPACECONSTRUCTORBUFFERBASEDIMPL_H_
#define SEARCHSPACECONSTRUCTORBUFFERBASEDIMPL_H_

#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "ISearchSpaceConstructor.h"
#include "ICombiner.h"

template<typename T>
class SearchSpaceConstructorBufferBasedImpl : public ISearchSpaceConstructor<T> {
public:
	SearchSpaceConstructorBufferBasedImpl(CombinerPtr<T> pCombiner);

	virtual ~SearchSpaceConstructorBufferBasedImpl(){};

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

	class SequenceBuffer {
	public:
		virtual ~SequenceBuffer(){};

		void addToBuffer(T pSequence);
	};

private:
	CombinerPtr<T> m_pCombiner;
	int m_counter;
};


#endif /* SEARCHSPACECONSTRUCTORBUFFERBASEDIMPL_H_ */
