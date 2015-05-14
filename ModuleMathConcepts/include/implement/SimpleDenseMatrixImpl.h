/*
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 * SimpleDenseMatrixImpl.h
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#ifndef SIMPLEDENSEMATRIXIMPL_H_
#define SIMPLEDENSEMATRIXIMPL_H_

#include "IMatrix.h"
#include "MathConceptsCommon.h"

class SimpleDenseMatrixImpl: public IMatrix {
public:
	/**
	 * Initialize matrix from an array of complex values, given also
	 * - Number of row
	 * - Number of column
	 * - Splice type of value array, i.e. 2-D alignment of value array, either col-col-col.. or row-row-row..
	 * - Matrix label
	 */
	SimpleDenseMatrixImpl(const ComplexVal* array, ArraySpliceType arraySpliceType, int nbRows, int nbColumns, std::string label = "");

	virtual ~SimpleDenseMatrixImpl();

	//Override
	virtual void getSize(int& rows, int& columns) const;

	//Override
	virtual ComplexVal getValue(int row, int column) const;

	//Override
	virtual void toArray(ComplexValArrayRef array) const;

	//Override
	virtual std::string getLabel() const;

private:
	void initArray(const ComplexVal* array,  int nbRows, int nbColumns);

	std::string m_label = "";
	ComplexValArray m_array;
	int m_nbRows;
	int m_nbColumns;

	ArraySpliceType m_arraySpliceType;
};


#endif /* SIMPLEDENSEMATRIXIMPL_H_ */
