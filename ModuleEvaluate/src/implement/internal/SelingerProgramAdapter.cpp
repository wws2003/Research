/*
 * SelingerProgramAdapter.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: pham
 */

#include "SelingerProgramAdapter.h"

SelingerProgramAdapter::SelingerProgramAdapter(MatrixFactoryPtr pMatrixFactory){
	m_pMatrixFactory = pMatrixFactory;
	initState();
	initContants();
};

void SelingerProgramAdapter::initState() {
	m_label = "";
	std::fill(m_us, m_us + UV_ARRAY_SIZE, 0);
	std::fill(m_ts, m_ts + UV_ARRAY_SIZE, 0);
	m_error = 0.0;
}

void SelingerProgramAdapter::consumeOutputLine(std::string line) {
	std::string uValueStr;
	std::string tValueStr;
	std::string errorValueStr;
	std::string circuitValueStr;

	parseOutputLine(line, uValueStr, tValueStr, errorValueStr, circuitValueStr);

	readValuesFromStr(uValueStr, m_us);
	readValuesFromStr(tValueStr, m_ts);
	m_error = readErrorFromStr(errorValueStr);
	m_label = readLabelFromStr(circuitValueStr);
}

bool SelingerProgramAdapter::isOutputMatrixAvailable() const {
	return m_label != "I" && !m_label.empty();
}

LookupResult<MatrixPtr> SelingerProgramAdapter::SelingerProgramAdapter::getOutputMatrix() const {
	MatrixPtr pMatrix = createMatrixFromUT(m_us, m_ts);
	LookupResult<MatrixPtr> result(pMatrix, m_error);
	return result;
}

//MARK: Private methods
void SelingerProgramAdapter::initContants() {
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

void SelingerProgramAdapter::parseOutputLine(std::string outputLine,
		std::string& uValueStr,
		std::string& tValueStr,
		std::string& errorValueStr,
		std::string& circuitValueStr) {

	char delimeter = '#';
	const char* data = outputLine.data();

	auto getOutputBuffer = [&uValueStr, &tValueStr, &errorValueStr, &circuitValueStr](int bufferIndex) -> std::string& {
		switch(bufferIndex) {
		case 0:
			return uValueStr;
		case 1:
			return tValueStr;
		case 2:
			return errorValueStr;
		case 3:
			return circuitValueStr;
		default:
			return circuitValueStr;
		}
	};

	int bufferIndex = 0;
	std::string buffer = "";
	for(unsigned int i = 0; data[i] != '\0'; i++) {
		const char c = data[i];
		if(c == delimeter) {
			std::string& outputBuffer = getOutputBuffer(bufferIndex);
			outputBuffer = buffer;
			bufferIndex++;
			buffer = "";
		}
		else {
			buffer.append(1, c);
		}
	}
}

void SelingerProgramAdapter::readValuesFromStr(std::string valueStr, int* values) {
	//Pre-process for special case
	if(valueStr.find("Omega") == 0) {
		valueStr = "0 * " + valueStr;
	}

	//Totally manual solution
	int currentVal = 0;
	int currentSign = 1;
	bool numberFlg = false;
	int numberCount = 0;

	for(unsigned int i = 0; i <= valueStr.size(); i++) {
		char c = valueStr[i];
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

mreal_t SelingerProgramAdapter::readErrorFromStr(std::string errorStr) {
	int pos = errorStr.find("*10^");
	errorStr.replace(pos, 4, "e");

	errorStr.erase(errorStr.find('('), 1);
	errorStr.erase(errorStr.find(')'), 1);

	double err;
	sscanf(errorStr.data(), "%lf", &err);

	return (mreal_t)err;
}

std::string SelingerProgramAdapter::readLabelFromStr(std::string labelStr) {
	std::string label;
	size_t nbGate = labelStr.size();
	for(unsigned int i = 0; i < nbGate; i++) {
		label.append(1, labelStr.at(i));
		if(i != nbGate - 1) {
			label.append(1, '*');
		}
	}
	return label;
}

MatrixPtr SelingerProgramAdapter::createMatrixFromUT(const int *u, const int *t) const {
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

ComplexVal SelingerProgramAdapter::calculateComplexValFromRingParam(const int *params) const {
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

ComplexVal SelingerProgramAdapter::getReverseSqrt2Pow(int exponent) const {
	auto iter = m_factorMap.find(exponent);
	if(iter != m_factorMap.end()) {
		return ComplexVal(iter->second, 0);
	}
	mreal_t rSqrt2 = 1/mreal::sqrt(2);
	return ComplexVal(mreal::pow(rSqrt2, exponent), 0);
}

