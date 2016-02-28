/*
 * Coordinate.h
 *
 *  Created on: May 18, 2015
 *      Author: pham
 */

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "MathConceptsCommon.h"
#include <vector>
#include <algorithm>

template<typename E, typename Field>
class Coordinate {
public:

	Coordinate(){m_element = NullPtr;};

	Coordinate(E element, const std::vector<Field>& coordinates):m_element(element){
		m_coordinates.reserve(coordinates.size());
		m_coordinates.insert(m_coordinates.end(), coordinates.begin(), coordinates.end());
	};

	virtual ~Coordinate(){};

	const std::vector<Field>& getCoordinates() const {return m_coordinates;};

	E getElement() const {return m_element;}

	inline void setElement(E element){m_element = element;};

	Coordinate<E, Field> operator+(const Coordinate<E, Field>& rhs) const {
		std::vector<Field> coordinates;
		int nbDimensional = m_coordinates.size();

		coordinates.reserve(nbDimensional);
		for(unsigned int i = 0; i < nbDimensional; i++) {
			coordinates.push_back(m_coordinates[i] + rhs.m_coordinates[i]);
		}
		return Coordinate<E, Field>(NullPtr, coordinates);
	}

	Coordinate<E, Field> operator-(const Coordinate<E, Field>& rhs) const {
		std::vector<Field> coordinates;
		int nbDimensional = m_coordinates.size();

		coordinates.reserve(nbDimensional);
		for(unsigned int i = 0; i < nbDimensional; i++) {
			coordinates.push_back(m_coordinates[i] - rhs.m_coordinates[i]);
		}
		return Coordinate<E, Field>(NullPtr, coordinates);
	}

	Coordinate<E, Field>& operator+=(const Coordinate<E, Field>& rhs) {
		int nbDimensional = m_coordinates.size();

		for(unsigned int i = 0; i < nbDimensional; i++) {
			m_coordinates[i] += rhs.m_coordinates[i];
		}
		//NOTE: This operator doesn't automatically update the element field if this instance
		return *this;
	}

	Coordinate<E, Field>& operator-=(const Coordinate<E, Field>& rhs) {
		int nbDimensional = m_coordinates.size();

		for(unsigned int i = 0; i < nbDimensional; i++) {
			m_coordinates[i] -= rhs.m_coordinates[i];
		}
		//NOTE: This operator doesn't automatically update the element field if this instance
		return *this;
	}

	void clear() {
		m_coordinates.clear();
		_destroy(m_element);
		m_element = NullPtr;
	}

	mreal_t getNorm() const {
		mreal_t sqrNorm = 0.0;
		std::for_each(m_coordinates.begin(), m_coordinates.end(), [&sqrNorm](Field coord){sqrNorm += coord*coord;});
		if(sqrNorm > 0.0) {
			return mreal::sqrt(sqrNorm);
		}
		return 0.0;
	}

private:
	std::vector<Field> m_coordinates;
	E m_element;
};

#endif /* COORDINATE_H_ */
