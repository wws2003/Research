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
	static constexpr char name[] = "T";
	static constexpr int cost = 100;
};
struct H {
	static constexpr char name[] = "H";
	static constexpr int cost = 10;
};
struct S {
	static constexpr char name[] = "S";
	static constexpr int cost = 20;
};
struct I {
	static constexpr char name[] = "I";
	static constexpr char inverse_name[] = "-I";
};
}

namespace twq {

struct T1 {
	static constexpr char name[] = "T1";
	static constexpr int cost = 100;
};
struct H1 {
	static constexpr char name[] = "H1";
	static constexpr int cost = 10;
};
struct S1 {
	static constexpr char name[] = "S1";
	static constexpr int cost = 20;
};

struct T2 {
	static constexpr char name[] = "T2";
	static constexpr int cost = 100;
};
struct H2 {
	static constexpr char name[] = "H2";
	static constexpr int cost = 10;
};
struct S2 {
	static constexpr char name[] = "S2";
	static constexpr int cost = 20;
};

struct CNOT1 {
	static constexpr char name[] = "CNOT1";
	static constexpr int cost = 1;
};
struct CNOT2 {
	static constexpr char name[] = "CNOT2";
	static constexpr int cost = 1;
};

struct I {
	static constexpr char name[] = "I";
	static constexpr char equiv_name1[] = "-I";
	static constexpr char equiv_name2[] = "iI";
	static constexpr char equiv_name3[] = "-iI";
};
}

}  // namespace gatespec


#endif /* GATESPECIFICATION_H_ */
