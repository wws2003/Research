/*
 * ResourceNaming.cpp
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#include "ResourceNaming.h"

ResourceNaming::ResourceNaming() {
	//Library set canonical names
	m_librarySetCanonicalNameMap["H-T"] = L_HT;
	m_librarySetCanonicalNameMap["H-T-S"] = L_HTS;
	m_librarySetCanonicalNameMap["H-T-CNOT"] = L_HTCNOT;
	m_librarySetCanonicalNameMap["H-T-t-CNOT"] = L_HTtCNOT;
	m_librarySetCanonicalNameMap["H-T-S-CNOT"] = L_HTSCNOT;

	//Rotation type canonical names
	m_rotationSetCanonicalNameMap["X"] = R_X;
	m_rotationSetCanonicalNameMap["Y"] = R_Y;
	m_rotationSetCanonicalNameMap["Z"] = R_Z;
	m_rotationSetCanonicalNameMap["CX"] = C_RX;
	m_rotationSetCanonicalNameMap["CY"] = C_RY;
	m_rotationSetCanonicalNameMap["CZ"] = C_RZ;
}

LibrarySet ResourceNaming::getLibrarySetFromCanonicalName(std::string canonicalName) const {
	auto nIter = m_librarySetCanonicalNameMap.find(canonicalName);
	if(nIter != m_librarySetCanonicalNameMap.end()) {
		return (LibrarySet)nIter->second;
	}
	return L_UNSPECIFIED;
}

RotationType ResourceNaming::getRotationTypeFromCanonicalName(std::string canonicalName) const {
	auto nIter = m_rotationSetCanonicalNameMap.find(canonicalName);
	if(nIter != m_rotationSetCanonicalNameMap.end()) {
		return (RotationType)nIter->second;
	}
	return C_UNSPECIFIED;
}

std::string ResourceNaming::getLibrarySetDisplayName(LibrarySet librarySet) const {
	std::string displayName;
	switch (librarySet) {
	case L_HT:
		displayName = "ht";
		break;
	case L_HTCNOT:
		displayName = "htcnot";
		break;
	case L_HTS:
		displayName = "hts";
		break;
	case L_HTSCNOT:
		displayName = "htscnot";
		break;
	case L_HTtCNOT:
		displayName = "httcnot";
		break;
	default:
		displayName = "unknown";
		break;
	}
	return displayName;
}


