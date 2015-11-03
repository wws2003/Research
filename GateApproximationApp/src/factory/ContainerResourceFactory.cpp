/*
 * ContainerResourceFactory.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: pham
 */

#include "ContainerResourceFactory.h"
#include "HTVBasedResourceContainerImpl.h"
#include "HVBasedResourceContainerImpl.h"
#include "SampleLibraryMatrixStore.h"

ResourceContainerFactory::ResourceContainerFactory() {

}

ResourceContainerFactory::~ResourceContainerFactory() {

}

ResourceContainerPtr ResourceContainerFactory::getResourceContainer(LibrarySet librarySet, MatrixFactoryPtr pMatrixFactory,
		MatrixOperatorPtr pMatrixOperator) {
	ResourceContainerPtr pResourceContainer = NullPtr;
	switch(librarySet) {
	case L_HT:
		pResourceContainer = ResourceContainerPtr(new SampleResourceContainerImpl(pMatrixOperator, pMatrixFactory));
		break;
	case L_HCV:
		pResourceContainer = ResourceContainerPtr(new HVBasedResourceContainerImpl(pMatrixOperator, pMatrixFactory));
		break;
	case L_HTCNOT:
		pResourceContainer = ResourceContainerPtr(new SampleResourceContainerImpl(pMatrixOperator, pMatrixFactory));
		break;
	case L_HTCV:
		pResourceContainer = ResourceContainerPtr(new HTVBasedResourceContainerImpl(pMatrixOperator, pMatrixFactory));
		break;
	default:
		pResourceContainer = ResourceContainerPtr(new SampleResourceContainerImpl(pMatrixOperator, pMatrixFactory));
	}

	pResourceContainer->setup();
	return pResourceContainer;
}
