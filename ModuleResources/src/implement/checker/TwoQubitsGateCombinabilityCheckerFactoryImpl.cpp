/*
 * TwoQubitGateCombinabilityCheckerFactoryImpl.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: pham
 */

#include "TwoQubitsGateCombinabilityCheckerFactoryImpl.h"
#include "GateIdentityCycleCombinabilityCheckerImpl.h"
#include "GateCancelationCombinabilityCheckerImpl.h"
#include "GateSpecification.h"
#include "GateSelectiveCombinabilityCheckerImpl.h"
#include <exception>

TwoQubitsGateCombinabilityCheckerFactoryImpl::TwoQubitsGateCombinabilityCheckerFactoryImpl() {
	initHTCNOTGateCombinabilityCheckers();
	initHTSCNOTGateCombinabilityCheckers();
	initHTtSCNOTGateCombinabilityCheckers();
}

TwoQubitsGateCombinabilityCheckerFactoryImpl::~TwoQubitsGateCombinabilityCheckerFactoryImpl() {
	releaseCheckers(m_hTtCNOTGateCombinabilityCheckers);
	releaseCheckers(m_htsCNOTGateCombinabilityCheckers);
	releaseCheckers(m_htCNOTGateCombinabilityCheckers);
}

void TwoQubitsGateCombinabilityCheckerFactoryImpl::getGateCombinabilityCheckers(LibrarySet librarySet, GateCombinabilityCheckers& checkers) {
	switch (librarySet) {
	case L_HTCNOT:
		checkers = m_htCNOTGateCombinabilityCheckers;
		break;
	case L_HTtCNOT:
		checkers = m_hTtCNOTGateCombinabilityCheckers;
		break;
	case L_HTSCNOT:
		checkers = m_htsCNOTGateCombinabilityCheckers;
		break;
	default:
		throw std::logic_error("Library set not supported");
		break;
	}
}

void TwoQubitsGateCombinabilityCheckerFactoryImpl::initHTCNOTGateCombinabilityCheckers() {
	GateCombinabilityCheckerPtr pChecker1 = getGateIdentityCycleCombinabilityChecker(L_HTCNOT);
	GateCombinabilityCheckerPtr pChecker2 = getGateCancelationCombinabilityChecker(L_HTCNOT);
	GateCombinabilityCheckerPtr pChecker3 = getHTCNOTGateSelectiveCombinabilityChecker();

	m_htCNOTGateCombinabilityCheckers.push_back(pChecker1);
	m_htCNOTGateCombinabilityCheckers.push_back(pChecker2);
	m_htCNOTGateCombinabilityCheckers.push_back(pChecker3);
}

void TwoQubitsGateCombinabilityCheckerFactoryImpl::initHTSCNOTGateCombinabilityCheckers() {
	GateCombinabilityCheckerPtr pChecker1 = getGateIdentityCycleCombinabilityChecker(L_HTSCNOT);
	GateCombinabilityCheckerPtr pChecker2 = getGateCancelationCombinabilityChecker(L_HTSCNOT);
	GateCombinabilityCheckerPtr pChecker3 = getHTSCNOTGateSelectiveCombinabilityChecker();

	m_htsCNOTGateCombinabilityCheckers.push_back(pChecker1);
	m_htsCNOTGateCombinabilityCheckers.push_back(pChecker2);
	m_htsCNOTGateCombinabilityCheckers.push_back(pChecker3);
}

void TwoQubitsGateCombinabilityCheckerFactoryImpl::initHTtSCNOTGateCombinabilityCheckers() {
	GateCombinabilityCheckerPtr pChecker1 = getHTtCNOTGateIdentityCycleCombinabilityChecker();
	GateCombinabilityCheckerPtr pChecker2 = getHTtCNOTGateCancelationCombinabilityChecker();
	GateCombinabilityCheckerPtr pChecker3 = getHTtSNOTGateSelectiveCombinabilityChecker();

	m_hTtCNOTGateCombinabilityCheckers.push_back(pChecker1);
	m_hTtCNOTGateCombinabilityCheckers.push_back(pChecker2);
	m_hTtCNOTGateCombinabilityCheckers.push_back(pChecker3);
}

void TwoQubitsGateCombinabilityCheckerFactoryImpl::releaseCheckers(GateCombinabilityCheckers& checkers) {
	for(GateCombinabilityCheckers::iterator cIter = checkers.begin(); cIter != checkers.end(); ) {
		_destroy(*cIter);
		cIter = checkers.erase(cIter);
	}
}

GateCombinabilityCheckerPtr TwoQubitsGateCombinabilityCheckerFactoryImpl::getGateIdentityCycleCombinabilityChecker(LibrarySet librarySet) {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair(T1::name, 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair(H1::name, 2));
	identityCycleMap.insert(GateLabelIdentityCyclePair(T2::name, 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair(H2::name, 2));
	identityCycleMap.insert(GateLabelIdentityCyclePair(CNOT1::name, 2));
	identityCycleMap.insert(GateLabelIdentityCyclePair(CNOT2::name, 2));

	if(librarySet == L_HTSCNOT) {
		identityCycleMap.insert(GateLabelIdentityCyclePair(S1::name, 4));
		identityCycleMap.insert(GateLabelIdentityCyclePair(S2::name, 4));
	}

	return GateCombinabilityCheckerPtr(new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap));
}

