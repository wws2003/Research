/*
 * IBinPatternCalculator.h
 *
 *  Created on: Oct 25, 2015
 *      Author: pham
 */

#ifndef IBINPATTERNCALCULATOR_H_
#define IBINPATTERNCALCULATOR_H_

#include "AlgoInternal.h"
#include "MathConceptsCommon.h"

typedef std::vector<mreal_t> real_coordinate_t;

class IBinPatternCalculator {
public:
	virtual ~IBinPatternCalculator(){};

	virtual void calculateBinPattern(const real_coordinate_t& queryCoordinate,
			const real_coordinate_t& apprxCoordinate,
			BinPattern& binPattern) = 0;
};



#endif /* IBINPATTERNCALCULATOR_H_ */
