/*
 * TwoPivotsSetImpl.h
 *
 *  Created on: Jul 17, 2016
 *      Author: pham
 */

#ifndef TWOPIVOTSSETIMPL_H_
#define TWOPIVOTSSETIMPL_H_

#include "ISet.h"
#include "IDistanceCalculator.h"
#include <map>

template<typename T>
class TwoPivotsSetImpl: public ISet<T> {
public:
	TwoPivotsSetImpl(DistanceCalculatorPtr<T> pDistanceCalculator);

	virtual ~TwoPivotsSetImpl();

	//Override
	bool isUnique(T element);

	//Override
	void addElement(T element);

	//Override
	void getAllUniqueElements(std::vector<T>& elements);

private:
	bool arePivotReady() const;
	void distanceToPivots(T element, mreal_t& d1, mreal_t& d2) const;

	void addElementWhenPivotsReady(T element);

	void updatePivots(T pivot);

	typedef std::map<mreal_t, std::vector<T>* > ElementDistanceMap;
	typedef ElementDistanceMap* ElementDistanceMapPtr;
	typedef std::map<mreal_t, ElementDistanceMapPtr> ElementDistance2DTable;

	ElementDistance2DTable m_distanceTable;

	T m_pivot1;
	T m_pivot2;

	DistanceCalculatorPtr<T> m_pDistanceCalculator;
};



#endif /* TWOPIVOTSSETIMPL_H_ */
