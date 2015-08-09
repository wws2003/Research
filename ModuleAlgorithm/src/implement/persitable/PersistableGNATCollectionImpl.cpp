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

/*--------------------For saving--------------------*/
template<typename T>
void saveGNATCollection(GNATCollectionImpl<T>* pCollection, GNATCollectionIdType parentId, GNATCollectionIdType *pCurrentMaxId, WriterPtr<T> pWritter, std::ostream& outputStream);

template<typename T>
void fillWriteBlockFromCollection(const GNATCollectionImpl<T>* pCollection, GNATCollectionWriteBlock<T>* pWriteBlock);

template<typename T>
void writeBlock(const GNATCollectionWriteBlock<T>* pWriteBlock, WriterPtr<T> pWriter, std::ostream& outputStream);
/*---------------------------------------------------*/

/*--------------------For loading--------------------*/
template<typename T>
using GNATCollectionMap = std::map<GNATCollectionIdType, GNATCollectionImpl<T>* >;

template<typename T>
void loadGNATCollection(GNATCollectionImpl<T>* pCollection, int id, ReaderPtr<T> pReader, std::ifstream& inputStream);

template<typename T>
void readCollectionBlocks(std::vector<GNATCollectionReadBlock<T>* >& rBlocks, ReaderPtr<T> pReader, std::ifstream& inputStream);

template<typename T>
void fillCollectionFromReadBlock(const GNATCollectionReadBlock<T>* pReadBlock, GNATCollectionImpl<T>* pCollection);

template<typename T>
void readBlock(const GNATCollectionReadBlock<T>* pReadBlock, ReaderPtr<T> pReader, std::istream& inputStream);
/*---------------------------------------------------*/

template<typename T>
PersistableGNATCollectionImpl<T>::PersistableGNATCollectionImpl(WriterPtr<T> pWriter, ReaderPtr<T> pReader) : GNATCollectionImpl<T>() {
	m_pWriter = pWriter;
	m_pReader = pReader;
}

template<typename T>
void PersistableGNATCollectionImpl<T>::save(std::string fileName) {
	//Open file in binary write mode
	std::ofstream outputStream(fileName, std::ofstream::binary);

	if(outputStream.is_open()) {
		//Recursively save the collection, given parent id and id
		GNATCollectionIdType parentId = -1;
		GNATCollectionIdType currentMaxId = 0;
		saveGNATCollection(this, parentId, &currentMaxId, m_pWriter, outputStream);
	}
	else {
		throw std::runtime_error("Can't open file to save collection");
	}

	//Closing file output stream is not really necessary as it is called by destructor automatically
}

template<typename T>
void PersistableGNATCollectionImpl<T>::load(std::string fileName) {
	//Open file in binary write mode
	std::ifstream inputStream(fileName, std::ifstream::binary);

	if(inputStream.is_open()) {
		std::vector<GNATCollectionReadBlock<T>* > blocks;

		//Read blocks from input stream
		readCollectionBlocks(blocks, m_pReader, inputStream);

		//Close input stream soon after reading blocks
		inputStream.close();

		loadGNATCollection(this, 1, blocks);

		//Release read blocks
		for(typename std::vector<GNATCollectionReadBlock<T>* >::iterator bIter = blocks.begin(); bIter != blocks.end();) {
			GNATCollectionReadBlock<T>* pCollectionReadBlock = *bIter;
			delete pCollectionReadBlock;
			bIter = blocks.erase(bIter);
		}
	}
	else {
		throw std::runtime_error("Can't open file to load collection");
	}

	//Closing file output stream is not really necessary as it is called by destructor automatically
}

template<typename T>
void saveGNATCollection(GNATCollectionImpl<T>* pCollection, GNATCollectionIdType parentId, GNATCollectionIdType *pCurrentMaxId, WriterPtr<T> pWritter, std::ostream& outputStream) {
	GNATCollectionWriteBlock<T> block;

	//Fill data to the block
	block.m_id = ++*pCurrentMaxId;
	block.m_parentId = parentId;
	fillWriteBlockFromCollection(pCollection, &block);

	//Write the block to output stream
	writeBlock(&block, pWritter, outputStream);

	//Recursively make sub-blocks and write to output stream
	GNATCollectionIdType subBlockParentId = block.m_id;
	for(typename CollectionVector<T>::const_iterator cIter = pCollection->m_subCollections.begin(); cIter != pCollection->m_subCollections.end(); cIter++) {
		GNATCollectionImpl<T>* pSubCollection = (GNATCollectionImpl<T>*)*cIter;
		GNATCollectionWriteBlock<T> subBlock;

		saveGNATCollection(pSubCollection, subBlockParentId, *pCurrentMaxId, pWritter, outputStream);
	}
}

template<typename T>
void fillWriteBlockFromCollection(const GNATCollectionImpl<T>* pCollection, GNATCollectionWriteBlock<T>* pWriteBlock) {
	pWriteBlock->m_splitPointRanges = pCollection->m_splitPointRanges;
	pWriteBlock->m_splitPoints = pCollection->m_splitPoints;
	pWriteBlock->m_unStructeredBuffer = pCollection->m_unStructeredBuffer;
}

