/*
 * ITimer.h
 *
 *  Created on: Apr 15, 2015
 *      Author: pham
 */

#ifndef ITIMER_H_
#define ITIMER_H_

class ITimer {
public:
	virtual ~ITimer(){};

	//Get current time. Different implementation should return different time such as wall time, cpu time
	virtual double getTimeInMs() = 0;
};


#endif /* ITIMER_H_ */
