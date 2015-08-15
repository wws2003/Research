/*
 * SimpleIdentityMatrixImpl.h
 *
 *  Created on: May 16, 2015
 *      Author: pham
 */

#ifndef SIMPLEIDENTITYMATRIXIMPL_H_
#define SIMPLEIDENTITYMATRIXIMPL_H_

#include "IMatrix.h"
#include "MathConceptsCommon.h"

class SimpleIdentityMatrixImpl : public IMatrix {
public:
	/**
	 * Initialize matrix from an array of complex values, given also
	 * - Number of row
	 * - Number of column
	 * - Splice type of value array, i.e. 2-D alignment of value array, either col-col-col.. or row-row-row..
	 * - Matrix label
	 */
	SimpleIdentityMatrixImpl(int dimension);

	virtual ~SimpleIdentityMatrixImpl(){};

	//Override
	virtual void getSize(int& rows, int& columns) const;

	//Override
	virtual ComplexVal getValue(int row, int column) const;

	//Override
	virtual void toArray(ComplexValArrayRef array) const;

	//Override
	virtual std::string getLabel() const;

	//Override
	virtual MatrixPtr clone() const;

private:
	int m_dimension;
};


#endif /* SIMPLEIDENTITYMATRIXIMPL_H_ */
