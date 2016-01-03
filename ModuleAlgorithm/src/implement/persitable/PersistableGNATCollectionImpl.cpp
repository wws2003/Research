/*
 * PersistableGNATCollectionImpl.cpp
 *
 *  Created on: Jul 25, 2015
 *      Author: pham
 */

#include "PersistableGNATCollectionImpl.h"
#include "GNATCollectionBlock.h"
#include <fstream>
#include <stdexcept>
#include <exception>
#include <map>

template<typename EPtr>
void releasePointerVector(std::vector<EPtr> vect);

/*--------------------For loading--------------------*/
template<typename T>
using GNATCollectionMap = std::map<GNATCollectionIdType, PersistableGNATCollectionImpl<T>* >;
/*---------------------------------------------------*/

template<typename T>
PersistableGNATCollectionImpl<T>::PersistableGNATCollectionImpl(WriterPtr<T> pWriter, ReaderPtr<T> pReader, LookupResultProcessorPtr<T> pLookupResultProcessor) : GNATCollectionImpl<T>(pLookupResultProcessor, true) {
	m_pWriter = pWriter;
	m_pReader = pReader;
}

template<typename T>
PersistableGNATCollectionImpl<T>::PersistableGNATCollectionImpl() : GNATCollectionImpl<T>(NullPtr, false) {
	m_pWriter = NullPtr;
	m_pReader = NullPtr;
}

template<typename T>
GNATCollectionImplPtr<T> PersistableGNATCollectionImpl<T>::generateSubCollection() {
	return new PersistableGNATCollectionImpl<T>();
}

template<typename T>
void PersistableGNATCollectionImpl<T>::save(std::string fileName) {
	GNATCollectionIdType parentId = -1;
	GNATCollectionIdType currentMaxId = 0;
	std::vector<GNATCollectionWriteBlock<T>* > blocks;

	//Construct blocks from collection (Recursively save the collection, given parent id and id)
	saveGNATCollection(parentId, &currentMaxId, blocks);

	//Open file in binary write mode
	std::ofstream outputStream(fileName,  std::ofstream::out | std::ofstream::binary);
	//Write blocks to files
	if(outputStream.is_open()) {
		writeCollectionBlocks(blocks, m_pWriter, outputStream);
	}
	else {
		throw std::runtime_error("Can't open file to save collection");
	}

	//Release write blocks
	releasePointerVector(blocks);

	//Closing file output stream is not really necessary as it is called by destructor automatically
}

template<typename T>
void PersistableGNATCollectionImpl<T>::load(std::string fileName) {
	//Open file in binary write mode
	std::ifstream inputStream(fileName, std::ifstream::in | std::ifstream::binary);

	if(inputStream.is_open()) {
		//Self-emptize
		GNATCollectionImpl<T>::purge();

		std::vector<GNATCollectionReadBlock<T>* > blocks;

		//Read blocks from input stream
		readCollectionBlocks(blocks, m_pReader, inputStream);

		//Close input stream soon after reading blocks
		inputStream.close();

		loadGNATCollection(1, blocks);

		//Release read blocks
		releasePointerVector(blocks);
	}
	else {
		throw std::runtime_error("Can't open file to load collection");
	}

	//Closing file output stream is not really necessary as it is called by destructor automatically
}

template<typename T>
void PersistableGNATCollectionImpl<T>::init(const SplitPointSet<T>& splitPoints,
		const UnstructuredBuffer<T>& unStructeredBuffer,
		const RangeMap& splitPointRanges) {

	GNATCollectionImpl<T>::m_splitPoints = splitPoints;
	GNATCollectionImpl<T>::m_unStructeredBuffer = unStructeredBuffer;
	GNATCollectionImpl<T>::m_splitPointRanges = splitPointRanges;
}

template<typename T>
void PersistableGNATCollectionImpl<T>::addSubCollection(GNATCollectionImplPtr<T> pSubCollection) {
	GNATCollectionImpl<T>::m_subCollections.push_back(pSubCollection);
}

