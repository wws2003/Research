/*
 * ComplexValIO.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: pham
 */

#include "ComplexValIO.h"

void readComplexVal(std::istream &inputStream, ComplexVal* pCVal) {
#if MPFR_REAL
	int defaultPrecision = inputStream.precision();
	//Increase precision of output stream for mpfr
	inputStream.precision(mpfr::mpreal::get_default_prec());
	mreal_t rVal;
	inputStream >> rVal;
	mreal_t iVal;
	inputStream >> iVal;
	pCVal->real(rVal);
	pCVal->imag(iVal);
	//Restore default precision of output stream
	inputStream.precision(defaultPrecision);

#else
	inputStream.read((char*)(pCVal), sizeof(ComplexVal));
#endif
}

void endOfReadComplexVals(std::istream &inputStream) {
#if MPFR_REAL
	//Read one delimeter character
	char delimeter;
	inputStream.get(delimeter);
#else
	//Do nothing
#endif
}

void writeComplexValue(const ComplexVal& cVal, std::ostream& outputStream) {
#if MPFR_REAL

	int defaultPrecision = outputStream.precision();
	//Increase precision of output stream for mpfr
	outputStream.precision(mpfr::mpreal::get_default_prec());
	outputStream << cVal.real() << " " << cVal.imag() << " ";
	//Restore default precision of output stream
	outputStream.precision(defaultPrecision);

#else
	outputStream.write((const char*)(&cVal), sizeof(ComplexVal));
#endif
}

void readRealValue(std::istream &inputStream, mreal_t* pRVal) {
#if MPFR_REAL
	int defaultPrecision = inputStream.precision();
	//Increase precision of output stream for mpfr
	inputStream.precision(mpfr::mpreal::get_default_prec());
	mreal_t rVal;
	inputStream >> rVal;
	*pRVal = rVal;
	//Restore default precision of output stream
	inputStream.precision(defaultPrecision);
#else
	inputStream.read((char*)pRVal, sizeof(mreal_t));
#endif
}

//Write real value to output stream
void writeRealValue(const mreal_t& rVal, std::ostream& outputStream) {
#if MPFR_REAL

	int defaultPrecision = outputStream.precision();
	//Increase precision of output stream for mpfr
	outputStream.precision(mpfr::mpreal::get_default_prec());
	outputStream << rVal << " ";
	//Restore default precision of output stream
	outputStream.precision(defaultPrecision);

#else
	outputStream.write((const char*)&rVal, sizeof(mreal_t));
#endif
}



