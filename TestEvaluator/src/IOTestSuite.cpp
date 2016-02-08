/*
 * IOTestSuite.cpp
 *
 *  Created on: Aug 10, 2015
 *      Author: pham
 */

#include "IOTestSuite.h"
#include "SimpleDenseMatrixImpl.h"
#include "BinaryMatrixReaderImpl.h"
#include "BinaryMatrixWriterImpl.h"
#include "BinaryGateReaderImpl.h"
#include "BinaryGateWriterImpl.h"
#include "SimpleDenseMatrixFactoryImpl.h"
#include "PersistableGNATGateCollectionImpl.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cassert>

template<typename TPtr>
void getSampleElement(TPtr& rSampleElement);

void getSampleElement(MatrixPtr& rpSampleMatrix);
void getSampleElement(GatePtr& rpSampleGate);
void constructSampleGNATCollection(PersistableGNATGateCollectionImpl* pSampleCollection);

const std::string IOTestSuite::MATRIX_FILE_NAME = "matrix.dat";
const std::string IOTestSuite::GATE_FILE_NAME = "gate.dat";
const std::string IOTestSuite::GNAT_COLLECTION_FILE = "gnat_collection.dat";

IOTestSuite::IOTestSuite(): m_matrixOutputStream(MATRIX_FILE_NAME, std::ofstream::out | std::ofstream::binary),
		m_matrixInputStream(MATRIX_FILE_NAME, std::ifstream::in | std::ifstream::binary),
		m_gateOutputStream(GATE_FILE_NAME, std::ofstream::out | std::ofstream::binary),
		m_gateInputStream(GATE_FILE_NAME, std::ifstream::in | std::ifstream::binary) {

	//Instantiate matrix reader, writer
	m_pMatrixFactory = new SimpleDenseMatrixFactoryImpl();
	m_pMatrixReader = new BinaryMatrixReaderImpl(m_pMatrixFactory);
	m_pMatrixWriter = new BinaryMatrixWriterImpl();

	//Instantiate gate reader, writer
	m_pGateWriter = new BinaryGateWriterImpl(m_pMatrixWriter);
	m_pGateReader = new BinaryGateReaderImpl(m_pMatrixReader);
}

IOTestSuite::~IOTestSuite() {
	//Release gate reader, writer
	delete m_pGateReader;
	delete m_pGateWriter;

	//Release matrix reader, writer
	delete m_pMatrixWriter;
	delete m_pMatrixReader;
	delete m_pMatrixFactory;

	//Close open files
	m_gateInputStream.close();
	m_gateOutputStream.close();
	m_matrixInputStream.close();
	m_matrixOutputStream.close();

	//Delete data file
	clearFiles();
}

void IOTestSuite::test() {

	//Test write matrix to file and read back
	testMatrixReadWrite();

	//Test write gate to file and read back
	testGateReadWrite();

	//Test write PersitableGNATCollection to file and read back
	testGNATCollectionPersistence();
}

template<typename TPtr>
void IOTestSuite::testReadWriteElement(WriterPtr<TPtr> pWriter, ReaderPtr<TPtr> pReader, std::ofstream& outputStream, std::ifstream& inputStream) {
	// Instantiate sample element
	TPtr pSampleElment = NullPtr;
	getSampleElement(pSampleElment);

	//Write sample element to file
	pWriter->write(pSampleElment, outputStream);

	//Read back element from file
	TPtr pReadElement = NullPtr;
	pReader->read(pReadElement, inputStream);

	//Compare to sample element
	assert(pReadElement != NullPtr);
	assert(*pSampleElment == *pReadElement);

	//Release read element
	delete pReadElement;

	//Release sample element
	delete pSampleElment;
}

void IOTestSuite::assertFilesOpen() {
	assert(m_matrixOutputStream.is_open());
	assert(m_matrixInputStream.is_open());
	assert(m_gateOutputStream.is_open());
	assert(m_gateInputStream.is_open());
}

void IOTestSuite::testMatrixReadWrite() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	testReadWriteElement<MatrixPtr>(m_pMatrixWriter, m_pMatrixReader, m_matrixOutputStream, m_matrixInputStream);
	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void IOTestSuite::testGateReadWrite() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	testReadWriteElement<GatePtr>(m_pGateWriter, m_pGateReader, m_gateOutputStream, m_gateInputStream);
	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void IOTestSuite::testGNATCollectionPersistence() {
	std::cout  << "--------------------------"<<  std::endl << __func__ << std::endl;
	PersistableGNATGateCollectionImpl* pSampleCollection = new PersistableGNATGateCollectionImpl(NullPtr, m_pGateWriter, m_pGateReader);

	constructSampleGNATCollection(pSampleCollection);

	pSampleCollection->save(GNAT_COLLECTION_FILE);

	PersistableGNATGateCollectionImpl* pReadCollection = new PersistableGNATGateCollectionImpl(NullPtr, m_pGateWriter, m_pGateReader);
	pReadCollection->load(GNAT_COLLECTION_FILE);

	assert(*pSampleCollection == *pReadCollection);

	delete pReadCollection;
	delete pSampleCollection;

	std::cout << __func__ << " passed " << std::endl << "--------------------------"<<  std::endl ;
}

