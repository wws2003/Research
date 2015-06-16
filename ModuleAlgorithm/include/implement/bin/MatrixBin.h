/*
 * MatrixBin.h
 *
 *  Created on: Jun 15, 2015
 *      Author: pham
 */

#ifndef MATRIXBIN_H_
#define MATRIXBIN_H_

#include "IMatrix.h"
#include "MathConceptsCommon.h"
#include <vector>
#include <string>

typedef std::string BinPattern;

class MatrixBin {
public:
	MatrixBin(BinPattern pattern);
	virtual ~MatrixBin();

	inline void addMatrix(MatrixPtr pMatrix){m_pMatrices.push_back(pMatrix);};

	inline void clear(){m_pMatrices.clear();};

	int distance(const MatrixBin& otherBin) const;

	inline const MatrixPtrVector& getMatrices(){return m_pMatrices;};

private:
	BinPattern m_pattern;
	MatrixPtrVector m_pMatrices;
};

typedef MatrixBin* MatrixBinPtr;
typedef std::vector<MatrixBinPtr> MatrixBinPtrVector;

#endif /* MATRIXBIN_H_ */
