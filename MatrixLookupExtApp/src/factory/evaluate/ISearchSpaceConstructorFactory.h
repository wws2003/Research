/*
 * ISearchSpaceConstructorFactory.h
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#ifndef ISEARCHSPACECONSTRUCTORFACTORY_H_
#define ISEARCHSPACECONSTRUCTORFACTORY_H_

#include "EvaluateCommon.h"
#include "ISearchSpaceConstructor.h"

class ISearchSpaceConstructorFactory {
public:
	virtual SearchSpaceConstructorPtr getSearchSpaceConstructorPtr() = 0;
	virtual ~ISearchSpaceConstructorFactory(){};
};


#endif /* ISEARCHSPACECONSTRUCTORFACTORY_H_ */
