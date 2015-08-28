//============================================================================
// Name        : Application.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Gate.h"
#include "ICollection.h"
#include "AlgoCommon.h"
#include "ISearchSpaceEvaluator.h"
#include "EvaluateCommon.h"
#include "SampleAppContainerImpl.h"
#include "IAppContainer.h"

using namespace std;

void evaluateCollection(AppContainerPtr pAppContainer);

void evaluateApproximator(AppContainerPtr pAppContainer);

int main(int argc, char* argv[]) {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	if(argc != 3) {
		cout << "Syntax error. Correct syntax is $GateApproximationApp config_file_name_1 config_file_name_2" << endl;
	}
	else {
		AppContainerPtr pAppContainer = new SampleAppContainerImpl(argv[1], argv[2]);

		evaluateCollection(pAppContainer);
		evaluateApproximator(pAppContainer);

		delete pAppContainer;
	}

	return 0;
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

	//Evaluate collection
	std::cout << "---------------------Start evaluating approximator---------------------" << "\r\n";
	pSearchSpaceEvaluator->evaluateApproximator(pGateApproximator, pGateCoreCollection);
	std::cout << "---------------------End evaluating approximator---------------------" << "\r\n";

	//Recycle instances
	pAppContainer->recycle(pGateApproximator);
	pAppContainer->recycle(pSearchSpaceEvaluator);
}