template<typename T>
void PersistableGNATCollectionImpl<T>::saveGNATCollection(GNATCollectionIdType parentId, GNATCollectionIdType *pCurrentMaxId, std::vector<GNATCollectionWriteBlock<T>* >& rBlocks) {
	GNATCollectionWriteBlock<T>* pBlock = new GNATCollectionWriteBlock<T>();

	//Fill data to the block
	GNATCollectionIdType nextId = ++*pCurrentMaxId;
	fillWriteBlockFromCollection(nextId, parentId, pBlock);

	//Write the block to output stream
	rBlocks.push_back(pBlock);

	//Recursively make sub-blocks and write to output stream
	GNATCollectionIdType subBlockParentId = pBlock->m_id;
	for(typename GNATCollectionVector<T>::const_iterator cIter = GNATCollectionImpl<T>::m_subCollections.begin(); cIter != GNATCollectionImpl<T>::m_subCollections.end(); cIter++) {
		PersistableGNATCollectionImpl<T>* pSubCollection = dynamic_cast<PersistableGNATCollectionImpl<T>*>(*cIter);
		pSubCollection->saveGNATCollection(subBlockParentId, pCurrentMaxId, rBlocks);
	}
}

template<typename T>
void PersistableGNATCollectionImpl<T>::fillWriteBlockFromCollection(GNATCollectionIdType blockId, GNATCollectionIdType parentBlockId, GNATCollectionWriteBlock<T>* pWriteBlock) const {
	pWriteBlock->m_id = blockId;
	pWriteBlock->m_parentId = parentBlockId;
	pWriteBlock->m_pSplitPointRanges = &(GNATCollectionImpl<T>::m_splitPointRanges);
	pWriteBlock->m_pSplitPoints = &(GNATCollectionImpl<T>::m_splitPoints);
	pWriteBlock->m_pUnStructeredBuffer = &(GNATCollectionImpl<T>::m_unStructeredBuffer);
}

template<typename T>
void PersistableGNATCollectionImpl<T>::loadGNATCollection(int id, const std::vector<GNATCollectionReadBlock<T>* >& blocks) {

	//Create collections and to map
	GNATCollectionMap<T> collectionMap;
	for(typename std::vector<GNATCollectionReadBlock<T>* >::const_iterator bIter = blocks.begin(); bIter != blocks.end(); bIter++) {
		GNATCollectionReadBlock<T>* pBlock = *bIter;
		int blockId = pBlock->m_id;

		PersistableGNATCollectionImpl<T>* pCollection = (blockId == id) ? this : dynamic_cast<PersistableGNATCollectionImpl<T>*>(generateSubCollection());
		pCollection->fillCollectionFromReadBlock(pBlock);
		collectionMap[blockId] = pCollection;
	}

	//Construct collection
	for(typename std::vector<GNATCollectionReadBlock<T>* >::const_iterator bIter = blocks.begin(); bIter != blocks.end(); bIter++) {
		GNATCollectionReadBlock<T>* pSubBlock = *bIter;

		//Get parent collection from collection map
		int parentBlockId = pSubBlock->m_parentId;
		typename GNATCollectionMap<T>::iterator parentCollectionIter = collectionMap.find(parentBlockId);
		if(parentCollectionIter == collectionMap.end()) {
			continue;
		}
		PersistableGNATCollectionImpl<T>* pParentCollection = parentCollectionIter->second;

		//Get sub-collection from collection map
		int subCollectionId = pSubBlock->m_id;
		GNATCollectionImpl<T>* pSubCollection = collectionMap[subCollectionId];

		//Add sub-collection to parent collection
		pParentCollection->addSubCollection(pSubCollection);
	}
}

template<typename T>
void PersistableGNATCollectionImpl<T>::fillCollectionFromReadBlock(const GNATCollectionReadBlock<T>* pReadBlock) {
	init(pReadBlock->m_splitPoints,
			pReadBlock->m_unStructeredBuffer,
			pReadBlock->m_splitPointRanges);
}

template<typename T>
bool PersistableGNATCollectionImpl<T>::operator==(const PersistableGNATCollectionImpl<T>& rhs) {

	return (sameSplitPoints(rhs) &&
			sameSplitPointsRange(rhs) &&
			sameSubCollections(rhs) &&
			sameUnstructuredBuffer(rhs));
}

