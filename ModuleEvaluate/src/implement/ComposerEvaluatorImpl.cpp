/*
 * ComposerEvaluatorImpl.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: pham
 */

#include "ComposerEvaluatorImpl.h"
#include "ScopeTimer.h"
#include "IWriter.h"
#include "Coordinate.hpp"
#include "ICoordinateWriter.h"
#include "ICoordinateCalculator.h"
#include <iostream>

template<typename T>
ComposerEvaluatorImpl<T>::ComposerEvaluatorImpl(const std::vector<T> targets,
		int nbPartialElements,
		int buildingBlocksBucketMaxSize,
		CollectionPtr<T> pCollection,
		CollectionPtr<T> pHelperCollection,
		DecomposerPtr<T> pDecomposer,
		mreal_t epsilon,
		mreal_t initialEpsilon,
		TimerPtr pTimer,
		RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
		RealCoordinateWriterPtr<T> pRealCoordinateWritter,
		WriterPtr<T> pWriter,
		std::ostream& ostream) : m_ostream(ostream) {

	m_targets = targets;
	m_nbPartialElements = nbPartialElements;
	m_buildingBlocksBucketMaxSize = buildingBlocksBucketMaxSize;
	m_epsilon = epsilon;
	m_initialEpsilon = initialEpsilon;
	m_pCollection = pCollection;
	m_pHelperCollection = pHelperCollection;
	m_pDecomposer = pDecomposer;

	m_pRealCoordinateCalculator = pRealCoordinateCalculator;
	m_pRealCoordinateWriter = pRealCoordinateWritter;
	m_pWriter = pWriter;
	m_pTimer = pTimer;
}

template<typename T>
void ComposerEvaluatorImpl<T>::evaluateComposers(ComposerPtr<T> pEvaluatedComposer,
		ComposerPtr<T> pStandardComposer) {
	for(T target: m_targets) {
		m_ostream << "-------------Composing target--------------\n";
		logTarget(target);
		evaluateComposerForTarget(pEvaluatedComposer, pStandardComposer, target);
	}
}

template<typename T>
void ComposerEvaluatorImpl<T>::ComposerEvaluateFactors::printInfo(std::ostream& ostream, std::string composerName) {
	ostream << composerName
			<< " took "
			<< m_composeTime
			<< " ms to retrieve "
			<< m_nbResults
			<< " results with best distance "
			<< m_bestDistance
			<<  "\n";
}

template<typename T>
void ComposerEvaluatorImpl<T>::ComposerEvaluateFactors::printCompareToInfo(std::ostream& ostream, const ComposerEvaluateFactors& evalFactors2) {
	if(evalFactors2.m_nbResults != 0) {
		ostream << "Recall = " << (float)m_nbResults / evalFactors2.m_nbResults << "\n";
		if(m_bestDistance > evalFactors2.m_bestDistance) {
			ostream << "Couldn not reached best result\n";
		}
		else {
			ostream << "Reached best result\n";
		}
	}
}

template<typename T>
void ComposerEvaluatorImpl<T>::logTarget(T target) {
	const std::string delimeter = ", ";
	const std::string ENDLINE = "\n";

	//Temporally output nothing but the target 's coordinate

	//Calculate result coordinate
	CoordinatePtr<T, mreal_t> pCoordinate = NullPtr;
	if(m_pRealCoordinateCalculator != NullPtr) {
		m_pRealCoordinateCalculator->calulateElementCoordinate(target,
				pCoordinate);
	}

	m_ostream <<  "--Target coord:"<< ENDLINE;
	if(target != NullPtr) {
		m_pWriter->write(target, m_ostream);
		if(pCoordinate != NullPtr) {
			m_pRealCoordinateWriter->writeCoordinate(*pCoordinate, m_ostream);
		}
	}
	m_ostream << ENDLINE;

	//Release pointer to coordinate
	_destroy(pCoordinate);
}

