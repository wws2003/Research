/*
 * IPersistableMatrixCollection.h
 *
 *  Created on: Apr 10, 2015
 *      Author: pham
 */

#ifndef IPERSISTABLEMATRIXCOLLECTION_H_
#define IPERSISTABLEMATRIXCOLLECTION_H_

#include <string>
#include "IPersistableMatrixCollection.h"

class IPersistableMatrixCollection: public IMatrixCollection {
public:
	virtual ~IPersistableMatrixCollection(){};

	virtual void save(std::string fileName) = 0;

	virtual void load(std::string fileName) = 0;
};

#endif /* IPERSISTABLEMATRIXCOLLECTION_H_ */
