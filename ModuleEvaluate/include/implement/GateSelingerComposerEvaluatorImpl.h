/*
 * GateSelingerComposerEvaluatorImpl.h
 *
 *  Created on: Jun 7, 2016
 *      Author: pham
 */

#ifndef GATESELINGERCOMPOSEREVALUATORIMPL_H_
#define GATESELINGERCOMPOSEREVALUATORIMPL_H_

#include "Gate.h"
#include "IMatrix.h"
#include "SelingerComposerEvaluatorImpl.h"
#include "IMatrixFactory.h"
#include "MathConceptsCommon.h"
#include "ICollection.h"
#include "SelingerProgramAdapter.h"
#include <map>

#define UV_ARRAY_SIZE (5)

class GateSelingerComposerEvaluatorImpl: public SelingerComposerEvaluatorImpl<GatePtr> {
public:
	GateSelingerComposerEvaluatorImpl(const std::vector<std::vector<GatePtr> > partialTargets,
			const std::vector<GatePtr> modifiers,
			int buildingBlocksBucketMaxSize,
			mreal_t epsilon,
			mreal_t initialEpsilon,
			TimerPtr pTimer,
			DistanceCalculatorPtr<GatePtr> pDistanceCalculator,
			CombinerPtr<GatePtr> pCombiner,
			RealCoordinateCalculatorPtr<GatePtr> pRealCoordinateCalculator,
			RealCoordinateWriterPtr<GatePtr> pRealCoordinateWritter,
			WriterPtr<GatePtr> pWriter,
			CollectionPtr<GatePtr> pHelperCollection,
			std::ostream& ostream,
			MatrixFactoryPtr pMatrixFactory,
			std::string selingerProgramPath);

	virtual ~GateSelingerComposerEvaluatorImpl(){};

protected:
	//@Override
	virtual void generateBuildingBlockBuckets(const std::vector<GatePtr>& partials,
			GatePtr pModifierGate,
			BuildingBlockBuckets<GatePtr>& buildingBlockBuckets);

private:
	IteratorPtr<GatePtr> getApproximationGatesIter(GatePtr pPartialTarget, GatePtr pModifier);

	IteratorPtr<GatePtr> getIterFromBuffer(const std::vector<LookupResult<GatePtr> >& buffer);

	std::string getSelingerProgramCommand(GatePtr pPartialTarget);

	void executeSelingerProgramForResults(std::string programCommand,
			std::vector<LookupResult<GatePtr> >& resultBuffer);

	void processSelingerProgramOutputLineForResults(std::string outputLine,
			std::vector<LookupResult<GatePtr> >& resultBuffer);

	std::string m_selingerProgramPath;

	CollectionPtr<GatePtr> m_pHelperCollection;

	SelingerProgramAdapter m_selingerProgramAdapter;

};

#endif /* GATESELINGERCOMPOSEREVALUATORIMPL_H_ */
