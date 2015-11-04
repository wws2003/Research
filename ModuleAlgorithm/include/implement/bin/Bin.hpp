/*
 * Bin.h
 *
 *  Created on: Jun 18, 2015
 *      Author: pham
 */

#ifndef BIN_H_
#define BIN_H_

#include "MathConceptsCommon.h"
#include "AlgoInternal.h"
#include <vector>
#include <string>

typedef std::string BinBinaryPattern;

template<typename T>
class Bin {
public:
	Bin(BinBinaryPattern pattern){m_pattern = pattern;};
	virtual ~Bin(){};

	inline void addElement(T element){m_elements.push_back(element);};

	inline void clear(){m_elements.clear();};

	int distance(const Bin& otherBin) const;

	inline const std::vector<T>& getElements(){return m_elements;};

	inline const BinBinaryPattern& getPattern(){return m_pattern;};

private:
	BinBinaryPattern m_pattern;
	std::vector<T> m_elements;
};

#endif /* BIN_H_ */
