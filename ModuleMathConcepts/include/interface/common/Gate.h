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

class Gate {
public:
	Gate(MatrixPtr pMatrix, cost_t cost, std::string singleLabel);

	Gate(MatrixPtr pMatrix, cost_t cost, const LabelSeq& label);

	Gate(MatrixPtr pMatrix, cost_t cost, const LabelSeq& label, const std::string& labelStr);

	virtual ~Gate(){_destroy(m_pMatrix);};

	inline MatrixPtr getMatrix() {
		return m_pMatrix;
	}

	inline void setMatrix(MatrixPtr pMatrix) {
		m_pMatrix = pMatrix;
	}

	inline cost_t getCost() {
		return m_cost;
	}

	inline const LabelSeq& getLabelSeq() {
		return m_label;
	}

	inline const std::string& getLabelStr() {
		return m_labelStr;
	}

	virtual GatePtr clone() const;

	bool operator==(const Gate& rhs);

private:
	MatrixPtr m_pMatrix;
	cost_t m_cost;
	LabelSeq m_label;
	std::string m_labelStr;
};


#endif /* GATE_H_ */
