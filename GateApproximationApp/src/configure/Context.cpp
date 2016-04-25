/*
 * Context.cpp
 *
 *  Created on: Apr 25, 2016
 *      Author: pham
 */

#include "Context.h"
#include "IOUtil.h"
#include <iostream>

Context* Context::g_pContext = NULL;
const std::string Context::g_dataFolder = "data";

Context* Context::setup(bool dryMode, std::string appRootFolder) {
	if(Context::g_pContext != NULL) {
		delete Context::g_pContext;
	}
	Context::g_pContext = new Context(dryMode, appRootFolder);
	return Context::g_pContext;
}

Context::Context(bool dryMode, std::string appRootFolder) {
	m_isDryMode = dryMode;
	m_appRootFolder = appRootFolder;
	m_appDataFolder = m_appRootFolder + "/" + g_dataFolder;
	createAppRootFolder();
}

Context::~Context() {
	deleteAppRootFolder();
}

Context* Context::getInstance() {
	return Context::g_pContext;
}

std::string Context::getAppRootFolder() {
	return m_appRootFolder;
}

std::string Context::getAppDataFolder() {
	return m_appDataFolder;
}

void Context::createAppRootFolder() {
	if(!isFolderExisting(m_appRootFolder)) {
		std::cout << "Root folder " << m_appRootFolder << " not found. To create new one\n";
		createFolder(m_appRootFolder);
	}
	createAppSubFolders();
}

void Context::createAppSubFolders() {
	//Just data folder for now
	if(!isFolderExisting(m_appDataFolder)) {
		std::cout << "Sub Folder " << m_appDataFolder << " not found. To create new one\n";
		createFolder(m_appDataFolder);
	}
}

void Context::deleteAppRootFolder() {
	if(m_isDryMode) {
		deleteFolder(m_appRootFolder);
	}
}
