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
#include "IReader.h"

template<typename T>
class  PersistableGNATCollectionImpl: public GNATCollectionImpl<T>, IPersistable {
public:
	PersistableGNATCollectionImpl(WriterPtr<T> pWriter, ReaderPtr<T> pReader);

	virtual ~PersistableGNATCollectionImpl(){};

	virtual void save(std::string fileName);

	virtual void load(std::string fileName);

private:
	WriterPtr<T> m_pWriter;
	ReaderPtr<T> m_pReader;
};



#endif /* PERSISTABLEGNATCOLLECTIONIMPL_H_ */
