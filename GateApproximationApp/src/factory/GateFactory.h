/*
 * GateFactory.h
 *
 *  Created on: Aug 28, 2015
 *      Author: pham
 */

#ifndef GATEFACTORY_H_
#define GATEFACTORY_H_

#include "Gate.h"
#include "MathConceptsCommon.h"
#include "OperatorCommon.h"
#include "ILibraryMatrixStore.h"
#include "Config.h"
#include <map>

typedef std::map<std::string, int> GateNameCodeMap;

class GateFactory {
public:
	GateFactory(LibraryMatrixStorePtr pLibraryMatrixStore);

	virtual ~GateFactory(){};
	GatePtr getLibraryGateInstance(std::string gateName, cost_t cost = 1);

private:
	void initGateNameCodeMap();

	GateNameCodeMap m_gateNameCodeMap;

	LibraryMatrixStorePtr m_pLibraryMatrixStore;

	enum GateCode {
		G_H,
		G_T,
		G_T_INVERSE,
		G_CNOT1,
		G_CV1,
		G_CNOT2,
		G_CV2,
		G_H1,
		G_H2,
		G_T1,
		G_T2
	};
};

typedef GateFactory* GateFactoryPtr;


#endif /* GATEFACTORY_H_ */
