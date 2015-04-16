/*
 * AbstractVectorizbleMatrixCollection.h
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#ifndef ABSTRACTVECTORIZBLEMATRIXCOLLECTION_H_
#define ABSTRACTVECTORIZBLEMATRIXCOLLECTION_H_

#include "common/Common.h"
#include "IMatrixCollection.h"

class AbstractVectorizableMatrixCollection: public IMatrixCollection {
public:
	AbstractVectorizableMatrixCollection(MatrixVectorMapperPtr pMatrixVectorMapper, MatrixCollectionFactoryPtr pMatrixCollectionFactory);
	virtual ~AbstractVectorizableMatrixCollection(){};

	//Override
	virtual MatrixIteratorPtr findApproxMatrices(MatrixPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const;

	virtual VectorItertatorPtr findApproxVectors(VectorPtr pQuery, MatrixDistanceCalculatorPtr pDistanceCalculator, double epsilon) const = 0;

private:
	MatrixVectorMapperPtr m_pMatrixVectorMapper;
	MatrixCollectionFactoryPtr m_pMatrixCollectionFactory;
};


#endif /* ABSTRACTVECTORIZBLEMATRIXCOLLECTION_H_ */
