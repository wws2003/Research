/*
 * GNATCollectionBlock.h
 *
 *  Created on: Jul 25, 2015
 *      Author: pham
 */

#ifndef GNATCOLLECTIONBLOCK_H_
#define GNATCOLLECTIONBLOCK_H_

#include <vector>
#include "GNATCollectionImpl.h"

typedef int GNATCollectionIdType;

template<typename T>
struct GNATCollectionWriteBlock {

	GNATCollectionIdType m_id;
	GNATCollectionIdType m_parentId;

	const SplitPointSet<T>& m_splitPoints;
	const UnstructuredBuffer<T>& m_unStructeredBuffer;
	const RangeMap& m_splitPointRanges;
};

template<typename T>
struct GNATCollectionReadBlock {

	GNATCollectionIdType m_id;
	GNATCollectionIdType m_parentId;

	SplitPointSet<T> m_splitPoints;
	UnstructuredBuffer<T> m_unStructeredBuffer;
	RangeMap m_splitPointRanges;
};

#endif /* GNATCOLLECTIONBLOCK_H_ */
