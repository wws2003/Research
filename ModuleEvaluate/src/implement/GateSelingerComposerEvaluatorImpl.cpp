/*
 * GateSelingerComposerEvaluatorImpl.cpp
 *
 *  Created on: Jun 7, 2016
 *      Author: pham
 */

#include "GateSelingerComposerEvaluatorImpl.h"
#include "SelingerComposerEvaluatorImpl.cpp"
#include <unistd.h>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>

#define MAX_COMMAND_LENGTH (1000)
#define READ_BUFFER_SIZE (1000)

GateSelingerComposerEvaluatorImpl::GateSelingerComposerEvaluatorImpl(const std::vector<std::vector<GatePtr> > partialTargets,
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
		std::string selingerProgramPath) : SelingerComposerEvaluatorImpl<GatePtr>(partialTargets,
				modifiers,
				buildingBlocksBucketMaxSize,
				epsilon,
				initialEpsilon,
				pTimer,
				pDistanceCalculator,
				pCombiner,
				pRealCoordinateCalculator,
				pRealCoordinateWritter,
				pWriter,
				ostream), m_selingerProgramAdapter(pMatrixFactory) {

	m_selingerProgramPath = selingerProgramPath;
	m_pHelperCollection = pHelperCollection;
}

//@Override
void GateSelingerComposerEvaluatorImpl::generateBuildingBlockBuckets(const std::vector<GatePtr>& partials,
		GatePtr pModifierGate,
		BuildingBlockBuckets<GatePtr>& buildingBlockBuckets) {

	buildingBlockBuckets.clear();

	for(unsigned int i = 0; i < partials.size(); i++) {
		GatePtr pPartialGate = partials[i];
		GatePtr pModifier = (i == 1) ? pModifierGate : NullPtr;
		IteratorPtr<GatePtr> pApprxIter = getApproximationGatesIter(pPartialGate, pModifier);
		buildingBlockBuckets.push_back(pApprxIter);
	}
}

//MARK: Private methods
IteratorPtr<GatePtr> GateSelingerComposerEvaluatorImpl::getApproximationGatesIter(GatePtr pPartialTarget,
		GatePtr pModifier) {

	std::string selingerProgramCommand = getSelingerProgramCommand(pPartialTarget);

	std::vector<LookupResult<GatePtr> > resultBuffer;
	executeSelingerProgramForResults(selingerProgramCommand, resultBuffer);
	std::sort(resultBuffer.begin(), resultBuffer.end(), DistanceComparator<GatePtr>());

	//Apply modifier
	if(pModifier != NullPtr) {
		for(unsigned int i = 0; i < resultBuffer.size(); i++) {
			GatePtr pGate = resultBuffer[i].m_resultElement;
			GatePtr pModifiedGate = NullPtr;
			applyModifier(pGate, pModifier, pModifiedGate);
			_destroy(pGate);
			resultBuffer[i].m_resultElement = pModifiedGate;
		}
	}

	return getIterFromBuffer(resultBuffer);
}

IteratorPtr<GatePtr> GateSelingerComposerEvaluatorImpl::getIterFromBuffer(const std::vector<LookupResult<GatePtr> >& buffer) {
	m_pHelperCollection->clear();
	for(LookupResult<GatePtr> result : buffer) {
		m_pHelperCollection->addElement(result.m_resultElement);
	}
	return m_pHelperCollection->getReadonlyIteratorPtr();
}

std::string GateSelingerComposerEvaluatorImpl::getSelingerProgramCommand(GatePtr pPartialTarget) {
	char command[MAX_COMMAND_LENGTH];

	//MARK This implementation requires gate label set correctly
	std::string zRotationSymbol = pPartialTarget->getLabelStr();
	double err = mreal::toDouble(m_initialEpsilon);

	sprintf(command, "%s %s -r 31313 -e %f -f 3000 -c %d -t | sort -u",
			m_selingerProgramPath.c_str(),
			zRotationSymbol.c_str(),
			err,
			m_buildingBlocksBucketMaxSize);

	std::string commandstr(command);
	return commandstr;
}

void GateSelingerComposerEvaluatorImpl::executeSelingerProgramForResults(std::string programCommand,
		std::vector<LookupResult<GatePtr> >& resultBuffer) {

	m_selingerProgramAdapter.initState();

	const char* command = programCommand.c_str();
	printf("+++++++++++++++++++++++++++++++++\nTo execute command %s\n", command);

	FILE* commandOut = popen(command, "r");
	if(commandOut == NULL) {
		throw new std::runtime_error("In child process something went wrong");
	}

	char* buffLine = new char[READ_BUFFER_SIZE];

	int count;
	size_t readSize = READ_BUFFER_SIZE;
	while((count = getline(&buffLine, &readSize, commandOut)) >= 0) {
		buffLine[count] = '\0';

		if(count == 0) {
			break;
		}
		printf("[Sel]%s", buffLine);
		//Process retrieved line for results
		processSelingerProgramOutputLineForResults(buffLine, resultBuffer);
	}

	delete[] buffLine;
	pclose(commandOut);
}

void GateSelingerComposerEvaluatorImpl::processSelingerProgramOutputLineForResults(std::string outputLine,
		std::vector<LookupResult<GatePtr> >& resultBuffer) {

	m_selingerProgramAdapter.consumeOutputLine(outputLine);

	if(m_selingerProgramAdapter.isOutputMatrixAvailable()) {
		LookupResult<MatrixPtr> result = m_selingerProgramAdapter.getOutputMatrix();

		MatrixPtr pResultMatrix = result.m_resultElement;
		mreal_t err = result.m_distanceToTarget;

		GatePtr pResultGate = GatePtr(new Gate(pResultMatrix, 1, pResultMatrix->getLabel()));
		resultBuffer.push_back(LookupResult<GatePtr> (pResultGate, err));
	}
}

template class SelingerComposerEvaluatorImpl<GatePtr>;
