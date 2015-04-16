/*
 * IIterator.h
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#ifndef IITERATOR_H_
#define IITERATOR_H_

template<typename T>
class IIterator {
public:
	virtual ~IIterator(){};

	virtual void toBegin() = 0;

	virtual void next() = 0;

	virtual void prev() = 0;

	virtual bool isDone() = 0;

	virtual T getObj() = 0;
};

#endif /* IITERATOR_H_ */
