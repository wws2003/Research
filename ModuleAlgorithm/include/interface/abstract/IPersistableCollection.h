/*
 * IPersistableCollection.h
 *
 *  Created on: Oct 26, 2015
 *      Author: pham
 */

#ifndef IPERSISTABLECOLLECTION_H_
#define IPERSISTABLECOLLECTION_H_

#include "AlgoCommon.h"
#include "IPersistable.h"
#include "ICollection.h"
#include "Gate.h"
#include "IMatrix.h"

template<typename T>
class IPersistableCollection : public ICollection<T>, public IPersistable {

};

typedef IPersistableCollection<GatePtr> PersistableGateCollection;
typedef IPersistableCollection<MatrixPtr> PersistableMatrixCollection;

template class IPersistableCollection<GatePtr>;
template class IPersistableCollection<MatrixPtr>;

#endif /* IPERSISTABLECOLLECTION_H_ */
