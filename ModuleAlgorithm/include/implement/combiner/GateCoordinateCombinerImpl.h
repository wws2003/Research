/*
 * GateCoordinateCombinerImpl.h
 *
 *  Created on: Dec 7, 2015
 *      Author: pham
 */

#ifndef GATECOORDINATECOMBINERIMPL_H_
#define GATECOORDINATECOMBINERIMPL_H_

#include "Coordinate.hpp"
#include "Gate.h"
#include "ICombiner.h"

class GateCoordinateCombinerImpl: public ICombiner<RealCoordinate<GatePtr> > {
public:
	GateCoordinateCombinerImpl(CombinerPtr<GatePtr> pGateCombiner);
	virtual ~GateCoordinateCombinerImpl(){};

	//Override
	void combine(RealCoordinate<GatePtr> gateCoord1, RealCoordinate<GatePtr> gateCoord2, RealCoordinate<GatePtr>& gateCoordResult);

private:
	CombinerPtr<GatePtr> m_pGateCombiner;
};



#endif /* GATECOORDINATECOMBINERIMPL_H_ */
