/*
 * SampleLibraryMatrixStore.h
 *
 *  Created on: Oct 20, 2015
 *      Author: pham
 */

#ifndef SAMPLELIBRARYMATRIXSTORE_H_
#define SAMPLELIBRARYMATRIXSTORE_H_

#include "OperatorCommon.h"
#include "ILibraryMatrixStore.h"
#include "IMatrixFactory.h"
#include <map>

typedef std::map<std::string, MatrixPtr> MatrixLabelMap;

class SampleLibraryMatrixStore: public ILibaryMatrixMatrixStore {
public:
	SampleLibraryMatrixStore(MatrixFactoryPtr pMatrixFactory, MatrixOperatorPtr pMatrixOperator);

	virtual ~SampleLibraryMatrixStore();

	//Override
	virtual MatrixPtr getLibraryMatrixFromLabel(std::string label);

private:
	void initMap();

	MatrixPtr getHMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getTMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getCNOT1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getCNOT2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getCV1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getCV2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getH1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getH2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getT1Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getT2Matrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);
	MatrixPtr getInverseTMatrix(MatrixOperatorPtr pMatrixOperator, MatrixFactoryPtr pMatrixFactory);

	MatrixFactoryPtr m_pMatrixFactory;
	MatrixOperatorPtr m_pMatrixOperator;
	MatrixLabelMap m_map;
};



#endif /* SAMPLELIBRARYMATRIXSTORE_H_ */
