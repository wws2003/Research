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

typedef unsigned int cost_t;

class Gate {
public:
	Gate(MatrixPtr pMatrix, cost_t cost, std::string label);

	virtual ~Gate();

	MatrixPtr getMatrix();

	cost_t getCost();

	std::string getLabel();

private:
	MatrixPtr m_pMatrix;
	cost_t m_cost;
	std::string m_label;
};


#endif /* GATE_H_ */
