/*
 * GateSpecification.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pham
 */

#ifndef GATESPECIFICATION_H_
#define GATESPECIFICATION_H_

#include "ResourcesCommon.h"

#define _CV(x, y) (ComplexVal(x, y))
#define CV(x)	((ComplexVal)x)

namespace gatespec {

namespace val {
const mreal_t r_sqrt2 = 1.0 / mreal::sqrt(2.0); // 1/sqrt(2)
const ComplexVal i_r_sqrt2 = _CV(0.0, r_sqrt2); // 1/sqrt(2) * i

const ComplexVal epi_8 = mreal::exp(_CV(0, M_PI / 8.0)); // e^(i*pi/8)
const ComplexVal e_pi_8 = mreal::exp(_CV(0, - M_PI / 8.0)); // e^(-i*pi/8);
const ComplexVal epi_4 = mreal::exp(_CV(0, M_PI / 4.0)); // e^(i*pi/4)
const ComplexVal e_pi_4 = mreal::exp(_CV(0, - M_PI / 4.0)); // e^(-i*pi/4);

const ComplexVal zero(0.0, 0.0); //0
const ComplexVal one(1.0, 0.0); //1
const ComplexVal minus_one(-1.0, 0); //-1
const ComplexVal image_unit(0.0, 1.0); //i
const ComplexVal minus_image_unit(0.0, -1.0); //-i
};

namespace sgq {

struct T {
	static const char name[];
	static const int cost;
};

struct H {
	static const char name[];
	static const int cost;
};

struct S {
	static const char name[];
	static const int cost;
};

struct I {
	static const char name[];
	static const char inverse_name[];
};
}

namespace twq {

struct T1 {
	static const char name[];
	static const int cost;
};

struct H1 {
	static const char name[];
	static const int cost;
};

struct S1 {
	static const char name[];
	static const int cost;
};

struct T2 {
	static const char name[];
	static const int cost;
};

struct H2 {
	static const char name[];
	static const int cost;
};

struct S2 {
	static const char name[];
	static const int cost;
};

struct CNOT1 {
	static const char name[];
	static const int cost;
};

struct CNOT2 {
	static const char name[];
	static const int cost;
};

struct I {
	static const char name[];
	static const char equiv_name1[];
	static const char equiv_name2[];
	static const char equiv_name3[];
};

}

}  // namespace gatespec


#endif /* GATESPECIFICATION_H_ */
