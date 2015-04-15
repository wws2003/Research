/*
 * SearchSpaceConstructor.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#include "implement/search/SearchSpaceConstructor.h"

void addMatricesFromIteratorToCollection(MatrixCollectionPtr pCollection, MatrixIteratorPtr pIter);

SearchSpaceConstructor::SearchSpaceConstructor(PersistableMatrixCollectionFactoryPtr pPersistableMatrixCollectionFactory, MatrixCollectionFactoryPtr pMatrixCollectionFactory, MatrixOperatorPtr pMatrixOperatorPtr):
m_pPersistableMatrixCollectionFactory(pPersistableMatrixCollectionFactory),
m_pMatrixCollectionFactory(pMatrixCollectionFactory),
m_pMatrixOperatorPtr(pMatrixOperatorPtr) {

}

PersistableMatrixCollectionPtr SearchSpaceConstructor::createPersistableSearchSpace(const MatrixCollectionPtr pUniversalSet, int instructionMaxLength) {
	PersistableMatrixCollectionPtr pPersistableCollection = m_pPersistableMatrixCollectionFactory->generateEmptyPersistableMatrixCollectionPtr();

	const MatrixIteratorPtr pUniversalSetIterator = pUniversalSet->getIteratorPtr();

	//Firstly add all single matrices in universal set into the persistable collection
	addMatricesFromIteratorToCollection(pPersistableCollection, pUniversalSetIterator);

	for (int i = 1; i < instructionMaxLength; i++) {
		MatrixIteratorPtr pCurrentSearchSetIter = pPersistableCollection->getReadonlyIteratorPtr();

		/*
		 *Tranverse through matrices in the search set so far, the apply the multplying operator with
		 *matrices in the universal set
		 */
		while(!pCurrentSearchSetIter->isDone()) {
			MatrixCollectionPtr pNewProductsCollection = m_pMatrixCollectionFactory->generateEmptyMatrixCollectionPtr();

			MatrixPtr pInstruction = pCurrentSearchSetIter->getMatrixObj();

			pUniversalSetIterator->toBegin();

			//Tranverse through the universal matrices
			while(!pUniversalSetIterator->isDone()) {
				MatrixPtr pUniversalMatrix = pUniversalSetIterator->getMatrixObj();
				MatrixPtr pProduct = NullPtr;

				m_pMatrixOperatorPtr->multiply(pInstruction, pUniversalMatrix, pProduct);

				pNewProductsCollection->addMatrix(pProduct);

				pUniversalSetIterator->next();
			}

			pCurrentSearchSetIter->next();

			addMatricesFromIteratorToCollection(pPersistableCollection, pNewProductsCollection->getIteratorPtr());

			pNewProductsCollection->clear();

			m_pMatrixCollectionFactory->recycle(pNewProductsCollection);
		}
	}

	return pPersistableCollection;
}


void addMatricesFromIteratorToCollection(MatrixCollectionPtr pCollection, MatrixIteratorPtr pIter) {
	pIter->toBegin();

	while(!pIter->isDone()) {
		pCollection->addMatrix(pIter->getMatrixObj());
		pIter->next();
	}

	pIter->toBegin();
}



