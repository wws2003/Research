/*
 * SimpleDenseMatrixImpl.h
 *
 *  Created on: Apr 16, 2015
 *      Author: pham
 */

#ifndef SIMPLEDENSEMATRIXIMPL_H_
#define SIMPLEDENSEMATRIXIMPL_H_

#include "abstract/basic/math_concept/IMatrix.h"

class SimpleDenseMatrixImpl: public IMatrix {
public:
	SimpleDenseMatrixImpl(ComplexValArray array, int nbRows, int nbColumns, std::string label = "");

	virtual ~SimpleDenseMatrixImpl();

	//Override
	virtual void getSize(int& rows, int& columns);

	//Override
	virtual ComplexVal getValue(int row, int column);

	//Override
	virtual void toArray(ComplexValArrayRef array);

	//Override
	virtual std::string getLabel();

private:
	void initArray(ComplexValArray array, int nbRows, int nbColumns);

	std::string m_label = "";
	ComplexValArray m_array;
	int m_nbRows;
	int m_nbColumns;
};


#endif /* SIMPLEDENSEMATRIXIMPL_H_ */
