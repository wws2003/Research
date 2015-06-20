/*
 * SampleGateCombinerImpl.h
 *
 *  Created on: Jun 20, 2015
 *      Author: pham
 */

#ifndef SAMPLEGATECOMBINERIMPL_H_
#define SAMPLEGATECOMBINERIMPL_H_

#include "ICombiner.h"
#include "AlgoCommon.h"
#include "AlgoInternal.h"
#include "Gate.h"
#include "IMatrixOperator.h"
#include "OperatorCommon.h"
#include <map>

typedef std::multimap<std::string, std::string> GateLabelCancelationMap;

class SampleGateCombinerImpl: public ICombiner<GatePtr> {
public:
	SampleGateCombinerImpl(cost_t maxCost, const GateLabelCancelationMap& cancelationMap, MatrixOperatorPtr pMatrixOperator);
	virtual ~SampleGateCombinerImpl(){};

	virtual void combine(GatePtr pGate1, GatePtr pGate2, GatePtr& result);

private:
	cost_t m_maxCost;
	GateLabelCancelationMap m_cancelationMap;
	MatrixOperatorPtr m_pMatrixOperator;
};




#endif /* SAMPLEGATECOMBINERIMPL_H_ */
