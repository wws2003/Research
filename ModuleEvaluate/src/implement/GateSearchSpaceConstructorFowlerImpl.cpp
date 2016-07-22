/*
 * GateSearchSpaceConstructorFowlerImpl.cpp
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#include "GateSearchSpaceConstructorFowlerImpl.h"
#include "SearchSpaceConstructorUniqueCheckImpl.cpp"
#include "Gate.h"
#include <iostream>

template class SearchSpaceConstructorUniqueCheckImpl<GatePtr>;

#define DUPLICATED_GATE_MINIMUM_LENGTH (4)

GateSearchSpaceConstructorFowlerImpl::GateSearchSpaceConstructorFowlerImpl(CollectionPtr<GatePtr> pBaseCollection,
		int baseSequenceLength,
		CombinerPtr<GatePtr> pCombiner,
		SetPtr<GatePtr> pGateSet) : SearchSpaceConstructorUniqueCheckImpl<GatePtr>(pBaseCollection, baseSequenceLength, pCombiner) {

	m_pGateSet = pGateSet;
}

GateSearchSpaceConstructorFowlerImpl::GateSearchSpaceConstructorFowlerImpl(CombinerPtr<GatePtr> pCombiner,
		SetPtr<GatePtr> pGateSet) : SearchSpaceConstructorUniqueCheckImpl<GatePtr>(pCombiner) {

	m_pGateSet = pGateSet;
}

GateSearchSpaceConstructorFowlerImpl::~GateSearchSpaceConstructorFowlerImpl() {

}

bool GateSearchSpaceConstructorFowlerImpl::isUnique(GatePtr pSeqGate) const {
	return areSubSequencesUnique(pSeqGate) && isUniqueConfirmed(pSeqGate);
	//return isUniqueConfirmed(pSeqGate);
}

void GateSearchSpaceConstructorFowlerImpl::addToUniqueList(GatePtr pSeqGate, bool isMaxLengReached) {
	if(!isMaxLengReached) {
		addToUniqueSeqNameSet(pSeqGate);
	}
	addToUniqueSet(pSeqGate);
}

bool GateSearchSpaceConstructorFowlerImpl::isMaxLengthBaseElement(GatePtr pBaseGate) const {
	return m_baseLength == (int)pBaseGate->getLabelSeq().size();
}

//----------------------------MARK: Private methods----------------------------//

bool GateSearchSpaceConstructorFowlerImpl::areSubSequencesUnique(GatePtr pSeqGate) const {
	if(pSeqGate->getLabelSeq().size() < DUPLICATED_GATE_MINIMUM_LENGTH) {
		return true;
	}

	std::vector<SequenceWithLength> subSequences;
	findSubSequences(pSeqGate, subSequences);

	const LabelSeq gateSequences = pSeqGate->getLabelSeq();

	//Sub sequences e.g HTT-3, H1T1T2-3
	for(SequenceWithLength subSequence : subSequences) {
		if(m_uniqueGateSequences[subSequence.second].count(subSequence.first) <= 0) {
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

	//Only need to check sub sequences end at last element
	std::string subSeq = gateSeqs[nbGateSeqs - 1];
	for(int i = nbGateSeqs - 2; i >= 0; i--) {
		subSeq = gateSeqs[i] + delimeter + subSeq; //Prepend to sub-sequence

		unsigned int subSeqLength = nbGateSeqs - i;
		if(subSeqLength >= DUPLICATED_GATE_MINIMUM_LENGTH && subSeqLength < nbGateSeqs && i == 1) {
			subSequences.push_back(SequenceWithLength(subSeq, subSeqLength));
		}
	}
}

bool GateSearchSpaceConstructorFowlerImpl::isUniqueConfirmed(GatePtr pSeqGate) const {
	return m_pGateSet->isUnique(pSeqGate);
}

void GateSearchSpaceConstructorFowlerImpl::addToUniqueSeqNameSet(GatePtr pSeqGate) {
	std::string gateSeqStr = getGateSeqStr(pSeqGate->getLabelSeq());
	m_uniqueGateSequences[pSeqGate->getLabelSeq().size()].insert(gateSeqStr);
}

void GateSearchSpaceConstructorFowlerImpl::addToUniqueSet(GatePtr pSeqGate) {
	m_pGateSet->addElement(pSeqGate);
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
