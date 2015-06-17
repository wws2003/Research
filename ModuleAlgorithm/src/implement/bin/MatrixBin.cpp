/*
 * MatrixBin.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: pham
 */


#include "MatrixBin.h"
#include <cstdlib>
#include <cstring>

MatrixBin::MatrixBin(BinPattern pattern) {
	m_pattern = pattern;
}

MatrixBin::~MatrixBin() {
}

int MatrixBin::distance(const MatrixBin& otherBin) const {
	int distance = m_pattern.size();
	for(unsigned int i = 0; i < m_pattern.size(); i++) {
		if(m_pattern[i] != otherBin.m_pattern[i]) {
			distance--;
		}
	}
	return distance;
}

