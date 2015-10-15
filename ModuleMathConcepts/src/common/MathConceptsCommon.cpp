/*
 * MathConceptsCommon.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: pham
 */

#include "MathConceptsCommon.h"

namespace mreal {
mreal_t norm(const ComplexVal& cVal) {
#if MPFR_REAL
	mreal_t real = cVal.real();
	mreal_t imag = cVal.imag();
	return mpfr::sqrt(real * real + imag * imag);
#else
	return std::sqrt(std::norm(cVal));
#endif
}

}


