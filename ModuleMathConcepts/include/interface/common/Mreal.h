/*
 * Mreal.h
 *
 *  Created on: Oct 28, 2015
 *      Author: pham
 */

#ifndef MREAL_H_
#define MREAL_H_

#include "mpreal.h"
#include <cmath>
#include <cfloat>

#define MPFR_REAL 1

#if MPFR_REAL
typedef mpfr::mpreal mreal_t;
#define REAL_IO_PRECISION 32
#else
typedef double mreal_t;
#endif

typedef std::complex<mreal_t> ComplexVal;

namespace mreal {

inline void initPrecision(int defaultPrecision = 256) {
#if MPFR_REAL
	mpfr::mpreal::set_default_prec(defaultPrecision);
#else
	//DO nothing
#endif
}

inline void initRand() {
#if MPFR_REAL
	mpfr::random(time(NULL));
#else
	srand(time(NULL));
#endif
}

inline mreal_t abs(const mreal_t& v) {
#if MPFR_REAL
	return mpfr::abs(v);
#else
	return std::abs(v);
#endif
}

inline mreal_t sqrt(const mreal_t& v) {
#if MPFR_REAL
	return mpfr::sqrt(v);
#else
	return std::sqrt(v);
#endif
}

inline double toDouble(const mreal_t& v) {
#if MPFR_REAL
	return v.toDouble();
#else
	return v;
#endif
}

inline bool isNAN(const mreal_t& v) {
#if MPFR_REAL
	return mpfr::isnan(v);
#else
	return std::isnan(v);
#endif
}

mreal_t norm(const ComplexVal& v);

mreal_t cos(const mreal_t& angle);
mreal_t sin(const mreal_t& angle);
mreal_t atan2(const mreal_t& y, const mreal_t& x);

//Random number in [0, 1]
mreal_t rand();
}
// namespace mreal


#endif /* MREAL_H_ */
