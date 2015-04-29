/*
 * AlgoCommon.h
 *
 *  Created on: Apr 28, 2015
 *      Author: pham
 */

#ifndef ALGOCOMMON_H_
#define ALGOCOMMON_H_

#include "MathConceptsCommon.h"

typedef long MatrixCollectionSize_t;

template<typename T> class IIterator;
class IMatrixCollection;
class IPersistableMatrixCollection;

typedef IIterator<MatrixPtr> MatrixIterator;
typedef IIterator<VectorPtr> VectorIterator;

typedef MatrixIterator* MatrixIteratorPtr;
typedef VectorIterator* VectorItertatorPtr;

typedef IMatrixCollection& MatrixCollectionRef;
typedef IMatrixCollection* MatrixCollectionPtr;

typedef IPersistableMatrixCollection* PersitableMatrixCollectionPtr;

#endif /* ALGOCOMMON_H_ */