template<typename T>
bool PersistableGNATCollectionImpl<T>::sameSplitPoints(const PersistableGNATCollectionImpl<T>& rhs) {
	size_t nbSplitPointsLhs = GNATCollectionImpl<T>::m_splitPoints.size();
	size_t nbSplitPointsRhs = rhs.m_splitPoints.size();

	if(nbSplitPointsLhs != nbSplitPointsRhs) {
		return false;
	}

	for(unsigned int i = 0; i < nbSplitPointsLhs; i++) {
		if(!(*(GNATCollectionImpl<T>::m_splitPoints[i]) == *(rhs.m_splitPoints[i]))) {
			return false;
		}
	}
	return true;
}

template<typename T>
bool PersistableGNATCollectionImpl<T>::sameUnstructuredBuffer(const PersistableGNATCollectionImpl<T>& rhs) {
	size_t nbUnstructuredBufferLhs = GNATCollectionImpl<T>::m_unStructeredBuffer.size();
	size_t nbUnstructuredBufferRhs = rhs.m_unStructeredBuffer.size();

	if(nbUnstructuredBufferLhs != nbUnstructuredBufferRhs) {
		return false;
	}

	typename UnstructuredBuffer<T>::const_iterator lIter = GNATCollectionImpl<T>::m_unStructeredBuffer.begin();
	typename UnstructuredBuffer<T>::const_iterator rIter = rhs.m_unStructeredBuffer.begin();

	for(;lIter != GNATCollectionImpl<T>::m_unStructeredBuffer.end(), rIter != rhs.m_unStructeredBuffer.end();
			lIter++, rIter++) {
		if(!(*(*rIter) == *(*lIter))) {
			return false;
		}
	}
	return true;
}

template<typename T>
bool PersistableGNATCollectionImpl<T>::sameSplitPointsRange(const PersistableGNATCollectionImpl<T>& rhs) {
#if MPFR_REAL
	mreal_t errorThreshold = 1e-30;
	size_t lRangeMapSize = GNATCollectionImpl<T>::m_splitPointRanges.size();
	size_t rRangeMapSize = rhs.m_splitPointRanges.size();
	if(lRangeMapSize != rRangeMapSize) {
		return false;
	}
	for(unsigned int i = 0; i < lRangeMapSize; i++) {
		std::vector<Range> lRanges = GNATCollectionImpl<T>::m_splitPointRanges[i];
		std::vector<Range> rRanges = rhs.m_splitPointRanges[i];
		size_t lRangeSize = lRanges.size();
		size_t rRangeSize = rRanges.size();
		if(lRangeSize != rRangeSize) {
			return false;
		}
		for(unsigned int j = 0; j < lRangeSize; j++) {
			Range lRange = lRanges[j];
			Range rRange = rRanges[j];
			if(mreal::abs(lRange.first - rRange.first) > errorThreshold
					|| mreal::abs(lRange.second - rRange.second) > errorThreshold) {
				return false;
			}
		}
	}
	return true;
#else
	return GNATCollectionImpl<T>::m_splitPointRanges == rhs.m_splitPointRanges;
#endif
}

template<typename T>
bool PersistableGNATCollectionImpl<T>::sameSubCollections(const PersistableGNATCollectionImpl<T>& rhs) {
	size_t nbSubCollectionsLhs = GNATCollectionImpl<T>::m_subCollections.size();
	size_t nbSubCollectionsRhs = rhs.m_subCollections.size();

	if(nbSubCollectionsLhs != nbSubCollectionsRhs) {
		return false;
	}

	for(unsigned int i = 0; i < nbSubCollectionsLhs; i++) {
		PersistableGNATCollectionImpl<T>* pLhsSubCollection = dynamic_cast<PersistableGNATCollectionImpl<T>*>(GNATCollectionImpl<T>::m_subCollections[i]);
		PersistableGNATCollectionImpl<T>* pRhsSubCollection = dynamic_cast<PersistableGNATCollectionImpl<T>*>(rhs.m_subCollections[i]);
		if(!(*pLhsSubCollection == *pRhsSubCollection)) {
			return false;
		}
	}
	return true;
}

template<typename EPtr>
void releasePointerVector(std::vector<EPtr> vect) {
	for(typename std::vector<EPtr>::iterator bIter = vect.begin(); bIter != vect.end();) {
		EPtr pE = *bIter;
		_destroy(pE);
		bIter = vect.erase(bIter);
	}
}
