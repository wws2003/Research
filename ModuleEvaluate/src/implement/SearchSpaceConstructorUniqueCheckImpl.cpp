/*
 * SearchSpaceConstructorUniqueCheckImpl.cpp
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#include "SearchSpaceConstructorUniqueCheckImpl.h"
#include "ICollection.h"
#include "ICombiner.h"
#include "IIterator.h"
#include "AlgoCommon.h"
#include <vector>

#define PROGRESS_COUNTER (600)

template<typename T>
SearchSpaceConstructorUniqueCheckImpl<T>::SearchSpaceConstructorUniqueCheckImpl(CombinerPtr<T> pCombiner) {
	m_pCombiner = pCombiner;
	m_counter = 0;
}

template<typename T>
void SearchSpaceConstructorUniqueCheckImpl<T>::constructSearchSpace(CollectionPtr<T> pCollection, CollectionPtr<T> pUniversalSet, int maxSequenceLength) {
	//Firstly add all single matrices in universal set into the persistable collection
	pCollection->clear();

	IteratorPtr<T> pUniversalSetIter = pUniversalSet->getReadonlyIteratorPtr();

	std::vector<T>* pCurrentMaxLengthSequences = createCurrentMaxLengthSequences();
	//If there are sequences in pCurrentMaxLengthSequences, add to constructing collection and unique list
	if(pCurrentMaxLengthSequences != NullPtr) {
		for(T pSequence : *pCurrentMaxLengthSequences) {
			pCollection->addElement(pSequence);
			addToUniqueList(pSequence);
		}
		std::cout << "Finished adding base collection with " << pCurrentMaxLengthSequences->size() << " items \n";
	}

	//Compose matrix sequences from universal matrices up to sequence length max
	int startCounter = getBaseCollectionMaxSequenceLength();
	for(int i = startCounter; i < maxSequenceLength; i++) {
		//Use a buffer to update current max length sequences (i.e. sequences length = i)
		std::vector<T>* pCurrentMaxLengthSequencesBuffer = new std::vector<T>();

		//Calculate buffer of current max length sequences along with search space (matrix/gate collection)
		if(pCurrentMaxLengthSequences == NullPtr) {
			addNewSequencesByApplyingUniversalElements(NullPtr, pUniversalSetIter, m_pCombiner, pCurrentMaxLengthSequencesBuffer, pCollection);
		}
		else {
			for(T pSequence: *(pCurrentMaxLengthSequences)) {
				addNewSequencesByApplyingUniversalElements(pSequence, pUniversalSetIter, m_pCombiner, pCurrentMaxLengthSequencesBuffer, pCollection);
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
void SearchSpaceConstructorUniqueCheckImpl<T>::addNewSequencesByApplyingUniversalElements(T pSequence,
		IteratorPtr<T> pUniversalSetIter,
		CombinerPtr<T> pCombiner,
		std::vector<T>* ppCurrentMaxLengthSequencesBuffer,
		CollectionPtr<T> pCollection) {

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

		if(pAppendedSequence != NullPtr && isUnique(pAppendedSequence)) {
			//Add the newly generated sequence 's matrix/gate to the collection
			pCollection->addElement(pAppendedSequence);

			//Also add to unique list to check later
			addToUniqueList(pAppendedSequence);

			//Also add newly generated sequence's matrix/gate to buffer of current-max length sequences
			ppCurrentMaxLengthSequencesBuffer->push_back(pAppendedSequence);

			m_counter++;
			if(m_counter % PROGRESS_COUNTER == 0) {
				std::cout <<  "Added " << m_counter << " items \n";
			}
		}

		//Go to next universal matrix/gate
		pUniversalSetIter->next();
	}

	//Rewind universal matrix/gate iterator to begin
	pUniversalSetIter->toBegin();
}

template<typename T>
bool SearchSpaceConstructorUniqueCheckImpl<T>::isUnique(T appendedSequence) const {
	return true;
}

template<typename T>
void SearchSpaceConstructorUniqueCheckImpl<T>::addToUniqueList(T appendedSequence) {
	//Do nothing
}

template<typename T>
std::vector<T>* SearchSpaceConstructorUniqueCheckImpl<T>::createCurrentMaxLengthSequences() {
	return NullPtr;
}

template<typename T>
int SearchSpaceConstructorUniqueCheckImpl<T>::getBaseCollectionMaxSequenceLength() {
	return 0;
}
