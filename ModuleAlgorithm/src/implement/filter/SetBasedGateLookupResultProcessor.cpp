/*
 * SetBasedGateLookupResultProcessor.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: pham
 */

#include "SetBasedGateLookupResultProcessor.h"
#include "SetBasedLookupResultProcessor.cpp"

template class SetBasedLookupResultProcessor<GatePtr>;

SetBasedGateLookupResultProcessor::SetBasedGateLookupResultProcessor(DistanceCalculatorPtr<GatePtr> pDistanceCalculator) : SetBasedLookupResultProcessor<GatePtr>(pDistanceCalculator){

}

bool SetBasedGateLookupResultProcessor::sameElement(const GatePtr& pGate1, const GatePtr& pGate2) const {
	MatrixPtr pMatrix1 = pGate1->getMatrix();
	MatrixPtr pMatrix2 = pGate2->getMatrix();

	int nbRows, nbColumns;

	//Assume 2 matrix are the same size
	pMatrix1->getSize(nbRows, nbColumns);

	mreal_t deltaReal = 0.0;
	mreal_t deltaImg = 0.0;

	const mreal_t sqr_norm = 1e-30;
	const mreal_t norm = 1e-12;

	for(int r = 0; r < nbRows; r++) {
		for (int c = 0; c < nbColumns; c++) {
			ComplexVal v1 = pMatrix1->getValue(r, c);
			ComplexVal v2 = pMatrix2->getValue(r, c);

			deltaReal += mreal::abs(v1.real() - v2.real());
			if(deltaReal > norm) {
				return false;
			}

			deltaImg +=  mreal::abs(v1.imag() - v2.imag());
			if(deltaImg > norm) {
				return false;
			}
		}
	}

	return (deltaReal * deltaReal + deltaImg * deltaImg <= sqr_norm);
}


