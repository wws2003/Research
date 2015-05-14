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

	//Go to the beginning (point to first element)
	virtual void toBegin() = 0;

	//Go to next element pointer
	virtual void next() = 0;

	//Go to previous pointer
	virtual void prev() = 0;

	//Check if the last element has been checked
	virtual bool isDone() = 0;

	//Get currently pointed element
	virtual T getObj() = 0;
};

#endif /* IITERATOR_H_ */
