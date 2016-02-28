/*
 * ElementsCombinationVerifyTask.h
 *
 *  Created on: Feb 21, 2016
 *      Author: pham
 */

#ifndef ELEMENTSCOMBINATIONVERIFYTASK_H_
#define ELEMENTSCOMBINATIONVERIFYTASK_H_

#include "IDistanceCalculator.h"
#include "ICombiner.h"
#include "AlgoCommon.h"
#include "AlgoInternal.h"
#include "ITask.h"
#include "ICollection.h"
#include "TaskCommon.h"
#include <mutex>

template<typename T>
class ElementsCombinationVerifyTask: public ITask<LookupResult<T> > {
public:
	enum TaskResultCode {
		TRC_UNKNOWN = 0,
		TRC_POSITIVE = 1,
		TRC_NEGATIVE = 2,
	};

	ElementsCombinationVerifyTask(const std::vector<T>& elements,
			CombinerPtr<T> pCombiner,
			DistanceCalculatorPtr<T> pDistanceCalculator,
			T target,
			mreal_t epsilon);

	virtual ~ElementsCombinationVerifyTask(){};

	//Override
	TaskResult<LookupResult<T> > execute();

protected:
	virtual void onCombinationVerified(const LookupResult<T>& verifiedResult);

private:
	void composeCandidate(const std::vector<T>& elements, T& result);

	const std::vector<T> m_elements;
	CombinerPtr<T> m_pCombiner;
	DistanceCalculatorPtr<T> m_pDistanceCalculator;
	T m_target;
	mreal_t m_epsilon;
};


#endif /* ELEMENTSCOMBINATIONVERIFYTASK_H_ */
