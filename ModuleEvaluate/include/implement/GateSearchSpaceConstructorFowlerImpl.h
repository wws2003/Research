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
#include "SearchSpaceConstructorUniqueCheckImpl.h"
#include "ISet.h"
#include <set>
#include <unordered_set>
#include <map>

#define MAX_GATE_SEQUENCE_LENGTH (128)

class GateSearchSpaceConstructorFowlerImpl: public SearchSpaceConstructorUniqueCheckImpl<GatePtr> {
public:
	GateSearchSpaceConstructorFowlerImpl(CollectionPtr<GatePtr> pBaseCollection,
			int baseSequenceLength,
			CombinerPtr<GatePtr> pCombiner,
			SetPtr<GatePtr> pGateSet);

	GateSearchSpaceConstructorFowlerImpl(CombinerPtr<GatePtr> pCombiner,
				SetPtr<GatePtr> pGateSet);

	virtual ~GateSearchSpaceConstructorFowlerImpl();

protected:
	//Override
	bool isUnique(GatePtr pSeqGate) const;

	//Override
	void addToUniqueList(GatePtr pSeqGate, bool isMaxLengReached);

	//Override
	bool isMaxLengthBaseElement(GatePtr pBaseGate) const;

private:

	//Check if sub sequences of gates are unique
	bool areSubSequencesUnique(GatePtr pSeqGate) const;

	typedef std::pair<std::string, unsigned int> SequenceWithLength;
	void findSubSequences(GatePtr pSeqGate, std::vector<SequenceWithLength>& subSequences) const;

	//Check if the gate is really unique by calculate distances to previously known unique gates
	bool isUniqueConfirmed(GatePtr pSeqGate) const;

	std::string getGateSeqStr(const LabelSeq& seqs) const;

	void addToUniqueSeqNameSet(GatePtr pSeqGate);

	void addToUniqueSet(GatePtr pSeqGate);

	typedef std::unordered_set<std::string> GateSequenceSet;
	GateSequenceSet m_uniqueGateSequences[MAX_GATE_SEQUENCE_LENGTH];

	SetPtr<GatePtr> m_pGateSet;
};



#endif /* GATESEARCHSPACECONSTRUCTORFOWLERIMPL_H_ */
