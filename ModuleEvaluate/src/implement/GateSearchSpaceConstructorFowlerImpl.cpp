/*
 * GateSearchSpaceConstructorFowlerImpl.cpp
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#include "GateSearchSpaceConstructorFowlerImpl.h"
#include "SearchSpaceConstructorFowlerImpl.cpp"
#include <iostream>

template class SearchSpaceConstructorFowlerImpl<GatePtr>;

#define DUPLICATED_GATE_MINIMUM_LENGTH (4)
#define DISTANCE_TO_CONSIDER_AS_ONE (1e-7)

GateSearchSpaceConstructorFowlerImpl::GateSearchSpaceConstructorFowlerImpl(CombinerPtr<GatePtr> pCombiner, GateDistanceCalculatorPtr pGateDistanceCalculator) : SearchSpaceConstructorFowlerImpl<GatePtr>(pCombiner) {
	m_pGateDistanceCalculator = pGateDistanceCalculator;
	m_pPivot = NullPtr;
}

GateSearchSpaceConstructorFowlerImpl::~GateSearchSpaceConstructorFowlerImpl() {
	for(GatesDistanceMap::iterator dIter = m_gatesDistanceMapToPivot.begin(); dIter != m_gatesDistanceMapToPivot.end();) {
		GatePtrVectorPtr pGateVector = dIter->second;
		_destroy(pGateVector);
		dIter = m_gatesDistanceMapToPivot.erase(dIter);
	}
}

bool GateSearchSpaceConstructorFowlerImpl::isUnique(GatePtr pSeqGate) const {
#ifdef DEBUG_FL
	if(pSeqGate->getLabelStr() == "HTHSHSHTH") {
		std::cout << "Checking HTHSHSHTH" << "\n";
	}
#endif
	return areSubSequencesUnique(pSeqGate) && isUniqueConfirmed(pSeqGate);
}

void GateSearchSpaceConstructorFowlerImpl::addToUniqueList(GatePtr pSeqGate) {
	addToUniqueSeqNameSet(pSeqGate);
	addToDistanceMap(pSeqGate);
}

bool GateSearchSpaceConstructorFowlerImpl::areSubSequencesUnique(GatePtr pSeqGate) const {
	if(pSeqGate->getLabelSeq().size() < DUPLICATED_GATE_MINIMUM_LENGTH) {
		return true;
	}

	std::vector<SequenceWithLength> subSequences;
	findSubSequences(pSeqGate, subSequences);

	const LabelSeq gateSequences = pSeqGate->getLabelSeq();

	//Sub sequences e.g HTT-3, H1T1T2-3
	for(SequenceWithLength subSequence : subSequences) {
		GateSequenceSet uniqueSequencesLengthL = m_uniqueGateSequences[subSequence.second];
		if(uniqueSequencesLengthL.count(subSequence.first) <= 0) {
#ifdef DEBUG_FL
			if(pSeqGate->getLabelStr() == "HTHSHSHTH") {
				std::cout << "Failed due to non-unique sub sequence " << subSequence.first << "\n";
			}
#endif
			return false;
		}
	}

	return true;
}

void GateSearchSpaceConstructorFowlerImpl::findSubSequences(GatePtr pSeqGate, std::vector<SequenceWithLength>& subSequences) const {
	//Note that this method does not need to find all sub sequences but as much as possible
	LabelSeq gateSeqs = pSeqGate->getLabelSeq();
	unsigned int nbGateSeqs = gateSeqs.size();

	const std::string delimeter = "_";

	/*for(unsigned int i = 0; i <= nbGateSeqs - DUPLICATED_GATE_MINIMUM_LENGTH; i++) {
		std::string subSeq = gateSeqs[i];

		for(unsigned int j = i + 1; j < nbGateSeqs - 1; j++) {
			subSeq += delimeter + gateSeqs[j];

			unsigned int subSeqLength = j - i + 1;
			if(subSeqLength >= DUPLICATED_GATE_MINIMUM_LENGTH) {
				subSequences.push_back(SequenceWithLength(subSeq, subSeqLength));
			}
		}
	}*/

	//Only need to check sub sequences end at last element
	std::string subSeq = gateSeqs[nbGateSeqs - 1];
	for(int i = nbGateSeqs - 2; i >= 0; i--) {
		subSeq = gateSeqs[i] + delimeter + subSeq; //Prepend to sub-sequence

		unsigned int subSeqLength = nbGateSeqs - i;
		if(subSeqLength >= DUPLICATED_GATE_MINIMUM_LENGTH && subSeqLength < nbGateSeqs) {
			subSequences.push_back(SequenceWithLength(subSeq, subSeqLength));
		}
	}
}

