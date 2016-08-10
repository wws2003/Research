/*
 * SimpleGateKDTree.h
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#ifndef SIMPLEGATEKDTREE_H_
#define SIMPLEGATEKDTREE_H_

#include "SimpleKDTree.h"
#include "Gate.h"

typedef SimpleKDTree<GatePtr, double> SimpleDoubleGateKDTree;
typedef SimpleKDTree<GatePtr, mreal_t> SimpleRealGateKDTree;

#endif /* SIMPLEGATEKDTREE_H_ */
