/*
 * GateCombinerImpl.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef GATECOMBINERIMPL_H_
#define GATECOMBINERIMPL_H_

#include "ICombiner.h"
#include "AlgoCommon.h"
#include "AlgoInternal.h"
#include "Gate.h"
#include "IMatrixOperator.h"
#include "OperatorCommon.h"
#include <vector>

typedef std::vector<GateCombinabilityCheckerPtr> GateCombinabilityCheckers;

class GateCombinerImpl: public ICombiner<GatePtr> {
public:
	GateCombinerImpl(const GateCombinabilityCheckers& combinabilityCheckers, MatrixOperatorPtr pMatrixOperator);
	virtual ~GateCombinerImpl(){};

	virtual void combine(GatePtr pGate1, GatePtr pGate2, GatePtr& result);

private:
	GateCombinabilityCheckers m_combinabilityCheckers;
	MatrixOperatorPtr m_pMatrixOperator;
};



#endif /* GATECOMBINERIMPL_H_ */
