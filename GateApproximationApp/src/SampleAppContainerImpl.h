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
#include "IBinCollection.h"
#include "AlgoInternal.h"
#include "NearIdentityElementApproximator.h"
#include <map>

typedef std::map<std::string, int> LibrarySetNameMap;
typedef std::map<int, std::string> LibrarySetFileNameMap;

class SampleAppContainerImpl: public IAppContainer {
public:
	SampleAppContainerImpl(std::string configFile = "in.conf", std::string approximatorConfigFile = "near_identity_approximator.conf");

	virtual ~SampleAppContainerImpl();

	virtual GateCollectionPtr getGateCollection();

	virtual GateApproximatorPtr getGateApproximator();

	virtual GateSearchSpaceEvaluatorPtr getGateSearchSpaceEvaluator();

	virtual void recycle(GateCollectionPtr& rpGateCollection);

	virtual void recycle(GateApproximatorPtr& rpGateApproximator);

	virtual void recycle(GateSearchSpaceEvaluatorPtr& rpGateSearchSpaceEvaluator);

private:
	enum LibrarySet {
		L_HT,
		L_HTCNOT,
		L_HCV
	};

	typedef struct _EvaluationConfig {
		LibrarySet m_librarySet;
		int m_maxSequenceLength;
		int m_nbQubits;
		double m_collectionEpsilon;
		double m_approximatorEpsilon;
	} EvaluationConfig;

	void initLibrarySetNameMap();

	void initLibrarySetPersistFileNameMap();

	void readEvaluatorConfigFromFile(std::string configFile);

	void readApproximatorConfigFromFile(std::string configFile);

	void wireDependencies();

	void setupResourceContainer();

	std::string getGateCollectionPersistenceFileFullName(const SampleAppContainerImpl::EvaluationConfig& config,
				const LibrarySetFileNameMap& librarySetFileNameMap,
				std::string fileExtension);

	void constructGateCollection(GateCollectionPtr pGateCollection);

	void releaseDependencies();

	LibrarySetNameMap m_librarySetNameMap;
	LibrarySetFileNameMap m_librarySetPersistFileNameMap;

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

	CombinerPtr<GatePtr> m_pGateInBinCombiner;
	BinCollectionPtr<GatePtr> m_pBinCollection;

	EvaluationConfig m_evaluationConfig;
	NearIdentityElementApproximator<GatePtr>::Config m_nearIdentityApproximatorConfig;

	const static int DEFAULT_MAX_SEQUENCE_LENGTH;
	const static int DEFAULT_NB_QUBITS;
	const static double DEFAULT_COLLECTION_EPSILON;
	const static double DEFAULT_APPROXIMATOR_EPSILON;

	const static NearIdentityElementApproximator<GatePtr>::Config DEFAULT_NEAR_IDENTITY_APPROXIMATOR_CONFIG;

	const static std::string DEFAULT_GATE_COLLECTION_PERSIST_FILE_EXT;
};


#endif /* SAMPLEAPPCONTAINERIMPL_H_ */