void IOTestSuite::clearFiles() {
	std::remove(GATE_FILE_NAME.c_str());
	std::remove(MATRIX_FILE_NAME.c_str());
	std::remove(GNAT_COLLECTION_FILE.c_str());
}

void getSampleElement(MatrixPtrRef rpSampleMatrix) {
	ComplexVal expmPi_8 = std::exp(ComplexVal(0, M_PI / 8.0));
	ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, - M_PI / 8.0));

	ComplexVal arrayT1[] = {expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8};

	rpSampleMatrix = new SimpleDenseMatrixImpl(arrayT1, ROW_SPLICE, 4, 4, "Label");
}

void getSampleElement(GatePtr& rpSampleGate) {
	ComplexVal expmPi_8 = std::exp(ComplexVal(0, M_PI / 8.0));
	ComplexVal expm_Pi_8 = std::exp(ComplexVal(0, -M_PI / 8.0));

	ComplexVal arrayT2[] = {expm_Pi_8, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, expmPi_8, (ComplexVal)0.0, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, expm_Pi_8, (ComplexVal)0.0
			, (ComplexVal)0.0, (ComplexVal)0.0, (ComplexVal)0.0, expmPi_8};

	MatrixPtr pT2Mat = new SimpleDenseMatrixImpl(arrayT2, ROW_SPLICE, 4, 4, "");

	LabelSeq labelSeq = {"T1", "ED"};
	rpSampleGate = new Gate(pT2Mat, 1, labelSeq);

}

void constructSampleGNATCollection(PersistableGNATGateCollectionImpl* pSampleCollection) {
	unsigned int nbGates = 13;
	GatePtr* pGates = new GatePtr[nbGates];
	for(unsigned int i = 0; i < nbGates; i++) {
		getSampleElement(pGates[i]);
	}

	int gateCounter = 0;
	//Split points: 2 splits points
	SplitPointSet<GatePtr> splitPoints = {pGates[gateCounter++],
			pGates[gateCounter++]};

	//Unstructured buffer: 3 points
	UnstructuredBuffer<GatePtr> unStructeredBuffer = {pGates[gateCounter++],
			pGates[gateCounter++],
			pGates[gateCounter++]};

	//RangeMap: Sample
	RangeMap splitPointRanges = {{Range(1,2), Range(2,3)},
			{Range(-1,2), Range(21,3)},
			{Range(1,5)}};

	//Sub Collection 1
	PersistableGNATCollectionImpl<GatePtr>* pSubCollection1 = new PersistableGNATCollectionImpl<GatePtr>(NullPtr, NullPtr, NullPtr);

	//---Unstructured buffer: 3 points
	UnstructuredBuffer<GatePtr> subUnStructeredBuffer1 = {pGates[gateCounter++],
			pGates[gateCounter++],
			pGates[gateCounter++]};

	//---Split points: 2 splits points
	SplitPointSet<GatePtr> subSplitPoints1 = {pGates[gateCounter++],
			pGates[gateCounter++]};

	//---RangeMap: Sample
	RangeMap subSplitPointRanges1 = {{Range(10,2), Range(0,3)},
			{Range(-1,2), Range(21,3)}};

	pSubCollection1->init(subSplitPoints1, subUnStructeredBuffer1, subSplitPointRanges1);

	//Sub Collection 2
	PersistableGNATCollectionImpl<GatePtr>* pSubCollection2 = new PersistableGNATCollectionImpl<GatePtr>(NullPtr, NullPtr, NullPtr);

	//---Unstructured buffer: 3 points
	UnstructuredBuffer<GatePtr> subUnStructeredBuffer2 = {pGates[gateCounter++],
			pGates[gateCounter++],
			pGates[gateCounter++]};

	//---Split points: 0 splits points
	SplitPointSet<GatePtr> subSplitPoints2;

	//---RangeMap: Empty
	RangeMap subSplitPointRanges2;

	pSubCollection2->init(subSplitPoints2, subUnStructeredBuffer2, subSplitPointRanges2);

	pSampleCollection->init(splitPoints, unStructeredBuffer, splitPointRanges);
	pSampleCollection->addSubCollection(pSubCollection1);
	pSampleCollection->addSubCollection(pSubCollection2);
}


