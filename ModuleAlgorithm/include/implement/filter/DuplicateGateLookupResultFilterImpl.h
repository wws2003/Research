/*
 * DuplicateGateLookupResultFilterImpl.h
 *
 *  Created on: Nov 27, 2015
 *      Author: pham
 */

#ifndef DUPLICATEGATELOOKUPRESULTFILTERIMPL_H_
#define DUPLICATEGATELOOKUPRESULTFILTERIMPL_H_

#include "Gate.h"
#include "DuplicateLookupResultFilterImpl.h"

class DuplicateGateLookupResultFilterImpl : public DuplicateLookupResultFilterImpl<GatePtr> {
protected:
	//Override
	bool sameElement(const GatePtr& pGate1, const GatePtr& pGate2) const;
};


#endif /* DUPLICATEGATELOOKUPRESULTFILTERIMPL_H_ */