template<typename T>
void ComposerEvaluatorImpl<T>::evaluateComposerForTarget(ComposerPtr<T> pEvaluateComposer,
		ComposerPtr<T> pStandardComposer,
		T target) {

	BuildingBlockBuckets<T> buildingBlockBuckets;
	decomposeQueryIntoBuildingBlocksBuckets(target, buildingBlockBuckets);

	m_ostream << "Starting on evaluated composer\n";
	ComposerEvaluateFactors evalFactors1;
	evaluateComposer(buildingBlockBuckets,
			pEvaluateComposer,
			target,
			&evalFactors1);
	evalFactors1.printInfo(m_ostream, "Evaluated composer");

	m_ostream << "Starting on brute-force composer\n";
	ComposerEvaluateFactors evalFactors2;
	evaluateComposer(buildingBlockBuckets,
			pStandardComposer,
			target,
			&evalFactors2);
	evalFactors2.printInfo(m_ostream, "Standard composer");

	evalFactors1.printCompareToInfo(m_ostream, evalFactors2);

	releaseBuildingBlocksBuckets(buildingBlockBuckets);
}

template<typename T>
void ComposerEvaluatorImpl<T>::decomposeQueryIntoBuildingBlocksBuckets(T target,
		BuildingBlockBuckets<T>& buildingBlockBuckets) {
	buildingBlockBuckets.clear();

	std::vector<T> partialQueries;
	m_pDecomposer->decomposeElement(target,
			partialQueries,
			m_nbPartialElements);

	combination_counter_t maximumNbCombinations = 1;
	for(unsigned int i = 0; i < partialQueries.size(); i++) {
		T partialTarget = partialQueries[i];

		//Get buiding block list for partial query
		IteratorPtr<LookupResult<T> > pLookupIter = m_pCollection->findNearestNeighbours(partialTarget,
				m_initialEpsilon);

		//Add found building blocks to the bucket to compose later
		buildingBlockBuckets.push_back(getExtractedElementIterator(pLookupIter,
				&maximumNbCombinations));

		_destroy(pLookupIter);

		partialQueries[i] = NullPtr;
		_destroy(partialTarget);
	}
	m_ostream << "Maximum number of combinations " << maximumNbCombinations << "\n";
}

template<typename T>
IteratorPtr<T> ComposerEvaluatorImpl<T>::getExtractedElementIterator(IteratorPtr<LookupResult<T> > pLookupResultIter,
		combination_counter_t* pCombinationCounter) {

	m_pHelperCollection->clear();
	int count = 0;
	while(!pLookupResultIter->isDone() && (m_buildingBlocksBucketMaxSize == 0 || count < m_buildingBlocksBucketMaxSize)) {
		m_pHelperCollection->addElement(pLookupResultIter->getObj().m_resultElement);
		count++;
		pLookupResultIter->next();
	}

	//Destroy un-used found results
	while(!pLookupResultIter->isDone()) {
		LookupResult<T> result = pLookupResultIter->getObj();
		_destroy(result.m_resultElement);
		result.m_resultElement = NullPtr;
		pLookupResultIter->next();
	}

	//Side-effect: Count maximum possible number of combinations
	m_ostream << "Size of subset: " << count << "\n";
	if(pCombinationCounter != NULL) {
		*pCombinationCounter *= count;
	}

	IteratorPtr<T> pElementIter = m_pHelperCollection->getReadonlyIteratorPtr();
	m_pHelperCollection->clear();
	return pElementIter;
}

template<typename T>
void ComposerEvaluatorImpl<T>::evaluateComposer(BuildingBlockBuckets<T>& buildingBlockBuckets,
		ComposerPtr<T> pComposer,
		T target,
		ComposerEvaluateFactors* pComposerEvaluateFactors) {

	//Use try catch and make use of scope timer
	IteratorPtr<LookupResult<T> >  pFindIter(NullPtr);
	try {
		ScopeTimer scopeTimer(m_pTimer, &(pComposerEvaluateFactors->m_composeTime));

		//Start the compose routine for the target
		pFindIter = pComposer->composeApproximations(buildingBlockBuckets,
				target,
				m_pCollection->getDistanceCalculator(),
				m_epsilon);
	}
	catch (std::exception & e) {
	}

	//Reset building blocks for future use
	for(IteratorPtr<T> pBuildingBlockIter : buildingBlockBuckets) {
		pBuildingBlockIter->toBegin();
	}

	processFoundResults(pFindIter, pComposerEvaluateFactors);

	releaseResultIter(pFindIter);
}

