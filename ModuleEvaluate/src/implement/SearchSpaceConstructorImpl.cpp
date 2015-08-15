/*
 * SearchSpaceConstructorImpl.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#include "SearchSpaceConstructorImpl.h"
#include "ICollection.h"
#include "ICombiner.h"
#include "IIterator.h"
#include "AlgoCommon.h"
#include <vector>

template<typename T>
void addNewSequencesByApplyingUniversalElements(T pSequence, IteratorPtr<T> pUniversalSetIter, CombinerPtr<T> pCombiner, std::vector<T>* ppCurrentMaxLengthSequencesBuffer, CollectionPtr<T> pCollection);

template<typename T>
SearchSpaceConstructorImpl<T>::SearchSpaceConstructorImpl(CombinerPtr<T> pCombiner) {
	m_pCombiner = pCombiner;
}

template<typename T>
void SearchSpaceConstructorImpl<T>::constructSearchSpace(CollectionPtr<T> pCollection, CollectionPtr<T> pUniversalSet, int maxSequenceLength) {
	//Firstly add all single matrices in universal set into the persistable collection
	pCollection->clear();

	IteratorPtr<T> pUniversalSetIter = pUniversalSet->getReadonlyIteratorPtr();

	std::vector<T>* pCurrentMaxLengthSequences = NullPtr;

	//Compose matrix sequences from universal matrices up to sequence length max
	for(int i = 0; i < maxSequenceLength; i++) {
		//Use a buffer to update current max length sequences (i.e. sequences length = i)
		std::vector<T>* pCurrentMaxLengthSequencesBuffer = new std::vector<T>();

		//Calculate buffer of current max length sequences along with search space (matrix/gate collection)
		if(pCurrentMaxLengthSequences == NullPtr) {
			addNewSequencesByApplyingUniversalElements<T>(NullPtr, pUniversalSetIter, m_pCombiner, pCurrentMaxLengthSequencesBuffer, pCollection);
		}
		else {
			for(T pSequence: *(pCurrentMaxLengthSequences)) {
				addNewSequencesByApplyingUniversalElements<T>(pSequence, pUniversalSetIter, m_pCombiner, pCurrentMaxLengthSequencesBuffer, pCollection);
			}
			delete pCurrentMaxLengthSequences;
		}

		//Restore current max length sequences from buffer if maxSequenceLength not reached
		if(i + 1 < maxSequenceLength) {
			pCurrentMaxLengthSequences = pCurrentMaxLengthSequencesBuffer;
		}
	}
}

template<typename T>
void addNewSequencesByApplyingUniversalElements(T pSequence, IteratorPtr<T> pUniversalSetIter, CombinerPtr<T> pCombiner, std::vector<T>* ppCurrentMaxLengthSequencesBuffer, CollectionPtr<T> pCollection) {
	pUniversalSetIter->toBegin();

	while(!pUniversalSetIter->isDone()) {
		T pUniversalElement = pUniversalSetIter->getObj();
		T pAppendedSequence = NullPtr;

		if(pSequence == NullPtr) {
			/**
			 *Instead of adding the universal
			 *element itself, add a copy to keep the universal set
			 *independent from the collection
			 */
			pAppendedSequence = pUniversalElement->clone();
		}
		else {
			//Otherwise apply multiply operator presenting the append of universal matrix to the sequence
			pCombiner->combine(pSequence, pUniversalElement, pAppendedSequence);
		}

		if(pAppendedSequence != NullPtr) {
			//Add the newly generated sequence 's matrix/gate to the collection
			pCollection->addElement(pAppendedSequence);

			//Also add newly generated sequence's matrix/gate to buffer of current-max length sequences
			ppCurrentMaxLengthSequencesBuffer->push_back(pAppendedSequence);
		}

		//Go to next universal matrix/gate
		pUniversalSetIter->next();
	}

	//Rewind universal matrix/gate iterator to begin
	pUniversalSetIter->toBegin();
}
