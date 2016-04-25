/*
 * Context.h
 *
 *  Created on: Apr 25, 2016
 *      Author: pham
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "ApplicationCommon.h"
#include <string>

class Context {
public:
	virtual ~Context();

	//Get singleton instance
	static Context* setup(bool dryMode, std::string appRootFolder);

	static Context* getInstance();

	std::string getAppRootFolder();

	std::string getAppDataFolder();

private:
	Context(bool dryMode, std::string appRootFolder);

	void createAppRootFolder();

	void createAppSubFolders();

	void deleteAppRootFolder();

	bool m_isDryMode;
	std::string m_appRootFolder;

	//Sub-folders of app root folder, including data, ...
	std::string m_appDataFolder;

	static Context* g_pContext;

	static const std::string g_dataFolder;
};

typedef Context* ContextPtr;

#endif /* CONTEXT_H_ */
