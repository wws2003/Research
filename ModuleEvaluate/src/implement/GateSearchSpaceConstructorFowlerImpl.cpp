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

GateSearchSpaceConstructorFowlerImpl::GateSearchSpaceConstructorFowlerImpl(CombinerPtr<GatePtr> pCombiner,
		CollectionPtr<GatePtr> pBaseCollection,
		int baseSequenceLength,
		GateDistanceCalculatorPtr pGateDistanceCalculator) : SearchSpaceConstructorFowlerImpl<GatePtr>(pCombiner), m_gate2DMap(pGateDistanceCalculator) {

	m_pBaseCollection = pBaseCollection;
	m_baseSequenceLength = baseSequenceLength;
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
	return m_gate2DMap.isUniqueGate(pSeqGate);
}

void GateSearchSpaceConstructorFowlerImpl::addToUniqueSeqNameSet(GatePtr pSeqGate) {
	std::string gateSeqStr = getGateSeqStr(pSeqGate->getLabelSeq());
	m_uniqueGateSequences[pSeqGate->getLabelSeq().size()].insert(gateSeqStr);
}

void GateSearchSpaceConstructorFowlerImpl::addToDistanceMap(GatePtr pSeqGate) {
	m_gate2DMap.addGate(pSeqGate);
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

//----------------------MARK: Inner class--------------------------//
GateSearchSpaceConstructorFowlerImpl::Gate2DMap::Gate2DMap(GateDistanceCalculatorPtr pGateDistanceCalculator) {
	m_pPivot1 = NullPtr;
	m_pPivot2 = NullPtr;
	m_pGateDistanceCalculator = pGateDistanceCalculator;
}

GateSearchSpaceConstructorFowlerImpl::Gate2DMap::~Gate2DMap() {
	for(GateDistance2DTable::iterator dIter = m_distanceTable.begin(); dIter != m_distanceTable.end();) {
		GatesDistanceMapPtr psubMap = dIter->second;
		for(GatesDistanceMap::iterator dSubIter = psubMap->begin(); dSubIter != psubMap->end();) {
			GatePtrVectorPtr pGateVector = dSubIter->second;
			_destroy(pGateVector);
			dSubIter = psubMap->erase(dSubIter);
		}
		_destroy(psubMap);
		dIter = m_distanceTable.erase(dIter);
	}
}

bool GateSearchSpaceConstructorFowlerImpl::Gate2DMap::isUniqueGate(GatePtr pSeqGate) const {
	if(!arePivotReady()) {
		return true;
	}
	mreal_t distanceToPivot1;
	mreal_t distanceToPivot2;

	distanceToPivots(pSeqGate, distanceToPivot1, distanceToPivot2);

	mreal_t eps = 2e-6;

	GateDistance2DTable::const_iterator dLowerIter = m_distanceTable.lower_bound(distanceToPivot1 - eps);
	GateDistance2DTable::const_iterator dUpperIter2 = m_distanceTable.upper_bound(distanceToPivot1 + eps);

	int distanceCalculateCounter = 0;
	for(auto dIter = dLowerIter; dIter != dUpperIter2; dIter++) {
		GatesDistanceMapPtr pSubMap = dIter->second;

		GatesDistanceMap::const_iterator dSubLowerIter = pSubMap->lower_bound(distanceToPivot2 - eps);
		GatesDistanceMap::const_iterator dSubUpperIter = pSubMap->upper_bound(distanceToPivot2 + eps);

		for(auto dSubIter = dSubLowerIter; dSubIter != dSubUpperIter; dSubIter++) {
			GatePtrVectorPtr pGatesWithSameDistanceToPivot = dSubIter->second;

			for(GatePtr pGate : *pGatesWithSameDistanceToPivot) {
				distanceCalculateCounter++;
				//Check if there is one gate too close to the target gate so that can deal as one
				if(m_pGateDistanceCalculator->distance(pGate, pSeqGate) <= DISTANCE_TO_CONSIDER_AS_ONE) {
					return false;
				}
			}
		}
	}

	/*std::cout << "---For " << pSeqGate->getLabelStr()
			<< " distance to pivot 1 = " << mreal::toDouble(distanceToPivot1)
			<< " distance to pivot 2 = " << mreal::toDouble(distanceToPivot2)
			<< " Number of distance to calculated " << distanceCalculateCounter << "\n";
	 */
	return true;
}

void GateSearchSpaceConstructorFowlerImpl::Gate2DMap::addGate(GatePtr pSeqGate) {
	if(!arePivotReady()) {
		updatePivots(pSeqGate);
		if(arePivotReady()) {
			addGateWhenPivotsReady(m_pPivot1);
			addGateWhenPivotsReady(m_pPivot2);
		}
	}
	else {
		addGateWhenPivotsReady(pSeqGate);
	}
}

bool GateSearchSpaceConstructorFowlerImpl::Gate2DMap::arePivotReady() const {
	return m_pPivot1 != NullPtr && m_pPivot2 != NullPtr;
}

void GateSearchSpaceConstructorFowlerImpl::Gate2DMap::distanceToPivots(GatePtr pGate, mreal_t& d1, mreal_t& d2) const {
	d1 = pGate == m_pPivot1 ? 0.0 : m_pGateDistanceCalculator->distance(pGate, m_pPivot1);
	d2 = pGate == m_pPivot2 ? 0.0 : m_pGateDistanceCalculator->distance(pGate, m_pPivot2);
}

void GateSearchSpaceConstructorFowlerImpl::Gate2DMap::addGateWhenPivotsReady(GatePtr pSeqGate) {
	mreal_t distanceToPivot1;
	mreal_t distanceToPivot2;

	distanceToPivots(pSeqGate, distanceToPivot1, distanceToPivot2);

	GateDistance2DTable::iterator dIter = m_distanceTable.find(distanceToPivot1);

	GatePtrVectorPtr pGatesWithSameDistanceToPivot2 = NullPtr;

	if(dIter == m_distanceTable.end()) {
		GatesDistanceMapPtr pSubMap = new GatesDistanceMap();
		pGatesWithSameDistanceToPivot2 = GatePtrVectorPtr(new GatePtrVector());
		pSubMap->insert(std::pair<mreal_t, GatePtrVectorPtr>(distanceToPivot2, pGatesWithSameDistanceToPivot2));
		m_distanceTable.insert(std::pair<mreal_t, GatesDistanceMapPtr>(distanceToPivot1, pSubMap));
	}
	else {
		GatesDistanceMapPtr pSubMap = dIter->second;
		GatesDistanceMap::iterator dSubIter = pSubMap->find(distanceToPivot2);
		if(dSubIter == pSubMap->end()) {
			pGatesWithSameDistanceToPivot2 = GatePtrVectorPtr(new GatePtrVector());
			pSubMap->insert(std::pair<mreal_t, GatePtrVectorPtr>(distanceToPivot2, pGatesWithSameDistanceToPivot2));
		}
		else {
			pGatesWithSameDistanceToPivot2 = dSubIter->second;
		}
	}

	pGatesWithSameDistanceToPivot2->push_back(pSeqGate);
}

void GateSearchSpaceConstructorFowlerImpl::Gate2DMap::updatePivots(GatePtr pGate) {
	if(m_pPivot1 == NullPtr) {
		m_pPivot1 = pGate;
		return;
	}
	if(m_pPivot2 == NullPtr) {
		m_pPivot2 = pGate;
		return;
	}
}
