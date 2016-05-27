/*
 * ResourcesCommon.h
 *
 *  Created on: Apr 13, 2016
 *      Author: pham
 */

#ifndef RESOURCESCOMMON_H_
#define RESOURCESCOMMON_H_

#include "MathConceptsCommon.h"

enum LibrarySet {
	L_HT,//No T^-1
	L_HTS,//No S^-1
	L_HTCNOT,
	L_HTtCNOT, //With T^-1
	L_HTSCNOT,
	L_UNSPECIFIED
};

enum RotationType {
	R_X,
	R_Y,
	R_Z,
	C_RX,//For controlled gate
	C_RY,//For controlled gate
	C_RZ, //For controlled gate
	C_UNSPECIFIED
};

struct RotationConfig {
	RotationType m_rotationType;
	mreal_t m_rotationAngle;
};

typedef std::vector<RotationConfig> RotationConfigs;

class IGateStore;
typedef IGateStore* GateStorePtr;

class IGateStoreFactory;
typedef IGateStoreFactory* GateStoreFactoryPtr;

class IGateCombinabilityChecker;
typedef IGateCombinabilityChecker* GateCombinabilityCheckerPtr;

typedef std::vector<GateCombinabilityCheckerPtr> GateCombinabilityCheckers;

class IGateCombinabilityCheckerFactory;
typedef IGateCombinabilityCheckerFactory* GateCombinabilityCheckerFactoryPtr;

#endif /* RESOURCESCOMMON_H_ */
