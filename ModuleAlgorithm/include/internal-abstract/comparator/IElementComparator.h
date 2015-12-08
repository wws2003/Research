/*
 * IElementComparator.h
 *
 *  Created on: Dec 7, 2015
 *      Author: pham
 */

#ifndef IELEMENTCOMPARATOR_H_
#define IELEMENTCOMPARATOR_H_

template<typename T>
class IElementComparator {
public:
	virtual ~IElementComparator(){};

	virtual bool lessThan(const T& lhs, const T& rhs) = 0;
};

#endif /* IELEMENTCOMPARATOR_H_ */
