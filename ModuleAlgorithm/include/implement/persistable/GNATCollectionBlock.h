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
#include "IOCommon.h"
#include "IReader.h"
#include "IWriter.h"
#include <fstream>

typedef int GNATCollectionIdType;

template<typename T>
struct GNATCollectionWriteBlock {

	GNATCollectionIdType m_id;
	GNATCollectionIdType m_parentId;

	const SplitPointSet<T>* m_pSplitPoints;
	const UnstructuredBuffer<T>* m_pUnStructeredBuffer;
	const RangeMap* m_pSplitPointRanges;
};

template<typename T>
struct GNATCollectionReadBlock {

	GNATCollectionIdType m_id;
	GNATCollectionIdType m_parentId;

	SplitPointSet<T> m_splitPoints;
	UnstructuredBuffer<T> m_unStructeredBuffer;
	RangeMap m_splitPointRanges;
};

template<typename T>
void writeCollectionBlocks(const std::vector<GNATCollectionWriteBlock<T>* >& rBlocks, WriterPtr<T> pWriter, std::ostream& outputStream);

template<typename T>
void writeBlock(const GNATCollectionWriteBlock<T>* pWriteBlock, WriterPtr<T> pWriter, std::ostream& outputStream);

template<typename T>
void readCollectionBlocks(std::vector<GNATCollectionReadBlock<T>* >& rBlocks, ReaderPtr<T> pReader, std::ifstream& inputStream);

template<typename T>
void readBlock(GNATCollectionReadBlock<T>* pReadBlock, ReaderPtr<T> pReader, std::istream& inputStream);


#endif /* GNATCOLLECTIONBLOCK_H_ */
