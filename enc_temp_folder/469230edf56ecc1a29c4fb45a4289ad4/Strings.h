/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Strings.h - Header for string manipulation functions
*/
#pragma once

// WinApi includes
#include <Windows.h>

// STL includes
#include <algorithm>
#include <string>
#include <iostream>

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


