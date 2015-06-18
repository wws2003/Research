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

template<typename T>
class Bin {
public:
	Bin(BinPattern pattern){m_pattern = pattern;};
	virtual ~Bin(){};

	inline void addElement(T element){m_elements.push_back(element);};

	inline void clear(){m_elements.clear();};

	int distance(const Bin& otherBin) const;

	inline const std::vector<T>& getElements(){return m_elements;};

private:
	BinPattern m_pattern;
	std::vector<T> m_elements;
};

template<typename T>
int Bin<T>::distance(const Bin<T>& otherBin) const {
	int distance = m_pattern.size();
	for(unsigned int i = 0; i < m_pattern.size(); i++) {
		if(m_pattern[i] != otherBin.m_pattern[i]) {
			distance--;
		}
	}
	return distance;
}


#endif /* BIN_H_ */
