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

	virtual ~Gate();

	MatrixPtr getMatrix();

	cost_t getCost();

	const label_t& getLabel();

private:
	MatrixPtr m_pMatrix;
	cost_t m_cost;
	label_t m_label;
};


#endif /* GATE_H_ */
