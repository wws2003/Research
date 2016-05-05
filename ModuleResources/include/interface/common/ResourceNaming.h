/*
 * ResourceNaming.h
 *
 *  Created on: May 6, 2016
 *      Author: pham
 */

#ifndef RESOURCENAMING_H_
#define RESOURCENAMING_H_

#include "ResourcesCommon.h"
#include <map>

class ResourceNaming {
public:
	ResourceNaming();
	virtual ~ResourceNaming(){};

	LibrarySet getLibrarySetFromCanonicalName(std::string canonicalName) const;

	RotationType getRotationTypeFromCanonicalName(std::string canonicalName) const;

	std::string getLibrarySetDisplayName(LibrarySet librarySet) const;

private:
	typedef std::map<std::string, int> LibrarySetNameMap;
	typedef std::map<std::string, int> RotationTypeNameMap;

	LibrarySetNameMap m_librarySetCanonicalNameMap;
	RotationTypeNameMap m_rotationSetCanonicalNameMap;
};



#endif /* RESOURCENAMING_H_ */
