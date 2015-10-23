/*
 * NotAvailableCommandExecutor.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: pham
 */

#include "NotAvailableCommand.h"
#include <iostream>

NotAvailableCommand::NotAvailableCommand() {

}

NotAvailableCommand::~NotAvailableCommand() {

}

void NotAvailableCommand::execute() {
	std::cout << "Sorry. Feature hasn't been available yet." << std::endl;
}
