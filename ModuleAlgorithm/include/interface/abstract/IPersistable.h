/*
 * IPersistable.h
 *
 *  Created on: Apr 29, 2015
 *      Author: pham
 */

#ifndef IPERSISTABLE_H_
#define IPERSISTABLE_H_

#include <string>

class IPersistable {
public:

	virtual ~IPersistable(){};

	virtual void save(std::string fileName) = 0;

	virtual void load(std::string fileName) = 0;
};


#endif /* IPERSISTABLE_H_ */