GateCombinabilityCheckerPtr TwoQubitsGateCombinabilityCheckerFactoryImpl::getGateCancelationCombinabilityChecker(LibrarySet librarySet) {
	//Possibly there are many other combinations should be cancelled
	using namespace gatespec::twq;
	using namespace gatespec::val;

	GateLabelCancelationMap cancelationMap;

	const std::string T1(T1::name);
	const std::string T2(T2::name);
	const std::string H1(H1::name);
	const std::string H2(H2::name);
	const std::string CNOT1(CNOT1::name);
	const std::string CNOT2(CNOT2::name);

	cancelationMap.insert(GateLabelPair(CNOT2, CNOT1 + CNOT2 + CNOT1));
	cancelationMap.insert(GateLabelPair(CNOT1, CNOT2 + CNOT1 + CNOT2));
	cancelationMap.insert(GateLabelPair(T2, CNOT1 + CNOT2 + T2 + CNOT1 + CNOT2));
	cancelationMap.insert(GateLabelPair(CNOT1, CNOT2 + H1 + CNOT1 + CNOT2 + H1));
	cancelationMap.insert(GateLabelPair(H1, CNOT1 + CNOT2 + H1 + CNOT1 + CNOT2));
	cancelationMap.insert(GateLabelPair(CNOT1, CNOT2 + H1 + CNOT2 + CNOT1 + H1));

	if(librarySet == L_HTSCNOT) {
		const std::string S1(S1::name);
		const std::string S2(S2::name);

		//Cancel T*T = S
		cancelationMap.insert(GateLabelPair(T1, T1));
		cancelationMap.insert(GateLabelPair(T2, T2));

		//Cancel T*S*S*S*T = I
		cancelationMap.insert(GateLabelPair(T1, T1+S1+S1+S1));
		cancelationMap.insert(GateLabelPair(T2, T2+S2+S2+S2));

		//Cancel S*T*S*T*S = I
		cancelationMap.insert(GateLabelPair(S1, S1+T1+S1+T1));
		cancelationMap.insert(GateLabelPair(S2, S2+T2+S2+T2));
	}

	return GateCombinabilityCheckerPtr(new GateCancelationCombinabilityCheckerImpl(cancelationMap));
}

