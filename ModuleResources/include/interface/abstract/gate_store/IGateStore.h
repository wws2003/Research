/*
 * IGateStore.h
 *
 *  Created on: Apr 13, 2016
 *      Author: pham
 */

#ifndef IGATESTORE_H_
#define IGATESTORE_H_

#include "Gate.h"
#include "ResourcesCommon.h"
#include <vector>

class IGateStore {
public:
	virtual ~IGateStore(){};

	virtual void getLibraryGates(std::vector<GatePtr>& pLibraryGates, LibrarySet librarySet) = 0;

	virtual void getIdentityGates(std::vector<GatePtr>& pIdentityGates, bool isPhaseIgnored) = 0;

	virtual void getMatrixOrthonormalBasis(std::vector<MatrixPtr>& pBasis) = 0;

	virtual void getRotationTargets(std::vector<GatePtr>& targets, const RotationConfigs& rotationTargetsConfig) = 0;
};

#endif /* IGATESTORE_H_ */