bool GateSearchSpaceConstructorFowlerImpl::isUniqueConfirmed(GatePtr pSeqGate) const {
	if(m_pPivot == NullPtr) {
		return true;
	}

	mreal_t distanceToPivot = m_pGateDistanceCalculator->distance(pSeqGate, m_pPivot);
	mreal_t eps = 1e-5;

	GatesDistanceMap::const_iterator dIter1 = m_gatesDistanceMapToPivot.lower_bound(distanceToPivot - eps);
	GatesDistanceMap::const_iterator dIter2 = m_gatesDistanceMapToPivot.lower_bound(distanceToPivot + eps);

	for(GatesDistanceMap::const_iterator dIter = dIter1; dIter != dIter2; dIter++) {
		GatePtrVectorPtr pGatesWithSameDistanceToPivot = dIter->second;
		for(GatePtr pGate : *pGatesWithSameDistanceToPivot) {
			//Check if there is one gate too close to the target gate so that can deal as one
			if(m_pGateDistanceCalculator->distance(pGate, pSeqGate) <= DISTANCE_TO_CONSIDER_AS_ONE) {
#ifdef DEBUG_FL
				if(pSeqGate->getLabelStr() == "HTHSHSHTH") {
					std::cout << "Failed due to duplicated with " << pGate->getLabelStr() << "\n";
				}
#endif
				return false;
			}
		}
	}

	return true;
}

void GateSearchSpaceConstructorFowlerImpl::addToUniqueSeqNameSet(GatePtr pSeqGate) {
	std::string gateSeqStr = getGateSeqStr(pSeqGate->getLabelSeq());
	m_uniqueGateSequences[pSeqGate->getLabelSeq().size()].insert(gateSeqStr);
}

void GateSearchSpaceConstructorFowlerImpl::addToDistanceMap(GatePtr pSeqGate) {
	mreal_t distanceToPivot = 0.0;
	if(m_pPivot == NullPtr) {
		m_pPivot = pSeqGate;
	}
	else {
		//This is calculated before but no way to re-use ??
		distanceToPivot = m_pGateDistanceCalculator->distance(pSeqGate, m_pPivot);
	}

	GatePtrVectorPtr pGatesWithSameDistanceToPivot = NullPtr;
	GatesDistanceMap::iterator dIter = m_gatesDistanceMapToPivot.find(distanceToPivot);
	if(dIter == m_gatesDistanceMapToPivot.end()) {
		pGatesWithSameDistanceToPivot = GatePtrVectorPtr(new GatePtrVector());
		m_gatesDistanceMapToPivot[distanceToPivot] = pGatesWithSameDistanceToPivot;
	}
	else {
		pGatesWithSameDistanceToPivot = dIter->second;
	}

	pGatesWithSameDistanceToPivot->push_back(pSeqGate);
}

std::string GateSearchSpaceConstructorFowlerImpl::getGateSeqStr(const LabelSeq& seqs) const {
	std::string seqStr = "";
	const std::string delimeter = "_";

	bool firstSeq = true;
	for(std::string seq : seqs) {
		seqStr += firstSeq ? seq : delimeter + seq;
		firstSeq = false;
	}
	return seqStr;
}
