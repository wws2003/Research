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

	class SelingerProgramAdapter {
	public:
		SelingerProgramAdapter(MatrixFactoryPtr pMatrixFactory);

		virtual ~SelingerProgramAdapter(){};

		void initState();

		void consumeOutputLine(std::string line);

		bool isOutputMatrixAvailable() const;

		LookupResult<MatrixPtr> getOutputMatrix() const;

	private:
		void initContants();

		enum State{
			ST_INIT,
			ST_READING_LABEL,
			ST_READING_U,
			ST_READING_T,
			ST_READING_EPS
		};

		State getNextState(State currentState) const;

		//Methods to read proper values from line
		std::string readLabelFromLine(std::string line);
		void readValuesFromLine(std::string line, int* values);
		mreal_t readErrorFromLine(std::string line);

		MatrixPtr createMatrixFromUT(const int *u, const int *t) const;

		ComplexVal calculateComplexValFromRingParam(const int *params) const;

		ComplexVal getReverseSqrt2Pow(int pow) const;

		//Fields to remember read values
		State m_state;
		std::string m_label;
		int m_us[UV_ARRAY_SIZE];
		int m_ts[UV_ARRAY_SIZE];
		mreal_t m_error;

		MatrixFactoryPtr m_pMatrixFactory;

		//Constants values
		std::map<int, mreal_t> m_factorMap;
		ComplexVal omega;
		ComplexVal omega2;
		ComplexVal omega3;
	};

	SelingerProgramAdapter m_selingerProgramAdapter;

};

#endif /* GATESELINGERCOMPOSEREVALUATORIMPL_H_ */
