/*
 * IConverter.h
 *
 *  Created on: Dec 8, 2015
 *      Author: pham
 */

#ifndef ICONVERTER_H_
#define ICONVERTER_H_

template<typename T1, typename T2>
class IConverter {
public:
	virtual ~IConverter(){};

	virtual void convert12(const T1& t1, T2& t2) = 0;

	virtual void convert21(const T2& t2, T1& t1) = 0;
};


#endif /* ICONVERTER_H_ */
