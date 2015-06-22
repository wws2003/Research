/*
 * IEquivalentQueryGenerator.h
 *
 *  Created on: Jun 22, 2015
 *      Author: pham
 */

#ifndef IEQUIVALENTQUERYGENERATOR_H_
#define IEQUIVALENTQUERYGENERATOR_H_

#include "AlgoCommon.h"
#include "OperatorCommon.h"
#include "AlgoInternal.h"
#include "ICollection.h"

template<typename T>
class IEquivalentQueryGenerator {
public:
	virtual ~IEquivalentQueryGenerator(){};

	virtual void generateEquivalentQueries(T query, std::vector<T>& equivalentQueries) = 0;
};



#endif /* IEQUIVALENTQUERYGENERATOR_H_ */
