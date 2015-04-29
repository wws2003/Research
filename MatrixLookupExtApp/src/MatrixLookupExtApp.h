/*
 * MatrixLookupExtApp.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef MATRIXLOOKUPEXTAPP_H_
#define MATRIXLOOKUPEXTAPP_H_

#include <string>

enum RunMode {
	GENERATE_SAVE,
	GENERATE_EVALUATE,
	LOAD_EVALUATE,
	UNKNOWN,
};

RunMode detectRunMode(int argc, char * argv[]);

typedef struct GenerateSaveModeParams {
	std::string _universalSetSavedLocation;
	int _maxSequenceLength;
	std::string _searchSpaceSavedLocation;
} GenerateSaveModeParams;

typedef struct GenerateEvaluateModeParams {
	std::string _universalSetSavedLocation;
	int _maxSequenceLength;
} GenerateEvaluateModeParams;

typedef struct LoadEvaluateModeParams {
	std::string _universalSetSavedLocation;
	std::string _searchSpaceSavedLocation;
} LoadEvaluateModeParams;

void getGenerateSaveModeParams(int argc, char* argv[], GenerateSaveModeParams& params);
void getGenerateEvaluateModeParams(int argc, char* argv[], GenerateEvaluateModeParams& params);
void getLoadEvaluateModeParams(int argc, char* argv[], LoadEvaluateModeParams& params);

#endif /* MATRIXLOOKUPEXTAPP_H_ */
