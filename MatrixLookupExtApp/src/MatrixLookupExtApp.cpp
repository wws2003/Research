//============================================================================
// Name        : MatrixLookupExtApp.cpp
// Author      : ngc
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "AlgoCommon.h"
#include "EvaluateCommon.h"
#include "IMatrixCollection.h"
#include "IMatrixApproximator.h"
#include "IPersistableMatrixCollection.h"
#include "ISearchSpaceConstructor.h"
#include "ISearchSpaceEvaluator.h"
#include "MatrixLookupExtApp.h"

int main(int argc, char* argv[]) {

	//TODO Wire to concrete instances
	SearchSpaceConstructorPtr pSearchSpaceConstructor = NullPtr;
	SearchSpaceEvaluatorPtr pSearchSpaceEvaluator = NullPtr;
	MatrixCollectionPtr pMatrixCollection = NullPtr;

	RunMode runMode = detectRunMode(argc, argv);

	switch (runMode) {

	case GENERATE_SAVE:
	{
		GenerateSaveModeParams params;
		getGenerateSaveModeParams(argc, argv, params);

		//Generate
		//TODO Wire to concrete instance
		PersitableMatrixCollectionPtr pUniversalSet = NullPtr;
		PersitableMatrixCollectionPtr pPersistableMatrixCollection = NullPtr;
		pUniversalSet->load(params._universalSetSavedLocation);
		pSearchSpaceConstructor->constructSearchSpace(pMatrixCollection, pUniversalSet, params._maxSequenceLength);

		//Save
		pPersistableMatrixCollection->save(params._searchSpaceSavedLocation);

		break;
	}
	case LOAD_EVALUATE:
	{
		LoadEvaluateModeParams params;
		getLoadEvaluateModeParams(argc, argv, params);

		//Load
		//TODO Wire to concrete instance
		PersitableMatrixCollectionPtr pPersistableMatrixCollection = NullPtr;
		pPersistableMatrixCollection->load(params._searchSpaceSavedLocation);

		//Evaluate
		pSearchSpaceEvaluator->evaluateCollection(pPersistableMatrixCollection);

		//TODO Wire to concrete instance
		MatrixApproximatorPtr pMatrixApproximator = NullPtr;
		pSearchSpaceEvaluator->evaluateApproximator(pMatrixApproximator, pPersistableMatrixCollection);

		break;
	}
	case GENERATE_EVALUATE:
	{
		GenerateEvaluateModeParams params;
		getGenerateEvaluateModeParams(argc, argv, params);

		//Generate
		//TODO Wire to concrete instance
		PersitableMatrixCollectionPtr pUniversalSet = NullPtr;
		pUniversalSet->load(params._universalSetSavedLocation);
		pSearchSpaceConstructor->constructSearchSpace(pMatrixCollection, pUniversalSet, params._maxSequenceLength);

		//Evaluate
		pSearchSpaceEvaluator->evaluateCollection(pMatrixCollection);

		//TODO Wire to concrete instance
		MatrixApproximatorPtr pMatrixApproximator = NullPtr;
		pSearchSpaceEvaluator->evaluateApproximator(pMatrixApproximator, pMatrixCollection);

		break;
	}
	default:
		break;
	}

	return 0;
}

RunMode detectRunMode(int argc, char * argv[]) {
	//TODO Implement
	return UNKNOWN;
}

void getGenerateSaveModeParams(int argc, char* argv[], GenerateSaveModeParams& params) {
	//TODO Implement
}

void getGenerateEvaluateModeParams(int argc, char* argv[], GenerateEvaluateModeParams& params) {
	//TODO Implement
}

void getLoadEvaluateModeParams(int argc, char* argv[], LoadEvaluateModeParams& params) {
	//TODO Implement
}
