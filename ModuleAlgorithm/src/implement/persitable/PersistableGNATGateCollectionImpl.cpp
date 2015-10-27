/*
 * PersistableGNATGateCollectionImpl.cpp
 *
 *  Created on: Aug 11, 2015
 *      Author: pham
 */

#include "PersistableGNATGateCollectionImpl.h"
#include "PersistableGNATCollectionImpl.cpp"
#include "GNATCollectionBlock.cpp"

template class PersistableGNATCollectionImpl<GatePtr>;
template class PersistableGNATItemCollectionImpl<GatePtr>;

