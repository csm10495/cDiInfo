/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Registry.h - Windows Registry interfacing header file
*/

#pragma once

// Local includes
#include "Strings.h"

// STL includes
#include <string>

// WinApi includes
#include <Windows.h>

// Services folder
#define REGISTRY_SERVICES "SYSTEM\\CurrentControlSet\\Services\\"

// Gets a string from a given place in the registry
bool getStringFromRegistry(HKEY hkey, std::string subKey, std::string value, std::string &output);

// Gets a numeric from a given place in the registry
bool getUIntFromRegistry(HKEY hkey, std::string subKey, std::string value, UINT64 &output);