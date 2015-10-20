/*
 * LazyGateDistanceCalculator.h
 *
 *  Created on: Oct 20, 2015
 *      Author: pham
 */

#ifndef LAZYGATEDISTANCECALCULATORIMPL_H_
#define LAZYGATEDISTANCECALCULATORIMPL_H_

#include "IDistanceCalculator.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"
#include "ILibraryMatrixStore.h"

class LazyGateDistanceCalculatorImpl: public IGateDistanceCalculator {
public:
	LazyGateDistanceCalculatorImpl(MatrixDistanceCalculatorPtr pMatrixDistanceCalculator,
			MatrixOperatorPtr pMatrixOperator,
			LibraryMatrixStorePtr pLibraryMatrixStore);

	virtual ~LazyGateDistanceCalculatorImpl(){};

	virtual mreal_t distance(GatePtr pGate1, GatePtr pGate2);

private:
	void provideMatrixToGate(GatePtr pGate);
	MatrixPtr getMatrixFromGateLabelSequence(const LabelSeq& sequence);

	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	MatrixOperatorPtr m_pMatrixOperator;
	LibraryMatrixStorePtr m_pLibraryMatrixStore;
};


#endif /* LAZYGATEDISTANCECALCULATOR_H_ */
