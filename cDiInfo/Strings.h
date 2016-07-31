/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Strings.h - Header for string manipulation functions
*/
#pragma once

// STL includes
#include <algorithm>
#include <regex>
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>

// WinApi includes
#include <Windows.h>
#include <cfgmgr32.h>
#define INITGUID //Define to make the DEVPKEYs work properly
#include <Devpropdef.h>
#include <devpkey.h>
#undef INITGUID
#include <sddl.h>

// Couldn't parse the resource
#define UNABLE_TO_PARSE_RESOURCE "<Unable To Parse Resource>"

// Used for buffer with 0 length
#define ZERO_LENGTH_BUFFER "<Zero Length buffer>"


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

// Converts a normal string to wide string
std::wstring stringToWString(std::string wStr);

// converts a DEVPROPKEY to string
std::string propertyKeyToString(DEVPROPKEY propertyKey);

// Gets a string representation of the given propertyBuffer depending on the propertyType
std::string propertyBufferToString(BYTE* propertyBuffer, ULONG propertyBufferSize, DEVPROPTYPE propertyType, DEVPROPKEY propertyKey);

// Byte array to string table
std::string byteArrayToString(BYTE* buffer, ULONG bufferSize);

// Resource type to string
std::string resourceTypeToString(RESOURCEID resourceType);

// Gets a string representation of the resource buffer
std::string resourceToString(BYTE* buffer, ULONG bufferSize, RESOURCEID resourceType);

// Converts a number to a hex string with a given width
std::string numToHexString(UINT64 number, int width);

// Converts a power state to string
std::string powerStateToString(ULONG powerState);

// Converts a system power state to string
std::string systemPowerStateToString(ULONG tmp);