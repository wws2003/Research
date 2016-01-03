/*
 * GBin.hpp
 *
 *  Created on: Nov 3, 2015
 *      Author: pham
 */

#ifndef GBIN_HPP_
#define GBIN_HPP_

#include "MathConceptsCommon.h"
#include "AlgoInternal.h"
#include "OperatorCommon.h"
#include "IDistanceCalculator.h"
#include <vector>
#include <string>

template<typename T, typename IdentifierType>
class GBin {
public:
	GBin(IdentifierType identifier) {
		m_identifier = identifier;
	};
	virtual ~GBin(){};

	inline void addElement(T element){m_elements.push_back(element);};

	inline void clear(){m_elements.clear();};

	inline const std::vector<T>& getElements(){return m_elements;};

private:
	IdentifierType m_identifier;
	std::vector<T> m_elements;
};



#endif /* GBIN_HPP_ */
