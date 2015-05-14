/*
 * IDistanceCalculator.h
 *
 *  Created on: May 12, 2015
 *      Author: pham
 */

#ifndef IDISTANCECALCULATOR_H_
#define IDISTANCECALCULATOR_H_

template<typename T>
class IDistanceCalculator {
public:
	virtual ~IDistanceCalculator();
	virtual double distance(T p1, T p2);
};



#endif /* IDISTANCECALCULATOR_H_ */
