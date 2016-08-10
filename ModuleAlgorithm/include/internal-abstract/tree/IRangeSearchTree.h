/*
 * IRangeSearchTree.h
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#ifndef IRANGESEARCHTREE_H_
#define IRANGESEARCHTREE_H_

#include "AlgoInternal.h"
#include "AlgoCommon.h"
#include "Coordinate.hpp"

template<typename E, typename Field>
class IRangeSearchTree {
public:
	virtual ~IRangeSearchTree(){};

	virtual void init() = 0;

	virtual void insert(Coordinate<E, Field> coordinate) = 0;

	virtual void query(const Coordinate<E, Field>& query,
			const std::vector<Field>& ranges,
			std::vector<Coordinate<E, Field> >& results) = 0;
};

#endif /* IRANGESEARCHTREE_H_ */
