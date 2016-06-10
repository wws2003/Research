/*
 * SelingerProgramAdapter.h
 *
 *  Created on: Jun 10, 2016
 *      Author: pham
 */

#ifndef SELINGERPROGRAMADAPTER_H_
#define SELINGERPROGRAMADAPTER_H_

#include "IMatrixFactory.h"
#include "EvaluateCommon.h"
#include "OperatorCommon.h"
#include "ICollection.h"
#include <map>

#define UV_ARRAY_SIZE (5)

class SelingerProgramAdapter {
public:
	SelingerProgramAdapter(MatrixFactoryPtr pMatrixFactory);

	virtual ~SelingerProgramAdapter(){};

	void initState();

	void consumeOutputLine(std::string line);

	bool isOutputMatrixAvailable() const;

	LookupResult<MatrixPtr> getOutputMatrix() const;

private:
	void initContants();

	//Methods to parse line of output
	void parseOutputLine(std::string outputLine,
			std::string& uValueStr,
			std::string& tValueStr,
			std::string& errorValueStr,
			std::string& circuitValueStr);

	//Methods to read proper values from line
	void readValuesFromStr(std::string valueStr, int* values);
	mreal_t readErrorFromStr(std::string errorStr);
	std::string readLabelFromStr(std::string labelStr);

	MatrixPtr createMatrixFromUT(const int *u, const int *t) const;

	ComplexVal calculateComplexValFromRingParam(const int *params) const;

	ComplexVal getReverseSqrt2Pow(int pow) const;

	//Fields to remember read values
	std::string m_label;
	int m_us[UV_ARRAY_SIZE];
	int m_ts[UV_ARRAY_SIZE];
	mreal_t m_error;

	MatrixFactoryPtr m_pMatrixFactory;

	//Constants values
	std::map<int, mreal_t> m_factorMap;
	ComplexVal omega;
	ComplexVal omega2;
	ComplexVal omega3;
};


#endif /* SELINGERPROGRAMADAPTER_H_ */
