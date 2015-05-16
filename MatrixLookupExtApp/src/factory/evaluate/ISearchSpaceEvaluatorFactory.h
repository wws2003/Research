/*
 * ISearchSpaceEvaluatorFactory.h
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#ifndef ISEARCHSPACEEVALUATORFACTORY_H_
#define ISEARCHSPACEEVALUATORFACTORY_H_

#include "EvaluateCommon.h"

class ISearchSpaceEvaluatorFactory {
public:
	virtual ~ISearchSpaceEvaluatorFactory(){};
	virtual SearchSpaceEvaluatorPtr getSearchSpaceEvaluatorPtr() = 0;
};


#endif /* ISEARCHSPACEEVALUATORFACTORY_H_ */
