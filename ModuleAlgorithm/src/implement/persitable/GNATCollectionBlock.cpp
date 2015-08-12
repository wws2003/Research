/*
 * GNATCollectionBlock.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: pham
 */
#include "GNATCollectionBlock.h"

template<typename T>
void writeCollectionBlocks(const std::vector<GNATCollectionWriteBlock<T>* >& rBlocks, WriterPtr<T> pWriter, std::ostream& outputStream) {

	//Write number of blocks
	size_t nbBlocks = rBlocks.size();
	outputStream.write((const char*)(&nbBlocks), sizeof(size_t));

	//Write blocks
	for(typename std::vector<GNATCollectionWriteBlock<T>* >::const_iterator bIter = rBlocks.begin(); bIter != rBlocks.end(); bIter++) {
		//Get new block
		GNATCollectionWriteBlock<T>* pWriteBlock = *bIter;

		//Write block
		writeBlock(pWriteBlock, pWriter, outputStream);
	}
}

template<typename T>
void writeBlock(const GNATCollectionWriteBlock<T>* pWriteBlock, WriterPtr<T> pWriter, std::ostream& outputStream) {
	//Write collection id
	outputStream.write((const char*)&(pWriteBlock->m_id), sizeof(GNATCollectionIdType));

	//Write parent collection id
	outputStream.write((const char*)&(pWriteBlock->m_parentId), sizeof(GNATCollectionIdType));

	//Write split points of collection
	size_t nbSplitPoints = pWriteBlock->m_pSplitPoints->size();
	outputStream.write((const char*)&(nbSplitPoints), sizeof(size_t));
	for(typename SplitPointSet<T>::const_iterator eIter = pWriteBlock->m_pSplitPoints->begin(); eIter != pWriteBlock->m_pSplitPoints->end(); eIter++) {
		T element = *eIter;
		pWriter->write(element, outputStream);
	}

	//Write unstructured of collection
	size_t nbUnstructuredBuffer = pWriteBlock->m_pUnStructeredBuffer->size();
	outputStream.write((const char*)&(nbUnstructuredBuffer), sizeof(size_t));
	for(typename UnstructuredBuffer<T>::const_iterator eIter = pWriteBlock->m_pUnStructeredBuffer->begin(); eIter != pWriteBlock->m_pUnStructeredBuffer->end(); eIter++) {
		T element = *eIter;
		pWriter->write(element, outputStream);
	}

	//Write range map of collection
	//Write size of map
	size_t nbSplitPointRanges = pWriteBlock->m_pSplitPointRanges->size();
	outputStream.write((const char*)&(nbSplitPointRanges), sizeof(size_t));

	for(RangeMap::const_iterator rIter = pWriteBlock->m_pSplitPointRanges->begin(); rIter != pWriteBlock->m_pSplitPointRanges->end(); rIter++) {
		//Write each map element (which itself is a map)
		std::vector<Range> range = *rIter;

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
void readCollectionBlocks(std::vector<GNATCollectionReadBlock<T>* >& rBlocks, ReaderPtr<T> pReader, std::ifstream& inputStream) {
	rBlocks.clear();

	//Read number of blocks
	size_t nbBlocks;
	inputStream.read((char*)&nbBlocks, sizeof(size_t));

	//Read blocks from input stream
	unsigned int blockCounter = 0;

	while(++blockCounter <= nbBlocks) {
		//Create new block
		GNATCollectionReadBlock<T>* pReadBlock = new GNATCollectionReadBlock<T>();

		//Read block
		readBlock(pReadBlock, pReader, inputStream);

		//Add blocks to block vector
		rBlocks.push_back(pReadBlock);
	}
}

template<typename T>
void readBlock(GNATCollectionReadBlock<T>* pReadBlock, ReaderPtr<T> pReader, std::istream& inputStream) {
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
