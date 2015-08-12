/*
 * PersistableGNATCollectionImpl.h
 *
 *  Created on: Jul 25, 2015
 *      Author: pham
 */

#ifndef PERSISTABLEGNATCOLLECTIONIMPL_H_
#define PERSISTABLEGNATCOLLECTIONIMPL_H_

#include "GNATCollectionImpl.h"
#include "IPersistable.h"
#include "IOCommon.h"
#include "IWriter.h"
#include "GNATCollectionBlock.h"
#include "IReader.h"

template<typename T>
class  PersistableGNATCollectionImpl: public GNATCollectionImpl<T>, IPersistable {
public:
	PersistableGNATCollectionImpl(WriterPtr<T> pWriter, ReaderPtr<T> pReader);

	virtual ~PersistableGNATCollectionImpl(){};

	/**
	 * Generate an instance of PersistableGNATCollectionImpl for sub-collection in
	 * order to keep persistability
	 *
	 */
	virtual CollectionPtr<T> generateSubCollection();

	virtual void save(std::string fileName);

	virtual void load(std::string fileName);

	//Explicitly set data fields
	void init(const SplitPointSet<T>& splitPoints,
			const UnstructuredBuffer<T>& unStructeredBuffer,
			const RangeMap& splitPointRanges);

	//Add sub-collection
	void addSubCollection(CollectionPtr<T> pSubCollection);

	//Overload
	bool operator==(const PersistableGNATCollectionImpl<T>& rhs);

private:
	PersistableGNATCollectionImpl();

	//Methods for saving
	void saveGNATCollection(GNATCollectionIdType parentId, GNATCollectionIdType *pCurrentMaxId, std::vector<GNATCollectionWriteBlock<T>* >& rBlocks);

	void fillWriteBlockFromCollection(GNATCollectionIdType blockId, GNATCollectionIdType parentBlockId, GNATCollectionWriteBlock<T>* pWriteBlock) const;

	//Methods for loading
	void loadGNATCollection(int id, const std::vector<GNATCollectionReadBlock<T>* >& blocks);

	void fillCollectionFromReadBlock(const GNATCollectionReadBlock<T>* pReadBlock);

	//Method for comparing
	bool sameSplitPoints(const PersistableGNATCollectionImpl<T>& rhs);

	bool sameUnstructuredBuffer(const PersistableGNATCollectionImpl<T>& rhs);

	bool sameSplitPointsRange(const PersistableGNATCollectionImpl<T>& rhs);

	bool sameSubCollections(const PersistableGNATCollectionImpl<T>& rhs);

	//Private members
	WriterPtr<T> m_pWriter;
	ReaderPtr<T> m_pReader;
};



#endif /* PERSISTABLEGNATCOLLECTIONIMPL_H_ */
