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

	sprintf(command, "%s %s -r 153 -p -e %f -c %d -t",
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

//MARK: Private class
GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::SelingerProgramAdapter(MatrixFactoryPtr pMatrixFactory){
	m_pMatrixFactory = pMatrixFactory;
	initState();
	initContants();
};

void GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::initState() {
	m_state = ST_INIT;
	m_label = "";
	std::fill(m_us, m_us + UV_ARRAY_SIZE, 0);
	std::fill(m_ts, m_ts + UV_ARRAY_SIZE, 0);
	m_error = 0.0;
}

void GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::consumeOutputLine(std::string line) {
	m_state = getNextState(m_state);

	switch (m_state) {
	case ST_READING_LABEL:
		m_label = readLabelFromLine(line);
		break;
	case ST_READING_U:
		readValuesFromLine(line, m_us);
		break;
	case ST_READING_T:
		readValuesFromLine(line, m_ts);
		break;
	case ST_READING_EPS:
		m_error = readErrorFromLine(line);
		break;
	default:
		break;
	}
}

bool GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::isOutputMatrixAvailable() const {
	return (m_state == ST_READING_EPS);
}

LookupResult<MatrixPtr> GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::SelingerProgramAdapter::getOutputMatrix() const {
	MatrixPtr pMatrix = createMatrixFromUT(m_us, m_ts);
	LookupResult<MatrixPtr> result(pMatrix, m_error);
	return result;
}

//MARK: Private methods of private class
void GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::initContants() {
	// 1/sqrt(2)
	mreal_t rSqrt2 = 1/mreal::sqrt(2);

	//omega = e^(i*pi/4) = (1 + i) / sqrt(2)
	omega = ComplexVal(rSqrt2, rSqrt2);

	//omega2 = omega ^ 2
	omega2 = omega * omega;

	//omega3 = omega ^ 3
	omega3 = omega2 * omega;

	int maxK = 28;
	mreal_t rSqrt2Pow = 1.0;
	for(int k = 0; k <= maxK; k++) {
		m_factorMap[k] = rSqrt2Pow;
		rSqrt2Pow *= rSqrt2;
	}
}

GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::State GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::getNextState(State currentState) const {
	State nextState = currentState;
	switch (currentState) {
	case ST_INIT:
		nextState = ST_READING_LABEL;
		break;
	case ST_READING_LABEL:
		nextState = ST_READING_U;
		break;
	case ST_READING_U:
		nextState = ST_READING_T;
		break;
	case ST_READING_T:
		nextState = ST_READING_EPS;
		break;
	case ST_READING_EPS:
		nextState = ST_READING_LABEL;
		break;
	}
	return nextState;
}

std::string GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::readLabelFromLine(std::string line) {
	char prefix[128];
	char label[128];

	sscanf(line.data(), "%[^:]:%[^\n]", prefix, label);
	return std::string(label);
}

void GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::readValuesFromLine(std::string line, int* values) {
	//Totally manual solution

	int currentVal = 0;
	int currentSign = 1;
	bool numberFlg = false;
	int numberCount = 0;

	for(unsigned int i = 0; i < line.size(); i++) {
		char c = line[i];
		if((c >= '0') && (c <= '9')) {
			numberFlg = true;
			currentVal = currentVal * 10 + (c - '0');
		}
		else {
			if(c == '-') {
				currentSign *= -1;
			}
			else {
				if(numberFlg) {
					currentVal *= currentSign;
					values[numberCount++] = currentVal;

					//Reset
					numberFlg = false;
					currentVal = 0;
					currentSign = 1;
				}
			}
		}
	}
}

mreal_t GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::readErrorFromLine(std::string outputLine) {
	std::string line = outputLine;

	char prefix[128];
	char errBuff[128];

	//Error:0.9079002114*10^(-5) -> 0.9079002114*10^(-5)
	sscanf(line.data(), "%[^:]:%[^\n]", prefix, errBuff);
	std::string errStr(errBuff);

	//0.9079002114*10^(-5) -> 0.9079002114e(-5)
	int pos = errStr.find("*10^");
	errStr.replace(pos, 4, "e");

	//0.9079002114e(-5) -> 0.9079002114e-5
	errStr.erase(errStr.find('('), 1);
	errStr.erase(errStr.find(')'), 1);

	//0.9079002114e-5
	double err;
	sscanf(errStr.data(), "%lf", &err);

	mreal_t errVal = err;
	return errVal;
}

MatrixPtr GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::createMatrixFromUT(const int *u, const int *t) const {
	ComplexVal uVal,tVal;

	uVal = calculateComplexValFromRingParam(u);
	tVal = calculateComplexValFromRingParam(t);

	ComplexVal uValConj = std::conj(uVal);
	ComplexVal tValConj = std::conj(tVal);

	/**
	 * [u, -t']
	 * [t,  u']
	 */
	ComplexVal vals[4] = {uVal, -tValConj, tVal, uValConj};

	return m_pMatrixFactory->getMatrixFromValues(2, 2, vals, ROW_SPLICE, m_label);
}

ComplexVal GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::calculateComplexValFromRingParam(const int *params) const {
	//Ref. http://arxiv.org/pdf/1403.2975.pdf

	int k = params[0];
	ComplexVal a(params[1], 0);
	ComplexVal b(params[2], 0);
	ComplexVal c(params[3], 0);
	ComplexVal d(params[4], 0);

	//omega = e^(i*pi/4) = (1 + i) / sqrt(2)
	// t = 1/sqrt(2)^k * (a * omega^3 + b * omega^2 + c * omega + d)

	ComplexVal factor = getReverseSqrt2Pow(k); //1/sqrt(2)^k
	return factor * (a * omega3 + b * omega2 + c * omega + d);
}

ComplexVal GateSelingerComposerEvaluatorImpl::SelingerProgramAdapter::getReverseSqrt2Pow(int exponent) const {
	auto iter = m_factorMap.find(exponent);
	if(iter != m_factorMap.end()) {
		return ComplexVal(iter->second, 0);
	}
	mreal_t rSqrt2 = 1/mreal::sqrt(2);
	return ComplexVal(mreal::pow(rSqrt2, exponent), 0);
}

template class SelingerComposerEvaluatorImpl<GatePtr>;
