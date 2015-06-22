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

Gate::~Gate() {

}
