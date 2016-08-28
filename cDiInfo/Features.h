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
void printVectorOfStrings(std::vector<std::string> &vec, std::string title, bool printStars);

// Gets a vector of attribute keys (note that this may not be all possible ones)
std::vector<std::string> getSampleAttributeKeys();

// Get a vector of enumerators
std::vector<std::string> getEnumerators();

// Get a vector of classes
std::vector<std::string> getClasses();

// Gets all AttributeSets that have a matching key and value
std::vector<cdi::attr::AttributeSet> getAttributeSetsWith(std::string key, std::string value);

// Gets all AttributeSets that have a matching key and value
std::vector<cdi::attr::AttributeSet> getAttributesWith(std::string key, std::string value, std::string alt);

// Gets all AttributeSets that don't have a matching key and value
std::vector<cdi::attr::AttributeSet> getAttributeSetsWithout(std::string key, std::string value);