/*
 * SelingerComposerEvaluatorImpl.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: pham
 */

#include "SelingerComposerEvaluatorImpl.h"
#include "ScopeTimer.h"
#include "IWriter.h"
#include "Coordinate.hpp"
#include "ICoordinateWriter.h"
#include "ICoordinateCalculator.h"
#include <iostream>

template<typename T>
SelingerComposerEvaluatorImpl<T>::SelingerComposerEvaluatorImpl(const std::vector<std::vector<T> > partialTargets,
		const std::vector<T> modifiers,
		int buildingBlocksBucketMaxSize,
		mreal_t epsilon,
		mreal_t initialEpsilon,
		TimerPtr pTimer,
		DistanceCalculatorPtr<T> pDistanceCalculator,
		CombinerPtr<T> pCombiner,
		RealCoordinateCalculatorPtr<T> pRealCoordinateCalculator,
		RealCoordinateWriterPtr<T> pRealCoordinateWritter,
		WriterPtr<T> pWriter,
		std::ostream& ostream): m_ostream(ostream)  {

	m_partialTargets = partialTargets;
	m_modifiers = modifiers;

	m_buildingBlocksBucketMaxSize = buildingBlocksBucketMaxSize;
	m_epsilon = epsilon;
	m_initialEpsilon = initialEpsilon;

	m_pDistanceCalculator = pDistanceCalculator;
	m_pCombiner = pCombiner;
	m_pRealCoordinateCalculator = pRealCoordinateCalculator;
	m_pRealCoordinateWriter = pRealCoordinateWritter;
	m_pWriter = pWriter;
	m_pTimer = pTimer;
}

template<typename T>
void SelingerComposerEvaluatorImpl<T>::evaluateComposers(ComposerPtr<T> pEvaluatedComposer,
		ComposerPtr<T> pStandardComposer) {

	for(unsigned int i = 0; i < m_partialTargets.size(); i++) {
		std::vector<T>& partialTargets = m_partialTargets[i];

		T target;
		T modifier = m_modifiers[i];

		generateTarget(partialTargets, modifier, target);
		m_ostream << "-------------Composing target--------------\n";
		logTarget(target);

		BuildingBlockBuckets<T> buildingBlockBuckets;
		generateBuildingBlockBuckets(partialTargets, modifier, buildingBlockBuckets);

		evaluateBestEffortPartialResults(buildingBlockBuckets, modifier, target);
		evaluateComposerForTarget(pEvaluatedComposer, pStandardComposer, buildingBlockBuckets, target);

		releaseBuildingBlocksBuckets(buildingBlockBuckets);
		_destroy(target);
	}
}

//MARK: Protected methods, can be used by sub class
template<typename T>
void SelingerComposerEvaluatorImpl<T>::applyModifier(T element, T modifier, T& result) {
	T me; //Modifier * Element
	m_pCombiner->combine(modifier, element, me);
	m_pCombiner->combine(me, modifier, result); //Result = Modifier * Element * Modifier
	_destroy(me);
}

//MARK: Private methods
template<typename T>
void SelingerComposerEvaluatorImpl<T>::generateTarget(const std::vector<T>& partials,
		T modifier,
		T& target) {

	std::vector<T> partialElements;
	size_t nbPartials = partials.size();

	auto toApplyModifierAt = [&modifier](int i) {
		//This is a temporary fix !
		return modifier != NullPtr && i == 1;
	};

	//Apply modifier to only the second paritial !
	for(unsigned int i = 0; i < nbPartials; i++) {
		T partialElement;
		if(toApplyModifierAt(i)) {
			applyModifier(partials[i], modifier, partialElement);
		}
		else {
			partialElement = partials[i];
		}
		partialElements.push_back(partialElement);
	}

	//Combine paritial elements (including modified ones)
	T combined = partialElements.empty() ? NullPtr : partialElements[0]->clone();
	for(unsigned int i = 1; i < nbPartials && combined != NullPtr; i++) {
		if(partialElements[i] != NullPtr) {
			T tmp = NullPtr;
			m_pCombiner->combine(combined, partialElements[i], tmp);
			_destroy(combined);
			combined = tmp;
		}
	}

	//Destroy generated partial elements
	for(unsigned int i = 0; i < nbPartials; i++) {
		T partialElement = partialElements[i];
		if(toApplyModifierAt(i)) {
			_destroy(partialElement);
			partialElements[i] = NullPtr;
		}
	}

	target = combined;
}

