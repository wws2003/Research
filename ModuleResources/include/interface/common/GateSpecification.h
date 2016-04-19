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
const mreal_t r_sqrt2 = 1.0 / mreal::sqrt(2.0);
const ComplexVal i_r_sqrt2 = _CV(0.0, r_sqrt2); // 1/sqrt(2) * i

const ComplexVal epi_8 = std::exp(_CV(0, M_PI / 8.0)); // e^(i*pi/8)
const ComplexVal e_pi_8 = std::exp(_CV(0, - M_PI / 8.0)); // e^(-i*pi/8);
const ComplexVal epi_4 = std::exp(_CV(0, M_PI / 4.0)); // e^(i*pi/4)
const ComplexVal e_pi_4 = std::exp(_CV(0, - M_PI / 4.0)); // e^(-i*pi/4);

const ComplexVal zero(0.0, 0.0);
const ComplexVal minus_one(-1.0, 0);
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

}  // namespace gatespec


#endif /* GATESPECIFICATION_H_ */
