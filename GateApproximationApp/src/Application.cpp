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

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	AppContainerPtr pAppContainer = new SampleAppContainerImpl("in.conf", "near_identity_approximator.conf");

	evaluateCollection(pAppContainer);
	evaluateApproximator(pAppContainer);

	delete pAppContainer;

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