template<typename T>
void SelingerComposerEvaluatorImpl<T>::logTarget(T target) {
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
void SelingerComposerEvaluatorImpl<T>::evaluateBestEffortPartialResults(BuildingBlockBuckets<T>& buildingBlockBuckets,
		T modifier,
		T target) {

	std::vector<T> partials;

	m_ostream << "---------------------------------------------\n";
	for(IteratorPtr<T> pIter : buildingBlockBuckets) {
		//Always get only first element
		T bestApprxElement = pIter->getObj();
		m_ostream << "Best partial approximation:\n";
		m_pWriter->write(bestApprxElement, m_ostream);
		partials.push_back(bestApprxElement);
	}

	T candidate;
	generateTarget(partials, NullPtr, candidate);
	m_ostream << "---------------------------------------------\n";
	m_ostream << "Best approximation:\n";
	m_pWriter->write(candidate, m_ostream);

	mreal_t err = m_pDistanceCalculator->distance(target, candidate);
	m_ostream << "Best effort err = " << err << "\n";

	_destroy(candidate);
}

template<typename T>
void SelingerComposerEvaluatorImpl<T>::evaluateComposerForTarget(ComposerPtr<T> pEvaluateComposer,
		ComposerPtr<T> pStandardComposer,
		BuildingBlockBuckets<T>& buildingBlockBuckets,
		T target) {

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
}

template<typename T>
void SelingerComposerEvaluatorImpl<T>::evaluateComposer(BuildingBlockBuckets<T>& buildingBlockBuckets,
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
				m_pDistanceCalculator,
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
void SelingerComposerEvaluatorImpl<T>::processFoundResults(IteratorPtr<LookupResult<T> >  pFindIter,
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
void SelingerComposerEvaluatorImpl<T>::logSearchResult(LookupResult<T> result, double searchTime) {

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
void SelingerComposerEvaluatorImpl<T>::releaseBuildingBlocksBuckets(BuildingBlockBuckets<T>& buildingBlockBuckets) {
	for(typename BuildingBlockBuckets<T>::iterator bIter = buildingBlockBuckets.begin(); bIter != buildingBlockBuckets.end();) {
		IteratorPtr<T> pIter = *bIter;
		bIter = buildingBlockBuckets.erase(bIter);
		releaseIter(pIter);
		_destroy(pIter);
	}
}

template<typename T>
void SelingerComposerEvaluatorImpl<T>::releaseResultIter(IteratorPtr<LookupResult<T> >& pFindResultIter) {
	while(pFindResultIter != NullPtr && !pFindResultIter->isDone()) {
		T element = pFindResultIter->getObj().m_resultElement;
		pFindResultIter->next();
		_destroy(element);
	}
	_destroy(pFindResultIter);
	pFindResultIter = NullPtr;
}

template<typename T>
void SelingerComposerEvaluatorImpl<T>::releaseIter(IteratorPtr<T>& pFindIter) {
	while(pFindIter != NullPtr && !pFindIter->isDone()) {
		T element = pFindIter->getObj();
		pFindIter->next();
		_destroy(element);
	}
}

template<typename T>
void SelingerComposerEvaluatorImpl<T>::ComposerEvaluateFactors::printInfo(std::ostream& ostream, std::string composerName) {
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
void SelingerComposerEvaluatorImpl<T>::ComposerEvaluateFactors::printCompareToInfo(std::ostream& ostream, const ComposerEvaluateFactors& evalFactors2) {
	ostream << "Run time = " << m_composeTime / evalFactors2.m_composeTime << "\n";
	if(evalFactors2.m_nbResults != 0) {
		ostream << "Recall = " << (float)m_nbResults / evalFactors2.m_nbResults << "\n";
		mreal_t noiseThresold = 1e-9;
		if(m_bestDistance > evalFactors2.m_bestDistance + noiseThresold) {
			ostream << "Couldn not reached best result\n";
		}
		else {
			ostream << "Reached best result\n";
		}
	}
}
