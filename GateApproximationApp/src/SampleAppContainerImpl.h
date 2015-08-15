/*
 * SampleAppContainerImpl.h
 *
 *  Created on: Aug 15, 2015
 *      Author: pham
 */

#ifndef SAMPLEAPPCONTAINERIMPL_H_
#define SAMPLEAPPCONTAINERIMPL_H_

#include "IAppContainer.h"
#include "EvaluateCommon.h"
#include "OperatorCommon.h"
#include "IOCommon.h"
#include "MathConceptsCommon.h"
#include "AlgoCommon.h"
#include "IResourceContainer.h"

class SampleAppContainerImpl: public IAppContainer {
public:
	//TODO May add some user config params / config file name
	SampleAppContainerImpl();

	virtual ~SampleAppContainerImpl();

	virtual GateCollectionPtr getGateCollection();

	virtual GateSearchSpaceEvaluatorPtr getGateSearchSpaceEvaluator();

	virtual void recycle(GateCollectionPtr& rpGateCollection);

	virtual void recycle(GateSearchSpaceEvaluatorPtr& rpGateSearchSpaceEvaluator);

private:
	void constructGateCollection(GateCollectionPtr pGateCollection);

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;

	ResourceContainerPtr m_pResourceContainer;

	GateWriterPtr m_pBinaryGateWriter;
	GateReaderPtr m_pBinaryGateReader;

	MatrixWriterPtr m_pBinaryMatrixWriter;
	MatrixReaderPtr m_pBinaryMatrixReader;

	GateCollectionPtr m_pUniversalSet;

	CombinerPtr<GatePtr> m_pGateCombiner;
	GateSearchSpaceConstructorPtr m_pGateSearchSpaceConstructor;

	MatrixDistanceCalculatorPtr m_pMatrixDistanceCalculator;
	GateDistanceCalculatorPtr m_pGateDistanceCalculator;

	GateWriterPtr m_pGateWriterInEvaluator;
	MatrixRealInnerProductCalculatorPtr m_pMatrixRealInnerProductCalculator;
	MatrixRealCoordinateCalculatorPtr m_pHermitiaRealCoordinateCalculator;
	MatrixRealCoordinateCalculatorPtr m_pMatrixRealCoordinateCalculator;
	GateRealCoordinateCalculatorPtr m_pGateRealCoordinateCalculator;
	RealCoordinateWriterPtr<GatePtr> m_pCoordinateWriter;
	TimerPtr m_pTimer;

	const static int MAX_SEQUENCE_LENGTH;
	const static std::string GATE_COLLECTION_PERSIST_FILE_NAME;
	const static std::string GATE_COLLECTION_PERSIST_FILE_EXT;
};



#endif /* SAMPLEAPPCONTAINERIMPL_H_ */
