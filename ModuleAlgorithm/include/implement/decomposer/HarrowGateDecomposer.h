/*
 * HarrowGateDecomposer.h
 *
 *  Created on: Nov 17, 2015
 *      Author: pham
 */

#ifndef HARROWGATEDECOMPOSER_H_
#define HARROWGATEDECOMPOSER_H_

#include "HarrowElementDecomposer.h"
#include "Gate.h"
#include "OperatorCommon.h"
#include "IMatrixOperator.h"

class HarrowGateDecomposer: public HarrowElementDecomposer<GatePtr> {
public:
	HarrowGateDecomposer(RealCoordinateCalculatorPtr<GatePtr> pSphereCoordinateCalculator,
			MatrixOperatorPtr pMatrixOperator);

protected:
	//Override
	virtual void getInverseElement(GatePtr element, GatePtr& inverseElement);

private:
	MatrixOperatorPtr m_pMatrixOperator;
};



#endif /* HARROWGATEDECOMPOSER_H_ */
