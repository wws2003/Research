/*
 * GateSearchSpaceConstructorFowlerImpl.h
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#ifndef GATESEARCHSPACECONSTRUCTORFOWLERIMPL_H_
#define GATESEARCHSPACECONSTRUCTORFOWLERIMPL_H_

#include "Gate.h"
#include "MathConceptsCommon.h"
#include "SearchSpaceConstructorFowlerImpl.h"
#include <set>
#include <map>

#define MAX_GATE_SEQUENCE_LENGTH (128)

class GateSearchSpaceConstructorFowlerImpl: public SearchSpaceConstructorFowlerImpl<GatePtr> {
public:
	GateSearchSpaceConstructorFowlerImpl(CombinerPtr<GatePtr> pCombiner,
			CollectionPtr<GatePtr> pBaseCollection,
			int baseSequenceLength,
			GateDistanceCalculatorPtr pGateDistanceCalculator);

	virtual ~GateSearchSpaceConstructorFowlerImpl();

protected:
	//Override
	bool isUnique(GatePtr pSeqGate) const;

	//Override
	void addToUniqueList(GatePtr pSeqGate);

	//Override
	std::vector<GatePtr>* createCurrentMaxLengthSequences();

	//Override
	int getBaseCollectionMaxSequenceLength();

private:
	void initSequencesSeqNameSet();

	//Check if sub sequences of gates are unique
	bool areSubSequencesUnique(GatePtr pSeqGate) const;

	//Check if gate is not already in base collection
	bool isInBaseCollection(GatePtr pSeqGate) const;

	typedef std::pair<std::string, unsigned int> SequenceWithLength;
	void findSubSequences(GatePtr pSeqGate, std::vector<SequenceWithLength>& subSequences) const;

	//Check if the gate is really unique by calculate distances to previously known unique gates
	bool isUniqueConfirmed(GatePtr pSeqGate) const;

	std::string getGateSeqStr(const LabelSeq& seqs) const;

	void addToUniqueSeqNameSet(GatePtr pSeqGate);

	void addToDistanceMap(GatePtr pSeqGate);

	GateDistanceCalculatorPtr m_pGateDistanceCalculator;

	typedef std::set<std::string> GateSequenceSet;
	GateSequenceSet m_uniqueGateSequences[MAX_GATE_SEQUENCE_LENGTH];

	typedef std::map<mreal_t, GatePtrVectorPtr> GatesDistanceMap;
	GatesDistanceMap m_gatesDistanceMapToPivot;

	GatePtr m_pPivot;

	//If the following field are set, search space will be constructed from base collection of
	//sequences of length up to m_baseSequenceLength
	CollectionPtr<GatePtr> m_pBaseCollection;
	int m_baseSequenceLength;
};



#endif /* GATESEARCHSPACECONSTRUCTORFOWLERIMPL_H_ */
