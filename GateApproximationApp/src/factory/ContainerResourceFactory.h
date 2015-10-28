/*
 * ContainerResourceFactory.h
 *
 *  Created on: Oct 28, 2015
 *      Author: pham
 */

#ifndef CONTAINERRESOURCEFACTORY_H_
#define CONTAINERRESOURCEFACTORY_H_

#include "Config.h"
#include "IResourceContainer.h"
#include "ApplicationCommon.h"
#include "OperatorCommon.h"
#include "IMatrixFactory.h"
#include "IMatrixOperator.h"

class ResourceContainerFactory {
public:
	ResourceContainerFactory();
	virtual ~ResourceContainerFactory();

	ResourceContainerPtr getResourceContainer(LibrarySet librarySet, MatrixFactoryPtr pMatrixFactory,
			MatrixOperatorPtr pMatrixOperator);

};

#endif /* CONTAINERRESOURCEFACTORY_H_ */