template<typename T>
void writeBlock(const GNATCollectionWriteBlock<T>* pWriteBlock, WriterPtr<T> pWriter, std::ostream& outputStream) {
	//Write collection id
	outputStream.write((const char*)&(pWriteBlock->m_id), sizeof(GNATCollectionIdType));

	//Write parent collection id
	outputStream.write((const char*)&(pWriteBlock->m_parentId), sizeof(GNATCollectionIdType));

	//Write split points of collection
	outputStream.write((const char*)&(pWriteBlock->m_splitPoints.size()), sizeof(size_t));
	for(typename SplitPointSet<T>::iterator eIter = pWriteBlock->m_splitPoints.begin(); eIter != pWriteBlock->m_splitPoints.end(); ) {
		T element = *eIter;
		pWriter->write(element, outputStream);
	}

	//Write unstructured of collection
	outputStream.write((const char*)&(pWriteBlock->m_unStructeredBuffer.size()), sizeof(size_t));
	for(typename UnstructuredBuffer<T>::iterator eIter = pWriteBlock->m_unStructeredBuffer.begin(); eIter != pWriteBlock->m_unStructeredBuffer.end(); ) {
		T element = *eIter;
		pWriter->write(element, outputStream);
	}

	//Write range map of collection
	//Write size of map
	outputStream.write((const char*)&(pWriteBlock->m_splitPointRanges.size()), sizeof(size_t));

	for(RangeMap::const_iterator rIter = pWriteBlock->m_splitPointRanges.begin(); rIter != pWriteBlock->m_splitPointRanges.end(); rIter++) {
		//Write each map element (which itself is a map)
		std::vector<Range>& range = *rIter;

		//Write size of sub-map
		size_t rangeSize = range.size();
		outputStream.write((const char*)&rangeSize, sizeof(size_t));

		for(std::vector<Range>::const_iterator dIter = range.begin(); dIter != range.end(); dIter++) {
			//Write each sub-map element
			double dMin = dIter->first;
			double dMax = dIter->second;
			outputStream.write((const char*)&dMin, sizeof(double));
			outputStream.write((const char*)&dMax, sizeof(double));
		}
	}
}

template<typename T>
void loadGNATCollection(GNATCollectionImpl<T>* pRootCollection, int id, const std::vector<GNATCollectionReadBlock<T>* >& blocks) {

	//Create collections and to map
	GNATCollectionMap<T> collectionMap;
	for(typename std::vector<GNATCollectionReadBlock<T>* >::const_iterator bIter = blocks.begin(); bIter != blocks.end(); bIter++) {
		GNATCollectionReadBlock<T>* pBlock = *bIter;
		int blockId = pBlock->m_id;

		GNATCollectionImpl<T>* pCollection = (blockId == id) ? pRootCollection : new GNATCollectionImpl<T>();
		fillCollectionFromReadBlock(pBlock, pCollection);
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
		GNATCollectionImpl<T>* pParentCollection = parentCollectionIter->second;

		//Get sub-collection from collection map
		int subCollectionId = pSubBlock->m_id;
		GNATCollectionImpl<T>* pSubCollection = collectionMap[subCollectionId];

		//Add sub-collection to parent collection
		pParentCollection->m_subCollections.push_back(pSubCollection);
	}
}

template<typename T>
void readCollectionBlocks(std::vector<GNATCollectionReadBlock<T>* >& rBlocks, ReaderPtr<T> pReader, std::ifstream& inputStream) {
	rBlocks.clear();
	//Read blocks from input stream
	while(!inputStream.eof()) {
		//Create new block
		GNATCollectionReadBlock<T>* pReadBlock = new GNATCollectionReadBlock<T>*();

		//Read block
		readBlock(pReadBlock, pReader, inputStream);

		//Add blocks to block vector
		rBlocks.push_back(pReadBlock);
	}
}


template<typename T>
void fillCollectionFromReadBlock(const GNATCollectionReadBlock<T>* pReadBlock, GNATCollectionImpl<T>* pCollection) {
	pCollection->m_splitPointRanges = pReadBlock->m_splitPointRanges;
	pCollection->m_splitPoints = pReadBlock->m_splitPoints;
	pCollection->m_unStructeredBuffer = pReadBlock->m_unStructeredBuffer;
}

template<typename T>
void readBlock(const GNATCollectionReadBlock<T>* pReadBlock, ReaderPtr<T> pReader, std::istream& inputStream) {
	//Read collection id
	inputStream.read((char*)&(pReadBlock->m_id), sizeof(GNATCollectionIdType));

	//Read parent collection id
	inputStream.read((char*)&(pReadBlock->m_parentId), sizeof(GNATCollectionIdType));

	//Read split points of collection
	size_t nbSplitPoints;
	inputStream.read((char*)&nbSplitPoints, sizeof(size_t));
	for(unsigned int i = 0; i < nbSplitPoints; i++) {
		T splitPoint;
		pReader->read(splitPoint, inputStream);
		pReadBlock->m_splitPoints.push_back(splitPoint);
	}

	//Read unstructured buffer of collection
	size_t unstructuredBufferSize;
	inputStream.read((char*)&unstructuredBufferSize, sizeof(size_t));
	for(unsigned int i = 0; i < unstructuredBufferSize; i++) {
		T element;
		pReader->read(element, inputStream);
		pReadBlock->m_unStructeredBuffer.push_back(element);
	}

	//Read range map of collection
	//Read size of map
	size_t nbSplitPointRanges;
	inputStream.read((char*)&nbSplitPointRanges, sizeof(size_t));

	for(unsigned int i = 0; i < nbSplitPointRanges; i++) {
		//Read each map element (which itself is a map)
		std::vector<Range> range;

		//Read size of sub-map
		size_t rangeSize;
		inputStream.read((char*)&rangeSize, sizeof(size_t));

		for(unsigned int i = 0; i < rangeSize; i++) {
			//Read each sub-map element
			double dMin;
			double dMax;
			inputStream.read((char*)&dMin, sizeof(double));
			inputStream.read((char*)&dMax, sizeof(double));
			range.push_back(Range(dMin, dMax));
		}

		pReadBlock->m_splitPointRanges.push_back(range);
	}
}