GateCombinabilityCheckerPtr TwoQubitsGateCombinabilityCheckerFactoryImpl::getHTCNOTGateSelectiveCombinabilityChecker() {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	const std::string T1(T1::name);
	const std::string T2(T2::name);
	const std::string H1(H1::name);
	const std::string H2(H2::name);
	const std::string CNOT1(CNOT1::name);
	const std::string CNOT2(CNOT2::name);

	CombinableGateLabelMap combinableLabelMap;
	combinableLabelMap.insert(CombinableGateLabelPair(T1,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(H1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(T2,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(H2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,CNOT1));

	return GateCombinabilityCheckerPtr(new GateSelectiveCombinabilityCheckerImpl(combinableLabelMap));
}

GateCombinabilityCheckerPtr TwoQubitsGateCombinabilityCheckerFactoryImpl::getHTSCNOTGateSelectiveCombinabilityChecker() {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	const std::string T1(T1::name);
	const std::string T2(T2::name);
	const std::string H1(H1::name);
	const std::string H2(H2::name);
	const std::string CNOT1(CNOT1::name);
	const std::string CNOT2(CNOT2::name);
	const std::string S1(S1::name);
	const std::string S2(S2::name);

	CombinableGateLabelMap combinableLabelMap;

	combinableLabelMap.insert(CombinableGateLabelPair(T1,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,S1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,S2));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(H1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,CNOT2));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,S1));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,S2));

	combinableLabelMap.insert(CombinableGateLabelPair(T2,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,S1));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,S2));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(H2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,CNOT2));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,S1));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,S2));

	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,S1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,S2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,S1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,S2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,CNOT1));

	combinableLabelMap.insert(CombinableGateLabelPair(S1,S1));
	combinableLabelMap.insert(CombinableGateLabelPair(S1,S2));
	combinableLabelMap.insert(CombinableGateLabelPair(S1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(S1,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(S1,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(S1,CNOT2));

	combinableLabelMap.insert(CombinableGateLabelPair(S2,S2));
	combinableLabelMap.insert(CombinableGateLabelPair(S2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(S2,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(S2,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(S2,CNOT2));

	return GateCombinabilityCheckerPtr(new GateSelectiveCombinabilityCheckerImpl(combinableLabelMap));
}

GateCombinabilityCheckerPtr TwoQubitsGateCombinabilityCheckerFactoryImpl::getHTtCNOTGateIdentityCycleCombinabilityChecker() {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	GateLabelIdentityCycleMap identityCycleMap;
	identityCycleMap.insert(GateLabelIdentityCyclePair(T1::name, 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair(TDagger1::name, 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair(H1::name, 2));
	identityCycleMap.insert(GateLabelIdentityCyclePair(T2::name, 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair(TDagger2::name, 8));
	identityCycleMap.insert(GateLabelIdentityCyclePair(H2::name, 2));
	identityCycleMap.insert(GateLabelIdentityCyclePair(CNOT1::name, 2));
	identityCycleMap.insert(GateLabelIdentityCyclePair(CNOT2::name, 2));

	return GateCombinabilityCheckerPtr(new GateIdentityCycleCombinabilityCheckerImpl(identityCycleMap));
}

GateCombinabilityCheckerPtr TwoQubitsGateCombinabilityCheckerFactoryImpl::getHTtCNOTGateCancelationCombinabilityChecker() {
	//Possibly there are many other combinations should be cancelled
	using namespace gatespec::twq;
	using namespace gatespec::val;

	GateLabelCancelationMap cancelationMap;

	const std::string T1(T1::name);
	const std::string TDagger1(TDagger1::name);
	const std::string T2(T2::name);
	const std::string TDagger2(TDagger2::name);
	const std::string H1(H1::name);
	const std::string H2(H2::name);
	const std::string CNOT1(CNOT1::name);
	const std::string CNOT2(CNOT2::name);

	cancelationMap.insert(GateLabelPair(TDagger1, T1));
	cancelationMap.insert(GateLabelPair(TDagger2, T2));
	cancelationMap.insert(GateLabelPair(CNOT2, CNOT1 + CNOT2 + CNOT1));
	cancelationMap.insert(GateLabelPair(CNOT1, CNOT2 + CNOT1 + CNOT2));
	cancelationMap.insert(GateLabelPair(T2, CNOT1 + CNOT2 + T2 + CNOT1 + CNOT2));
	cancelationMap.insert(GateLabelPair(TDagger2, CNOT1 + CNOT2 + TDagger2 + CNOT1 + CNOT2));
	cancelationMap.insert(GateLabelPair(CNOT1, CNOT2 + H1 + CNOT1 + CNOT2 + H1));
	cancelationMap.insert(GateLabelPair(H1, CNOT1 + CNOT2 + H1 + CNOT1 + CNOT2));
	cancelationMap.insert(GateLabelPair(CNOT1, CNOT2 + H1 + CNOT2 + CNOT1 + H1));

	return GateCombinabilityCheckerPtr(new GateCancelationCombinabilityCheckerImpl(cancelationMap));
}

GateCombinabilityCheckerPtr TwoQubitsGateCombinabilityCheckerFactoryImpl::getHTtSNOTGateSelectiveCombinabilityChecker() {
	using namespace gatespec::twq;
	using namespace gatespec::val;

	const std::string T1(T1::name);
	const std::string TDagger1(TDagger1::name);
	const std::string T2(T2::name);
	const std::string TDagger2(TDagger2::name);
	const std::string H1(H1::name);
	const std::string H2(H2::name);
	const std::string CNOT1(CNOT1::name);
	const std::string CNOT2(CNOT2::name);

	CombinableGateLabelMap combinableLabelMap;
	combinableLabelMap.insert(CombinableGateLabelPair(T1,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(T1,CNOT2));
	combinableLabelMap.insert(CombinableGateLabelPair(T1, TDagger2));

	combinableLabelMap.insert(CombinableGateLabelPair(H1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,CNOT2));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,TDagger1));
	combinableLabelMap.insert(CombinableGateLabelPair(H1,TDagger2));

	combinableLabelMap.insert(CombinableGateLabelPair(T2,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,CNOT2));
	combinableLabelMap.insert(CombinableGateLabelPair(T2,TDagger1));

	combinableLabelMap.insert(CombinableGateLabelPair(H2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,CNOT2));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,TDagger1));
	combinableLabelMap.insert(CombinableGateLabelPair(H2,TDagger2));

	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,CNOT2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,TDagger1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT1,TDagger2));

	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,T1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,T2));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,TDagger1));
	combinableLabelMap.insert(CombinableGateLabelPair(CNOT2,TDagger2));

	combinableLabelMap.insert(CombinableGateLabelPair(TDagger1,H1));
	combinableLabelMap.insert(CombinableGateLabelPair(TDagger1,CNOT1));
	combinableLabelMap.insert(CombinableGateLabelPair(TDagger1,TDagger1));
	combinableLabelMap.insert(CombinableGateLabelPair(TDagger1,TDagger2));

	combinableLabelMap.insert(CombinableGateLabelPair(TDagger2,H2));
	combinableLabelMap.insert(CombinableGateLabelPair(TDagger2,CNOT2));
	combinableLabelMap.insert(CombinableGateLabelPair(TDagger2,TDagger2));

	return GateCombinabilityCheckerPtr(new GateSelectiveCombinabilityCheckerImpl(combinableLabelMap));
}





