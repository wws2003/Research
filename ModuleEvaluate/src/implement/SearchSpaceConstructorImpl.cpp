/*
 * SearchSpaceConstructorImpl.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#include "SearchSpaceConstructorImpl.h"
#include "IMatrixCollection.h"
#include "IIterator.h"
#include "IMatrixOperator.h"
#include <vector>

typedef std::vector<MatrixPtr> MatrixPtrVector;
typedef std::vector<MatrixPtr>* MatrixPtrVectorPtr;

void addNewSequencesByApplyingUniversalMatrices(MatrixPtr pSequence, MatrixIteratorPtr pUniversalSetIter, MatrixCombinerPtr pMatrixCombiner, MatrixPtrVectorPtr ppCurrentMaxLengthSequencesBuffer, MatrixCollectionPtr pMatrixCollection);

SearchSpaceConstructorImpl::SearchSpaceConstructorImpl(MatrixCombinerPtr pMatrixCombiner) {
	m_pMatrixCombiner = pMatrixCombiner;
}

SearchSpaceConstructorImpl::~SearchSpaceConstructorImpl() {

}

void SearchSpaceConstructorImpl::constructSearchSpace(MatrixCollectionPtr pMatrixCollection, MatrixCollectionPtr pUniversalSet, int maxSequenceLength) {
	//Firstly add all single matrices in universal set into the persistable collection
	pMatrixCollection->clear();

	MatrixIteratorPtr pUniversalSetIter = pUniversalSet->getReadonlyIteratorPtr();

	MatrixPtrVectorPtr pCurrentMaxLengthSequences = NullPtr;

	//Compose matrix sequences from universal matrices up to sequence length max
	for(int i = 0; i < maxSequenceLength; i++) {
		//Use a buffer to update current max length sequences (i.e. sequences length = i)
		MatrixPtrVectorPtr pCurrentMaxLengthSequencesBuffer = new MatrixPtrVector();

		//Calculate buffer of current max length sequences along with search space (matrix collection)
		if(pCurrentMaxLengthSequences == NullPtr) {
			addNewSequencesByApplyingUniversalMatrices(NullPtr, pUniversalSetIter, m_pMatrixCombiner, pCurrentMaxLengthSequencesBuffer, pMatrixCollection);
		}
		else {
			for(MatrixPtr pSequence: *(pCurrentMaxLengthSequences)) {
				addNewSequencesByApplyingUniversalMatrices(pSequence, pUniversalSetIter, m_pMatrixCombiner, pCurrentMaxLengthSequencesBuffer, pMatrixCollection);
			}
			delete pCurrentMaxLengthSequences;
		}

		//Restore current max length sequences from buffer if maxSequenceLength not reached
		if(i + 1 < maxSequenceLength) {
			pCurrentMaxLengthSequences = pCurrentMaxLengthSequencesBuffer;
		}
	}
}

void addNewSequencesByApplyingUniversalMatrices(MatrixPtr pSequence, MatrixIteratorPtr pUniversalSetIter, MatrixCombinerPtr pMatrixCombiner, MatrixPtrVectorPtr ppCurrentMaxLengthSequencesBuffer, MatrixCollectionPtr pMatrixCollection) {
	pUniversalSetIter->toBegin();

	while(!pUniversalSetIter->isDone()) {
		MatrixPtr pUniversalMatrix = pUniversalSetIter->getObj();
		MatrixPtr pAppendedSequence = NullPtr;

		if(pSequence == NullPtr) {
			//If current sequence is null, i.e. empty sequence, add the universal matrix it self to collection
			pAppendedSequence = pUniversalMatrix;
		}
		else {
			//Otherwise apply multiply operator presenting the append of universal matrix to the sequence
			pMatrixCombiner->combine(pSequence, pUniversalMatrix, pAppendedSequence);
		}

		if(pAppendedSequence != NullPtr) {
			//Add the newly generated sequence 's matrix to the collection
			pMatrixCollection->addMatrix(pAppendedSequence);

			//Also add newly generated sequence's matrix to buffer of current-max length sequences
			ppCurrentMaxLengthSequencesBuffer->push_back(pAppendedSequence);
		}

		//Go to next universal matrix
		pUniversalSetIter->next();
	}

	//Rewind universal matrix iterator to begin
	pUniversalSetIter->toBegin();
}

