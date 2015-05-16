/*
 * SampleSearchSpaceEvaluatorFactoryImpl.cpp
 *
 *  Created on: May 15, 2015
 *      Author: pham
 */

#include "SampleSearchSpaceEvaluatorFactoryImpl.h"

SampleSearchSpaceEvaluatorFactoryImpl::SampleSearchSpaceEvaluatorFactoryImpl(const TargetMatrices& pTargets, double epsilon, MatrixDistanceCalculatorPtr pMatrixDistanceCalculator, MatrixWriterPtr pMatrixWriter, TimerPtr pTimer, std::ostream& outputStream) :m_ostream(outputStream) {
	setTargetMatrices(pTargets);
	setEpsilon(epsilon);
	m_pMatrixDistanceCalculator = pMatrixDistanceCalculator;
	m_pMatrixWriter = pMatrixWriter;
}

SearchSpaceEvaluatorPtr SampleSearchSpaceEvaluatorFactoryImpl::getSearchSpaceEvaluatorPtr() {
	return new SearchSpaceTimerEvaluatorImpl(m_targetMatrices, m_epsilon, m_pMatrixDistanceCalculator, m_pMatrixWriter, m_pTimer, m_ostream);
}

void SampleSearchSpaceEvaluatorFactoryImpl::setTargetMatrices(const TargetMatrices& pTargets) {
	m_targetMatrices.clear();
	m_targetMatrices.insert(m_targetMatrices.end(), pTargets.begin(), pTargets.end());
}

void SampleSearchSpaceEvaluatorFactoryImpl::setEpsilon(double epsilon) {
	m_epsilon = epsilon;
}

