/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Features.h - Features header file
*/
#pragma once

// Local includes
#include "Core.h"
#include "Enumerations.h"

// STL includes
#include <vector>

// Tries to disable a devInst
STATUS disableDevice(DEVINST devInst);

// Tries to enable a devInst
STATUS enableDevice(DEVINST devInst);

// Prints everything we can get about every device
void printAllInfo();

// Prints a vector of strings with a title
void printVectorOfStrings(std::vector<std::string> &vec, std::string title);

// Gets a vector of attribute keys (note that this may not be all possible ones)
std::vector<std::string> getSampleAttributeKeys();

// Get a vector of enumerators
std::vector<std::string> getEnumerators();

// Get a vector of classes
std::vector<std::string> getClasses();
