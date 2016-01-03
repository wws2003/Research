/*
 * IOUtil.cpp
 *
 *  Created on: Nov 28, 2015
 *      Author: pham
 */

#include "IOUtil.h"
#include <sys/types.h>
#include <exception>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

bool isFolderExisting(std::string folderPath) {
	DIR *theFolder = opendir(folderPath.c_str());

	if(theFolder == NULL) {
		return false;
	}
	else {
		closedir(theFolder);
		return true;
	}
}

void createFolder(std::string folderPath) {
	struct stat st = {0};

	if (stat(folderPath.c_str(), &st) == -1) {
		int err = mkdir(folderPath.c_str(), 0766);
		if(err) {
			throw err;
		}
	}
}

void emptyFolder(std::string folderPath) {
	DIR *theFolder = opendir(folderPath.c_str());

	if(theFolder == NULL) {
		throw -1;
	}

	struct dirent *nextSubFile;

	char filePath[256];

	while ((nextSubFile = readdir(theFolder)) != NULL)
	{
		// build the path for each file in the folder
		sprintf(filePath, "%s/%s", folderPath.c_str(), nextSubFile->d_name);

		if(nextSubFile->d_name[0] != '.') {
			if(nextSubFile->d_type == DT_DIR) {
				emptyFolder(filePath);
			}
			int err = remove(filePath);
			if(err) {
				printf("Error when try to remove %s\n", filePath);
				throw err;
			}
		}
	}

	closedir(theFolder);
}


