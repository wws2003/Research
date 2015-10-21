/*
 * NotAvailableCommandExecutor.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "NotAvailableCommandExecutor.h"
#include <iostream>

NotAvailableCommandExecutor::NotAvailableCommandExecutor() {

}

NotAvailableCommandExecutor::~NotAvailableCommandExecutor() {

}

void NotAvailableCommandExecutor::execute() {
	std::cout << "Sorry. Feature hasn't been available yet." << std::endl;
}
