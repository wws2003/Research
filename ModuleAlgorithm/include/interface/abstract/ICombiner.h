/*
 * ICombiner.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef ICOMBINER_H_
#define ICOMBINER_H_

#include "AlgoCommon.h"

template<typename T>
class ICombiner {
public:
	virtual ~ICombiner(){};

	virtual void combine(T element1, T element2, T& result) = 0;
};


#endif /* ICOMBINER_H_ */
