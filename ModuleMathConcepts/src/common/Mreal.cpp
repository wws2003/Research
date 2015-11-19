/*
 * Mreal.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: pham
 */

#include "Mreal.h"

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

mreal_t cos(const mreal_t& angle) {
#if MPFR_REAL
	return mpfr::cos(angle);
#else
	return std::cos(angle);
#endif
}

mreal_t sin(const mreal_t& angle) {
#if MPFR_REAL
	return mpfr::sin(angle);
#else
	return std::sin(angle);
#endif
}

mreal_t atan2(const mreal_t& y, const mreal_t& x) {
#if MPFR_REAL
	return mpfr::atan2(y, x);
#else
	return mpfr::atan2(y, x);
#endif
}

}


