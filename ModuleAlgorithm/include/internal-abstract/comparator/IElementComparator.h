/*
 * IElementComparator.h
 *
 *  Created on: Dec 7, 2015
 *      Author: pham
 */

#ifndef IELEMENTCOMPARATOR_H_
#define IELEMENTCOMPARATOR_H_

#include "AlgoInternal.h"

template<typename T>
class IElementComparator {
public:
	virtual ~IElementComparator(){};

	virtual bool lessThan(const T& lhs, const T& rhs) = 0;
};

template<typename T>
struct WrapperComparator {
	WrapperComparator(ComparatorPtr<T> pElementComparator) {
		m_pElementComparator = pElementComparator;
	}
	bool operator()(const T& lhs, const T& rhs)
	{
		return m_pElementComparator->lessThan(lhs, rhs);
	}
	ComparatorPtr<T> m_pElementComparator;
};

#endif /* IELEMENTCOMPARATOR_H_ */
