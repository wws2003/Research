/*
 * NearIdentityElementApproximator.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef NEARIDENTITYELEMENTAPPROXIMATOR_H_
#define NEARIDENTITYELEMENTAPPROXIMATOR_H_

#include "OperatorCommon.h"
#include "ICollection.h"
#include "ICombiner.h"
#include "AlgoInternal.h"
#include "IApproximator.h"
#include "OperatorCommon.h"

/**
 * This generic class aims to serve both gate and matrix approximator
 */

/*template<typename T>
class NearIdentityElementApproximator: public IApproximator<T> {
public:
	virtual ~NearIdentityElementApproximator();
	virtual IteratorPtr<T> getApproximateElements(CollectionPtr<T> pCoreCollection, T pQuery, DistanceCalculatorPtr<T> pDistanceCalculator, double epsilon);

private:
	RealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	CombinerPtr m_pMatrixCombiner;
	BinCollectionPtr m_pMatrixBinCollection;
};
*/
#endif /* NEARIDENTITYELEMENTAPPROXIMATOR_H_ */
