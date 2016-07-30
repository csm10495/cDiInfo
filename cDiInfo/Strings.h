/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Strings.h - Header for string manipulation functions
*/
#pragma once

// STL includes
#include <algorithm>
#include <regex>
#include <string>
#include <iostream>

// WinApi includes
#include <Windows.h>
#define INITGUID //Define to make the DEVPKEYs work properly
#include <Devpropdef.h>
#include <devpkey.h>
#undef INITGUID
#include <sddl.h>

// Right trims the given string (by reference) and returns it
std::string &rTrim(std::string &s);

// Converts the given string (by reference) to uppercase characters and returns it
std::string &toUpper(std::string &s);

// Returns true if the needle is at the start of the haystack
bool startsWith(std::string needle, std::string haystack, bool ignoreCase);

// Converts GUID to string
std::string guidToString(GUID guid);

// Turns the CM_Prob number into a string of the status
std::string cmProbToString(unsigned long problemNumber);

// Gets a string of the node status from the numeric mask
std::string nodeStatusToString(unsigned long status);

// Gets the string for the CM return value
std::string cmRetValueToString(unsigned long value);

// Converts a Windows error to string
std::string windowsErrorToString(uint32_t errorCode);

// Converts a wide string to normal string
std::string wStringToString(std::wstring wStr);

// converts a DEVPROPKEY to string
std::string propertyKeyToString(DEVPROPKEY propertyKey);

// Gets a string representation of the given propertyBuffer depending on the propertyType
std::string propertyBufferToString(BYTE* propertyBuffer, ULONG propertyBufferSize, DEVPROPTYPE propertyType);



