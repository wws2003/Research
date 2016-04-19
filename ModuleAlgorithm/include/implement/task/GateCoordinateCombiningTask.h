/*
 * GateCoordinateCombiningTask.h
 *
 *  Created on: Feb 24, 2016
 *      Author: pham
 */

#ifndef GATECOORDINATECOMBININGTASK_H_
#define GATECOORDINATECOMBININGTASK_H_

#include "Coordinate.hpp"
#include "Gate.h"
#include "ICombiner.h"
#include "MathConceptsCommon.h"
#include "ICollection.h"
#include "ITask.h"

class GateCoordinateCombiningTask: public ITask<LookupResult<GateRealCoordinate> > {
public:
	enum TaskResultCode {
		TRC_UNKNOWN = 0,
		TRC_VALID = 1,
		TRC_INVALID = 2,
	};

	GateCoordinateCombiningTask(const std::vector<GateRealCoordinate>& elements,
			CombinerPtr<GateRealCoordinate> pCombiner,
			bool elementsOrder);

	virtual ~GateCoordinateCombiningTask(){};

	//Override
	TaskResult<LookupResult<GateRealCoordinate> > execute();

private:
	void composeCandidate(const std::vector<GateRealCoordinate>& elements,
			GateRealCoordinate& result);

	bool m_elementsOrder;
	const std::vector<GateRealCoordinate> m_elements;
	CombinerPtr<GateRealCoordinate> m_pCombiner;
};

#endif /* GATECOORDINATECOMBINATIONVERIFYTASK_H_ */
