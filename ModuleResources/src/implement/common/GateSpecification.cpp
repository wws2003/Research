/*
 * GateSpecification.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: pham
 */

#include "GateSpecification.h"
namespace gatespec {

namespace sgq {

const char T::name[] = "T";
const int T::cost = 100;

const char H::name[] = "H";
const int H::cost = 10;

const char S::name[] = "S";
const int S::cost = 20;

const char I::name[] = "I";
const char I::inverse_name[] = "-I";
}

namespace twq {
const char T1::name[] = "T1";
const int T1::cost = 100;

const char TDagger1::name[] = "t1";
const int TDagger1::cost = 100;

const char H1::name[] = "H1";
const int H1::cost = 10;

const char S1::name[] = "S1";
const int S1::cost = 20;

const char T2::name[] = "T2";
const int T2::cost = 100;

const char TDagger2::name[] = "t2";
const int TDagger2::cost = 100;

const char H2::name[] = "H2";
const int H2::cost = 10;

const char S2::name[] = "S2";
const int S2::cost = 20;

const char CNOT1::name[] = "CNOT1";
const int CNOT1::cost = 1;

const char CNOT2::name[] = "CNOT2";
const int CNOT2::cost = 1;

const char I::name[] = "I";
const char I::equiv_name1[] = "-I";
const char I::equiv_name2[] = "iI";
const char I::equiv_name3[] = "-iI";

}

}  // namespace gatespec