template<typename T>
void ComposerEvaluatorImpl<T>::processFoundResults(IteratorPtr<LookupResult<T> >  pFindIter,
		ComposerEvaluateFactors* pComposerEvaluateFactor) {

	//Retrieve info to inout params
	if(pFindIter == NullPtr || pFindIter->isDone()) {
		pComposerEvaluateFactor->m_bestDistance = -1.0;
		pComposerEvaluateFactor->m_nbResults = 0;
		return;
	}
	pFindIter->toBegin();
	pComposerEvaluateFactor->m_bestDistance = pFindIter->getObj().m_distanceToTarget;

	//Print results if necessary
	double searchTime = pComposerEvaluateFactor->m_composeTime;
	pComposerEvaluateFactor->m_nbResults = 0;
	while(!pFindIter->isDone()) {
		//Only log first result
		if(pComposerEvaluateFactor->m_nbResults == 0) {
			logSearchResult(pFindIter->getObj(), searchTime);
		}
		pFindIter->next();
		pComposerEvaluateFactor->m_nbResults++;
	}

	//Rewind iterator for furture purpose
	pFindIter->toBegin();
}

template<typename T>
void ComposerEvaluatorImpl<T>::logSearchResult(LookupResult<T> result, double searchTime) {

	const std::string delimeter = ", ";
	const std::string ENDLINE = "\n";

	//Temporally output nothing but the result 's coordinate and approximation precision

	//Calculate result coordinate
	CoordinatePtr<T, mreal_t> pCoordinate = NullPtr;
	if(m_pRealCoordinateCalculator != NullPtr) {
		m_pRealCoordinateCalculator->calulateElementCoordinate(result.m_resultElement,
				pCoordinate);
	}

	m_ostream << "Search time: "  << searchTime << delimeter;

	if(result.m_resultElement != NullPtr) {
		m_pWriter->write(result.m_resultElement, m_ostream);
		if(pCoordinate != NullPtr) {
			m_pRealCoordinateWriter->writeCoordinate(*pCoordinate, m_ostream);
		}
	}

	m_ostream << "Distance to target:";
	m_ostream << result.m_distanceToTarget;
	m_ostream << ENDLINE;

	//Release pointer to coordinate
	_destroy(pCoordinate);

	/*outputStream << "Query:" << END_LINE;
	pWriter->write(pQuery, outputStream);
	outputStream << "Epsilon:" << epsilon << END_LINE;

	outputStream << "Info" << END_LINE;
	outputStream << "Search time: " ;
	outputStream << searchTime << delimeter;
	outputStream << "Result:" << END_LINE;*/
}

template<typename T>
void ComposerEvaluatorImpl<T>::releaseBuildingBlocksBuckets(BuildingBlockBuckets<T>& buildingBlockBuckets) {
	for(typename BuildingBlockBuckets<T>::iterator bIter = buildingBlockBuckets.begin(); bIter != buildingBlockBuckets.end();) {
		IteratorPtr<T> pIter = *bIter;
		bIter = buildingBlockBuckets.erase(bIter);
		releaseIter(pIter);
		_destroy(pIter);
	}
}

template<typename T>
void ComposerEvaluatorImpl<T>::releaseResultIter(IteratorPtr<LookupResult<T> >& pFindResultIter) {
	while(pFindResultIter != NullPtr && !pFindResultIter->isDone()) {
		T element = pFindResultIter->getObj().m_resultElement;
		pFindResultIter->next();
		_destroy(element);
	}
	_destroy(pFindResultIter);
	pFindResultIter = NullPtr;
}

template<typename T>
void ComposerEvaluatorImpl<T>::releaseIter(IteratorPtr<T>& pFindIter) {
	while(pFindIter != NullPtr && !pFindIter->isDone()) {
		T element = pFindIter->getObj();
		pFindIter->next();
		_destroy(element);
	}
}

