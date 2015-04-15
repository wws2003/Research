/*
 * SearchSpaceConstructor.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef SEARCHSPACECONSTRUCTOR_H_
#define SEARCHSPACECONSTRUCTOR_H_

#include "abstract/basic/algo/IMatrixOperator.h"
#include "abstract/basic/algo/IMatrixIterator.h"
#include "abstract/factory/IMatrixCollectionFactory.h"
#include "abstract/factory/IPersistableMatrixCollectionFactory.h"

class SearchSpaceConstructor {
public:
	SearchSpaceConstructor(PersistableMatrixCollectionFactoryPtr pPersistableMatrixCollectionFactory, MatrixCollectionFactoryPtr pMatrixCollectionFactory, MatrixOperatorPtr pMatrixOperatorPtr);
	virtual ~SearchSpaceConstructor(){};

	virtual PersistableMatrixCollectionPtr createPersistableSearchSpace(const MatrixCollectionPtr pMatrixCollection, int instructionMaxLength);

private:
	PersistableMatrixCollectionFactoryPtr m_pPersistableMatrixCollectionFactory;
	MatrixCollectionFactoryPtr m_pMatrixCollectionFactory;
	MatrixOperatorPtr m_pMatrixOperatorPtr;
};


#endif /* SEARCHSPACECONSTRUCTOR_H_ */
