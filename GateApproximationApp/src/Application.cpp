//============================================================================
// Name        : Application.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstring>
#include "Gate.h"
#include "ICollection.h"
#include "AlgoCommon.h"
#include "ISearchSpaceEvaluator.h"
#include "EvaluateCommon.h"
#include "SampleAppContainerImpl.h"
#include "IAppContainer.h"

using namespace std;

bool isValidSyntax(int argc, char* argv[]);

void printSyntaxMessage();

void evaluateCollection(AppContainerPtr pAppContainer);

void evaluateApproximator(AppContainerPtr pAppContainer);

int main(int argc, char* argv[]) {
	cout << "!!!Welcome to Quantum Gate Approximation App!!!" << endl; // prints !!!Hello World!!!

	if(!isValidSyntax(argc, argv)) {
		printSyntaxMessage();
	}
	else {
		mreal::initPrecision();

		AppContainerPtr pAppContainer = new SampleAppContainerImpl(argv[2], argv[3]);

		evaluateCollection(pAppContainer);
		evaluateApproximator(pAppContainer);

		delete pAppContainer;
	}

	return 0;
}

bool isValidSyntax(int argc, char* argv[]) {
	const int VALID_ARGUMENTS_NUMBER = 4;
	const char* VALID_ARGUMENT1 = "-G";

	return (argc == VALID_ARGUMENTS_NUMBER) && (strcmp(argv[1], VALID_ARGUMENT1) == 0);
}

void printSyntaxMessage() {
	cout << "Sorry for any inconvenience. Currently the only accepted syntax is $GateApproximationApp -G config_file_name_1 config_file_name_2" << endl << endl;
	cout << "_ -G: To generate and print out sequences close to the identity gate. Other options should be added in the future" << endl << endl;
	cout << "_ config_file_name_1: Path to file contains basic information for approximation like size of gates, library gates, basic sequence length. Please see in1.conf or in2.conf for reference" << endl << endl;
	cout << "_ config_file_name_2: Path to file contains detailed information to configure the NearIdentity Approximation algorithm based on clustering (binning) gate sequences by their coordinate (see NearIdentityElementApproximator.cpp). Please see near_identity_approximator1.conf or near_identity_approximator2.conf for reference" << endl << endl;
}

void evaluateCollection(AppContainerPtr pAppContainer) {
	//Get collection instance, either from file or generate
	GateCollectionPtr pGateCollection = pAppContainer->getGateCollection();

	//Get search space evaluator instance
	GateSearchSpaceEvaluatorPtr pSearchSpaceEvaluator = pAppContainer->getGateSearchSpaceEvaluator();

	//Evaluate collection
	std::cout << "---------------------Start evaluating collection---------------------" << "\r\n";
	pSearchSpaceEvaluator->evaluateCollection(pGateCollection);
	std::cout << "---------------------End evaluating collection---------------------" << "\r\n";

	//Recycle instances
	//pAppContainer->recycle(pGateCollection);
	//pAppContainer->recycle(pSearchSpaceEvaluator);
}

void evaluateApproximator(AppContainerPtr pAppContainer) {
	//Get approximator instance, either from file or generate
	GateApproximatorPtr pGateApproximator = pAppContainer->getGateApproximator();

	//Get collection instance, either from file or generate
	GateCollectionPtr pGateCoreCollection = pAppContainer->getGateCollection();

	//Get search space evaluator instance
	GateSearchSpaceEvaluatorPtr pSearchSpaceEvaluator = pAppContainer->getGateSearchSpaceEvaluator();

	//Evaluate approximator
	std::cout << "---------------------Start evaluating approximator---------------------" << "\r\n";
	pSearchSpaceEvaluator->evaluateApproximator(pGateApproximator, pGateCoreCollection);
	std::cout << "---------------------End evaluating approximator---------------------" << "\r\n";

	//Recycle instances
	pAppContainer->recycle(pGateApproximator);
	pAppContainer->recycle(pSearchSpaceEvaluator);
}
