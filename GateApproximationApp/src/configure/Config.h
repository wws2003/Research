/*
 * Config.h
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "MathConceptsCommon.h"
#include "ResourcesCommon.h"

enum MatrixDistanceCalculatorType {
	MDCT_FOWLER = 1,
	MDCT_TRACE = 2
};

struct CollectionConfig {
	LibrarySet m_librarySet;
	int m_maxSequenceLength;
	int m_nbQubits;
	MatrixDistanceCalculatorType m_matrixDistanceCalculatorType = MDCT_FOWLER;
};

struct EvaluatorConfig {
	double m_collectionEpsilon;
	double m_approximatorEpsilon;
	RotationConfigs m_rotationTargets;
};

struct SKApproximatorConfig {
	mreal_t m_initialEpsilon;
	int m_recursiveLevels;
	int m_nbCandidates;
};

enum ComposerType {
	CT_SIMPLE = 1,
	CT_COORDINATE_ADDTIONAL_BASED = 2
};

enum DecomposerType {
	DT_NEAR_BALANCE = 1,
	DT_GC_SIMPLE = 2,
	DT_GC_MULTI_LEVELS = 3
};

struct ComposerBasedApproximatorConfig {
	int m_nbPartialQueries;
	mreal_t m_initialEpsilon;
	int m_nbCandidates;
	ComposerType m_composerBasedApproximatorType;
	DecomposerType m_queryDecomposerType;
	int m_buildingBlockComposerType;
	bool m_userFilter;
};

enum CoordinateComparatorTypes {
	CMP_DICTIONARY = 1,
	CMP_SUM = 2,
	CMP_ONE_ELEMENT = 3
};

typedef std::vector<int> AppliedCoordinateIndices;

struct CoordinateComparatorConfig {
	CoordinateComparatorTypes m_coordinateComparatorType = CMP_DICTIONARY;
	mreal_t m_coordinateEpsilon = 0.0;
	AppliedCoordinateIndices m_appliedCoordinateIndices; //Just a work-around for CMP_ONE_ELEMENT
};

typedef std::vector<CoordinateComparatorConfig> CoordinateComparatorConfigList;

struct CoordinateAdditionalBasedComposerConfig {
	CoordinateComparatorConfig m_primaryCoordinateComparatorConfig;
	CoordinateComparatorConfigList m_secondaryCoordinateComparatorConfigs;
};

struct ComposerEvaluatorConfig {
	int m_nbPartialQueries;
	mreal_t m_initialEpsilon;
	int m_nbCandidates;
	DecomposerType m_queryDecomposerType;
	double m_composeEpsilon;
	RotationConfigs m_rotationTargets;
};

struct ParallelConfig {
	int m_nbThreads;
	int m_taskBufferSize;
	int m_taskFutureBufferSize;
};

#endif /* CONFIG_H_ */
