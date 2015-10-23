/*
 * Config.h
 *
 *  Created on: Oct 23, 2015
 *      Author: pham
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "MathConceptsCommon.h"

enum LibrarySet {
	L_HT,
	L_HTCNOT,
	L_HCV,
	L_HTCV
};

typedef struct _CollectionConfig {
	LibrarySet m_librarySet;
	int m_maxSequenceLength;
	int m_nbQubits;
}
CollectionConfig;

typedef struct _EvaluatorConfig {
	LibrarySet m_librarySet;
	double m_collectionEpsilon;
	double m_approximatorEpsilon;
	int m_nbQubits;
}
EvaluatorConfig;


#endif /* CONFIG_H_ */
