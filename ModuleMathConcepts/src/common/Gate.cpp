/*
 * Gate.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: pham
 */

#include "Gate.h"

Gate::Gate(MatrixPtr pMatrix, cost_t cost, std::string label) {
	m_pMatrix = pMatrix;
	m_cost = cost;
	m_label = label;
}

Gate::~Gate() {

}

MatrixPtr Gate::getMatrix() {
	return m_pMatrix;
}

cost_t Gate::getCost() {
	return m_cost;
}

std::string Gate::getLabel() {
	return m_label;
}


