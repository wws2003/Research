/*
 * ComplexValIO.h
 *
 *  Created on: Oct 14, 2015
 *      Author: pham
 */

#ifndef COMPLEXVALIO_H_
#define COMPLEXVALIO_H_

#include "MathConceptsCommon.h"
#include "IOCommon.h"
#include <ios>

//Read complex value from input stream
void readComplexVal(std::istream &inputStream, ComplexVal* pCVal);

//Read from input stream pending of written complex value, in order to continue to read data other format
void endOfReadComplexVals(std::istream &inputStream);

//Write complex value to output stream
void writeComplexValue(const ComplexVal& cVal, std::ostream& outputStream);

//Read real value from input stream
void readRealValue(std::istream &inputStream, mreal_t* pRVal);

//Write real value to output stream
void writeRealValue(const mreal_t& rVal, std::ostream& outputStream);

#endif /* COMPLEXVALIO_H_ */
