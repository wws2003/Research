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

SearchSpaceConstructorImpl::SearchSpaceConstructorImpl(MatrixOperatorPtr pMatrixOperator, MatrixPtrArray pUniversalMatrices, int universalSetSize) {
	m_pMatrixOperator = pMatrixOperator;
	m_pUniversalMatrices = new MatrixPtr[universalSetSize];
	m_unversalSetSize = universalSetSize;
}

SearchSpaceConstructorImpl::~SearchSpaceConstructorImpl() {
	delete[] m_pUniversalMatrices;
}

void SearchSpaceConstructorImpl::constructSearchSpace(MatrixCollectionPtr pMatrixCollection, MatrixCollectionPtr pUniversalSet, int maxSequenceLength) {
	//Firstly add all single matrices in universal set into the persistable collection
	pMatrixCollection->clear();

	MatrixIteratorPtr pUniversalSetIter = pUniversalSet->getIteratorPtr();
	pUniversalSetIter->toBegin();

	while(!pUniversalSetIter->isDone()) {
		pMatrixCollection->addMatrix(pUniversalSetIter->getObj());
	}

	//Compose matrix sequences from universal matrices up to sequence length max
	for (int i = 1; i < maxSequenceLength; i++) {
		MatrixIteratorPtr pCurrentSearchSetIter = pMatrixCollection->getReadonlyIteratorPtr();

		/*
		 *Tranverse through matrices in the search set so far, the apply the multplying operator with
		 *matrices in the universal set
		 */
		pCurrentSearchSetIter->toBegin();

		while(!pCurrentSearchSetIter->isDone()) {
			MatrixPtr pSequence = pCurrentSearchSetIter->getObj();

			MatrixIteratorPtr pUniversalSetIter = pUniversalSet->getIteratorPtr();
			pUniversalSetIter->toBegin();

			while(!pUniversalSetIter->isDone()) {
				MatrixPtr pUniversalMatrix = pUniversalSetIter->getObj();
				MatrixPtr pAppendedSequence = NullPtr;

				//Apply multiply operator presenting the append of universal matrix to the sequence
				m_pMatrixOperator->multiply(pSequence, pUniversalMatrix, pAppendedSequence);

				//Add the newly generated sequence 's matrix to the collection
				pMatrixCollection->addMatrix(pAppendedSequence);

				//Go to next universal matrix
				pUniversalSetIter->next();
			}

			//Rewind universal matrix iterator to begin
			pUniversalSetIter->toBegin();

			pCurrentSearchSetIter->next();
		}
	}
}

