/*
 * AbstractVectorizableMatrixCollection.cpp
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#include "abstract/basic/algo/AbstractVectorizbleMatrixCollection.h"
#include "abstract/basic/algo/IMatrixVectorMapper.h"
#include "abstract/basic/algo/IVectorIterator.h"
#include "abstract/factory/IMatrixCollectionFactory.h"

AbstractVectorizableMatrixCollection::AbstractVectorizableMatrixCollection(MatrixVectorMapperPtr pMatrixVectorMapper, MatrixCollectionFactoryPtr pMatrixCollectionFactory) : m_pMatrixVectorMapper(pMatrixVectorMapper), m_pMatrixCollectionFactory(pMatrixCollectionFactory) {

}
//Override
MatrixIteratorPtr AbstractVectorizableMatrixCollection::findApproxMatrices(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const {
	VectorPtr pVectorQuery;
	m_pMatrixVectorMapper->matrixToVector(pQuery, pVectorQuery);

	VectorItertatorPtr pFindVectorIter = findApproxVectors(pVectorQuery, pDistanceCalculator, epsilon);

	pFindVectorIter->toBegin();

	MatrixCollectionPtr pResultMatrices = m_pMatrixCollectionFactory->generateEmptyMatrixCollectionPtr();

	while(!pFindVectorIter->isDone()) {
		VectorPtr pVector = pFindVectorIter->getObj();

		MatrixPtr pMatrix;
		m_pMatrixVectorMapper->vectorToMatrix(pVector, pMatrix);

		pResultMatrices->addMatrix(pMatrix);

		pFindVectorIter->next();

	}

	return pResultMatrices->getIteratorPtr();
}




