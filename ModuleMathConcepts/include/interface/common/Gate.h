/*
 * Gate.h
 *
 *  Created on: Jun 17, 2015
 *      Author: pham
 */

#ifndef GATE_H_
#define GATE_H_

#include "IMatrix.h"
#include <string>
#include <vector>

typedef unsigned int cost_t;
typedef std::vector<std::string> label_t;

class Gate {
public:
	Gate(MatrixPtr pMatrix, cost_t cost, const label_t& label);

	Gate(MatrixPtr pMatrix, cost_t cost, const label_t& label, const std::string& labelStr);

	virtual ~Gate();

	inline MatrixPtr getMatrix() {
		return m_pMatrix;
	}

	inline cost_t getCost() {
		return m_cost;
	}

	inline const label_t& getLabel() {
		return m_label;
	}

	inline const std::string& getLabelStr() {
		return m_labelStr;
	}

private:
	MatrixPtr m_pMatrix;
	cost_t m_cost;
	label_t m_label;
	std::string m_labelStr;
};


#endif /* GATE_H_ */
