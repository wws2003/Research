/*
 * Config.h
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "MathConceptsCommon.h"

enum LibrarySet {
	L_HT,
	L_HTCNOT,
	L_HCV,
	L_HTCV,
	L_UNSPECIFIED
};

enum RotationType {
	R_X,
	R_Y,
	R_Z,
	C_RX,//For controlled gate
	C_RY,//For controlled gate
	C_RZ //For controlled gate
};

typedef struct _RotationInfo {
	RotationType m_rotationType;
	mreal_t m_rotationAngle;
}
RotationConfig;

typedef std::vector<RotationConfig> RotationConfigs;

struct CollectionConfig {
	LibrarySet m_librarySet;
	int m_maxSequenceLength;
	int m_nbQubits;
};

struct EvaluatorConfig {
	double m_collectionEpsilon;
	double m_approximatorEpsilon;
	RotationConfigs m_rotationTargets;
};

struct NearIdentityApproximatorConfig {
	int m_maxMergedBinDistance;
	mreal_t m_maxCandidateEpsilon;
	mreal_t m_maxCandidateEpsilonDecreaseFactor;
	int m_iterationMaxSteps;
	int m_maxResultNumber;
	mreal_t m_initialEpsilon;
};

struct ComposerBasedApproximatorConfig {
	int m_nbPartialQueries;
	mreal_t m_initialEpsilon;
	int m_nbCandidates;
	int m_composerBasedApproximatorType;
	int m_queryDecomposerType;
	int m_buildingBlockComposerType;
	bool m_userFilter;
};

struct SKApproximatorConfig {
	mreal_t m_initialEpsilon;
	int m_recursiveLevels;
	int m_nbCandidates;
};

struct SKApproximatorConfig2 : public SKApproximatorConfig {
	mreal_t m_coordinateEpsilon;
	int m_coordinateComparatorType;
};

#endif /* CONFIG_H_ */
