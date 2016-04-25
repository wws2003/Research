/*
 * SingleQubitGateCombinabilityCheckerFactoryImpl.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: pham
 */

#include "SingleQubitGateCombinabilityCheckerFactoryImpl.h"
#include "GateIdentityCycleCombinabilityCheckerImpl.h"
#include "GateCancelationCombinabilityCheckerImpl.h"
#include "GateSpecification.h"
#include <exception>

SingleQubitGateCombinabilityCheckerFactoryImpl::SingleQubitGateCombinabilityCheckerFactoryImpl() {
	initHTGateCombinabilityCheckers();
	initHTSGateCombinabilityCheckers();
}

SingleQubitGateCombinabilityCheckerFactoryImpl::~SingleQubitGateCombinabilityCheckerFactoryImpl() {
	releaseCheckers(m_htGateCombinabilityCheckers);
	releaseCheckers(m_htsGateCombinabilityCheckers);
}

void SingleQubitGateCombinabilityCheckerFactoryImpl::getGateCombinabilityCheckers(LibrarySet librarySet, GateCombinabilityCheckers& checkers) {
	switch (librarySet) {
	case L_HT:
		checkers = m_htGateCombinabilityCheckers;
		break;
	case L_HTS:
		checkers = m_htsGateCombinabilityCheckers;
		break;
	default:
		throw std::logic_error("Library set not supported");
		break;
	}
}

void SingleQubitGateCombinabilityCheckerFactoryImpl::initHTGateCombinabilityCheckers() {
	GateCombinabilityCheckerPtr pChecker1 = getGateIdentityCycleCombinabilityChecker(L_HT);
	GateCombinabilityCheckerPtr pChecker2 = getGateCancelationCombinabilityChecker(L_HT);

	m_htGateCombinabilityCheckers.push_back(pChecker1);
	m_htGateCombinabilityCheckers.push_back(pChecker2);
}

void SingleQubitGateCombinabilityCheckerFactoryImpl::initHTSGateCombinabilityCheckers() {
	GateCombinabilityCheckerPtr pChecker1 = getGateIdentityCycleCombinabilityChecker(L_HTS);
	GateCombinabilityCheckerPtr pChecker2 = getGateCancelationCombinabilityChecker(L_HTS);

	m_htsGateCombinabilityCheckers.push_back(pChecker1);
	m_htsGateCombinabilityCheckers.push_back(pChecker2);
}

void SingleQubitGateCombinabilityCheckerFactoryImpl::releaseCheckers(GateCombinabilityCheckers& checkers) {
	for(GateCombinabilityCheckers::iterator cIter = checkers.begin(); cIter != checkers.end(); ) {
		_destroy(*cIter);
		cIter = checkers.erase(cIter);
	}
}

GateCombinabilityCheckerPtr SingleQubitGateCombinabilityCheckerFactoryImpl::getGateIdentityCycleCombinabilityChecker(LibrarySet librarySet) {
	using namespace gatespec::sgq;
	using namespace gatespec::val;

	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair(T::name, 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair(H::name, 2));

	if(librarySet == L_HTS) {
		identityCycleMap.insert(GateLabelIdentityCyclePair(S::name, 4));
	}

	return GateCombinabilityCheckerPtr(new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap));
}

GateCombinabilityCheckerPtr SingleQubitGateCombinabilityCheckerFactoryImpl::getGateCancelationCombinabilityChecker(LibrarySet librarySet) {
	using namespace gatespec::sgq;
	using namespace gatespec::val;

	GateLabelCancelationMap cancelationMap;

	const std::string T(T::name);

	if(librarySet == L_HTS) {
		const std::string S(S::name);

		//Cancel T*T = S
		cancelationMap.insert(GateLabelPair(T, T));

		//Cancel T*S*S*S*T = I
		cancelationMap.insert(GateLabelPair(T, T+S+S+S));

		//Cancel S*T*S*T*S = I
		cancelationMap.insert(GateLabelPair(S, S+T+S+T));
	}

	return GateCombinabilityCheckerPtr(new GateCancelationCombinabilityCheckerImpl(cancelationMap));
}


