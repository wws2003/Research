/*
 * IOUtil.h
 *
 *  Created on: Nov 28, 2015
 *      Author: pham
 */

#ifndef IOUTIL_H_
#define IOUTIL_H_

#include <string>

//Check if a folder specified by path does exist or not
bool isFolderExisting(std::string folderPath);

//Create a folder specified by path
//If the folder already exists, do nothing
//If there is any error when create folder, throws an integer exception
//for the error code
void createFolder(std::string folderPath);

//Remove any sub elements (file, folder) under the specified path
//If there is any error, throws an integer exception
//for the error code
void emptyFolder(std::string folderPath);

//Remove folder as well as all sub elements
//If there is any error, throws an integer exception
//for the error code
void deleteFolder(std::string folderPath);

#endif /* IOUTIL_H_ */
