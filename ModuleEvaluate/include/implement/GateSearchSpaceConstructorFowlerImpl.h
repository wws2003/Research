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

	//Check if sub sequences of gates are unique
	bool areSubSequencesUnique(GatePtr pSeqGate) const;

	typedef std::pair<std::string, unsigned int> SequenceWithLength;
	void findSubSequences(GatePtr pSeqGate, std::vector<SequenceWithLength>& subSequences) const;

	//Check if the gate is really unique by calculate distances to previously known unique gates
	bool isUniqueConfirmed(GatePtr pSeqGate) const;

	std::string getGateSeqStr(const LabelSeq& seqs) const;

	void addToUniqueSeqNameSet(GatePtr pSeqGate);

	void addToDistanceMap(GatePtr pSeqGate);

	typedef std::set<std::string> GateSequenceSet;
	GateSequenceSet m_uniqueGateSequences[MAX_GATE_SEQUENCE_LENGTH];

	//If the following field are set, search space will be constructed from base collection of
	//sequences of length up to m_baseSequenceLength
	CollectionPtr<GatePtr> m_pBaseCollection;
	int m_baseSequenceLength;

	class Gate2DMap {
	public:
		Gate2DMap(GateDistanceCalculatorPtr pGateDistanceCalculator);
		virtual ~Gate2DMap();

		bool isUniqueGate(GatePtr pGate) const;
		void addGate(GatePtr pGate);

	private:
		bool arePivotReady() const;
		void distanceToPivots(GatePtr pGate, mreal_t& d1, mreal_t& d2) const;

		void addGateWhenPivotsReady(GatePtr pGate);

		void updatePivots(GatePtr pGate);

		typedef std::map<mreal_t, GatePtrVectorPtr> GatesDistanceMap;
		typedef GatesDistanceMap* GatesDistanceMapPtr;
		typedef std::map<mreal_t, GatesDistanceMapPtr> GateDistance2DTable;
		GateDistance2DTable m_distanceTable;

		GatePtr m_pPivot1;
		GatePtr m_pPivot2;

		GateDistanceCalculatorPtr m_pGateDistanceCalculator;
	};

	Gate2DMap m_gate2DMap;
};



#endif /* GATESEARCHSPACECONSTRUCTORFOWLERIMPL_H_ */
