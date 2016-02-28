/*
 * GatesCoordinateCombinationVerifyTask.h
 *
 *  Created on: Feb 26, 2016
 *      Author: pham
 */

#ifndef GATESCOORDINATECOMBINATIONVERIFYTASK_H_
#define GATESCOORDINATECOMBINATIONVERIFYTASK_H_

#include "ElementsCombinationVerifyTask.h"
#include "Gate.h"
#include "MathConceptsCommon.h"
#include "Coordinate.hpp"
#include "ITask.h"
#include "TaskCommon.h"

class GatesCoordinateCombinationVerifyTask: public ITask<LookupResult<GateRealCoordinate> > {
public:
	enum TaskResultCode {
		TRC_UNKNOWN = 0,
		TRC_POSITIVE = 1,
		TRC_NEGATIVE = -1,
	};

	GatesCoordinateCombinationVerifyTask(const std::vector<GateRealCoordinate>& elements,
			CombinerPtr<GateRealCoordinate> pCombiner,
			DistanceCalculatorPtr<GateRealCoordinate> pDistanceCalculator,
			GateRealCoordinate target,
			mreal_t epsilon,
			bool elementsOrder);

	virtual ~GatesCoordinateCombinationVerifyTask(){};

	//Override
	TaskResult<LookupResult<GateRealCoordinate> > execute();

private:
	void composeCandidate(const std::vector<GateRealCoordinate>& elements, GateRealCoordinate& result);

	const std::vector<GateRealCoordinate> m_elements;
	CombinerPtr<GateRealCoordinate> m_pCombiner;
	DistanceCalculatorPtr<GateRealCoordinate> m_pDistanceCalculator;
	GateRealCoordinate m_target;
	mreal_t m_epsilon;

	bool m_elementsOrder;
};


#endif /* GATESCOORDINATECOMBINATIONVERIFYTASK_H_ */
