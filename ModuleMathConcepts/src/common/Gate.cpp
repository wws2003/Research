/*
 * Gate.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: pham
 */

#include "Gate.h"

Gate::Gate(MatrixPtr pMatrix, cost_t cost, std::string singleLabel) {
	m_pMatrix = pMatrix;
	m_cost = cost;
	m_label.push_back(singleLabel);
	m_labelStr = singleLabel;
}

Gate::Gate(MatrixPtr pMatrix, cost_t cost, const LabelSeq& label) {
	m_pMatrix = pMatrix;
	m_cost = cost;
	m_label = label;
	for(unsigned int i = 0; i < m_label.size(); i++) {
		m_labelStr += m_label[i];
	}
}

Gate::Gate(MatrixPtr pMatrix, cost_t cost, const LabelSeq& label, const std::string& labelStr) {
	m_pMatrix = pMatrix;
	m_cost = cost;
	m_label = label;
	m_labelStr = labelStr;
}

GatePtr Gate::clone() const {
	return GatePtr(new Gate(m_pMatrix->clone(), m_cost, m_label, m_labelStr));
}

bool Gate::operator==(const Gate& rhs) {
	if(m_cost != rhs.m_cost &&
			m_labelStr != rhs.m_labelStr &&
			m_label != rhs.m_label) {
		return false;
	}

	if(m_pMatrix != NullPtr && rhs.m_pMatrix != NullPtr) {
		return (*m_pMatrix == *rhs.m_pMatrix);
	}
	else {
		return (m_pMatrix == NullPtr && rhs.m_pMatrix == NullPtr);
	}
}
