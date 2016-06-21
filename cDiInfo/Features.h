/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Features.h - Features header file
*/
#pragma once

// Local includes
#include "Enumerations.h"

// Tries to disable a device with a key/value
STATUS disableDevice(std::string key, std::string value);

// Tries to enable a device with a key/value
STATUS enableDevice(std::string key, std::string value);

// Prints everything we can get about every device
void printAllInfo();

// Gets the string for the CM return value
std::string cmRetValueToString(ULONG value);
