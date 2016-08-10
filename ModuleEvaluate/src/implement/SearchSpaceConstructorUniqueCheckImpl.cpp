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
SearchSpaceConstructorUniqueCheckImpl<T>::SearchSpaceConstructorUniqueCheckImpl(CollectionPtr<T> pBaseCollection,
		int baseLength,
		CombinerPtr<T> pCombiner) {

	m_pBaseCollection = pBaseCollection;
	m_baseLength = baseLength;

	m_pCombiner = pCombiner;
	m_counter = 0;
}

template<typename T>
SearchSpaceConstructorUniqueCheckImpl<T>::SearchSpaceConstructorUniqueCheckImpl(CombinerPtr<T> pCombiner) {
	m_pBaseCollection = NullPtr;
	m_baseLength = 0;

	m_pCombiner = pCombiner;
	m_counter = 0;
}

template<typename T>
void SearchSpaceConstructorUniqueCheckImpl<T>::constructSearchSpace(CollectionPtr<T> pCollection,
		CollectionPtr<T> pUniversalSet,
		int maxSequenceLength) {

	pCollection->clear();

	//Firstly add all elements in base collection if applicable
	//Also remember (add to unique list elements in base collection if applicable)
	std::vector<T>* pCurrentMaxLengthSequences = NullPtr;

	processBaseCollection(pCollection, pCurrentMaxLengthSequences);

	//Compose matrix sequences from universal matrices up to sequence length max
	IteratorPtr<T> pUniversalSetIter = pUniversalSet->getReadonlyIteratorPtr();

	int startCounter = m_baseLength;
	for(int i = startCounter; i < maxSequenceLength; i++) {
		//Use a buffer to update current max length sequences (i.e. sequences length = i)
		std::vector<T>* pCurrentMaxLengthSequencesBuffer = new std::vector<T>();

		//Calculate buffer of current max length sequences along with search space (matrix/gate collection)
		bool isMaxLengthReached = (i >= maxSequenceLength - 1);
		if(pCurrentMaxLengthSequences == NullPtr) {
			addNewSequencesByApplyingUniversalElements(NullPtr,
					pUniversalSetIter,
					m_pCombiner,
					pCurrentMaxLengthSequencesBuffer,
					isMaxLengthReached,
					pCollection);
		}
		else {
			for(T pSequence: *(pCurrentMaxLengthSequences)) {
				addNewSequencesByApplyingUniversalElements(pSequence,
						pUniversalSetIter,
						m_pCombiner,
						pCurrentMaxLengthSequencesBuffer,
						isMaxLengthReached,
						pCollection);
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
		bool isMaxLengReached,
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
			addToUniqueList(pAppendedSequence, isMaxLengReached);

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

//MARK: Private methods
template<typename T>
void SearchSpaceConstructorUniqueCheckImpl<T>::processBaseCollection(CollectionPtr<T> pTargetCollection,
		std::vector<T>*& pCurrentMaxLengthSequences) {

	if(m_pBaseCollection != NullPtr) {
		pCurrentMaxLengthSequences = new std::vector<T>();

		IteratorPtr<T> pBaseIter = m_pBaseCollection->getIteratorPtr();
		if(pBaseIter != NullPtr) {
			while(!pBaseIter->isDone()) {
				T baseElement = pBaseIter->getObj()->clone();

				pTargetCollection->addElement(baseElement);
				addToUniqueList(baseElement, false);

				if(isMaxLengthBaseElement(baseElement)) {
					pCurrentMaxLengthSequences->push_back(baseElement);
				}

				pBaseIter->next();
			}
		}
		_destroy(pBaseIter);
	}
}
