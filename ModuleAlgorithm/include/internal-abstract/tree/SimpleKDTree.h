/*
 * SimpleKDTree.h
 *
 *  Created on: Jul 18, 2016
 *      Author: pham
 */

#ifndef SIMPLEKDTREE_H_
#define SIMPLEKDTREE_H_

#include "IRangeSearchTree.h"
#include "AlgoInternal.h"

template<typename T, typename F>
class SimpleKDTree: public IRangeSearchTree<T, F> {
public:
	SimpleKDTree(int dimension, F rootCoord);
	virtual ~SimpleKDTree();

	//Override
	void init();

	//Override
	void insert(Coordinate<T, F> coordinate);

	//Override
	void query(const Coordinate<T, F>& query,
			const std::vector<F>& ranges,
			std::vector<Coordinate<T, F> >& results);

private:
	SimpleKDTree(Coordinate<T, F> dataCoord, int compareIndex);

	bool isMatched(const Coordinate<T, F>& query, const std::vector<F>& ranges);

	Coordinate<T, F> m_dataCoord;
	SimpleKDTree<T, F> *m_pLeftChild;
	SimpleKDTree<T, F> *m_pRightChild;
	int m_compareIndex;
};



#endif /* SIMPLEKDTREE_H_ */
