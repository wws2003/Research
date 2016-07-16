/*
 * GateSearchSpaceConstructorFowlerImpl.cpp
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#include "GateSearchSpaceConstructorFowlerImpl.h"
#include "SearchSpaceConstructorUniqueCheckImpl.cpp"
#include <iostream>

template class SearchSpaceConstructorUniqueCheckImpl<GatePtr>;

#define DUPLICATED_GATE_MINIMUM_LENGTH (4)
#define DISTANCE_TO_CONSIDER_AS_ONE (1e-7)

GateSearchSpaceConstructorFowlerImpl::GateSearchSpaceConstructorFowlerImpl(CombinerPtr<GatePtr> pCombiner,
		CollectionPtr<GatePtr> pBaseCollection,
		int baseSequenceLength,
		SetPtr<GatePtr> pGateSet) : SearchSpaceConstructorUniqueCheckImpl<GatePtr>(pCombiner) {

	m_pBaseCollection = pBaseCollection;
	m_baseSequenceLength = baseSequenceLength;
	m_pGateSet = pGateSet;
}

GateSearchSpaceConstructorFowlerImpl::~GateSearchSpaceConstructorFowlerImpl() {

}

bool GateSearchSpaceConstructorFowlerImpl::isUnique(GatePtr pSeqGate) const {
	return areSubSequencesUnique(pSeqGate) && isUniqueConfirmed(pSeqGate);
}

void GateSearchSpaceConstructorFowlerImpl::addToUniqueList(GatePtr pSeqGate) {
	addToUniqueSeqNameSet(pSeqGate);
	addToDistanceMap(pSeqGate);
}

std::vector<GatePtr>* GateSearchSpaceConstructorFowlerImpl::createCurrentMaxLengthSequences() {
	if(m_pBaseCollection == NullPtr) {
		return NullPtr;
	}
	else {
		IteratorPtr<GatePtr> pBaseIter = m_pBaseCollection->getIteratorPtr();
		std::vector<GatePtr>* pSequences = new std::vector<GatePtr>;
		if(pBaseIter != NullPtr) {
			while(!pBaseIter->isDone()) {
				pSequences->push_back(pBaseIter->getObj()->clone());
				pBaseIter->next();
			}
		}
		_destroy(pBaseIter);
		return pSequences;
	}
}

int GateSearchSpaceConstructorFowlerImpl::getBaseCollectionMaxSequenceLength() {
	return m_baseSequenceLength;
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
		GateSequenceSet uniqueSequencesLengthL = m_uniqueGateSequences[subSequence.second];
		if(uniqueSequencesLengthL.count(subSequence.first) <= 0) {
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
		if(subSeqLength >= DUPLICATED_GATE_MINIMUM_LENGTH && subSeqLength < nbGateSeqs) {
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

void GateSearchSpaceConstructorFowlerImpl::addToDistanceMap(GatePtr pSeqGate) {
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
