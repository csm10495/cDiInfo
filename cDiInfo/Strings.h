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
#include <string.h>
#include <iomanip>
#include <iostream>

// WinApi includes
#include <Windows.h>
#include <cfgmgr32.h>
#define INITGUID //Define to make the DEVPKEYs work properly
#include <Devpropdef.h>
#include <devpkey.h>
#include <sddl.h>
#include <usbioctl.h>
#undef INITGUID

// Local includes
#include "Include/csmisas.h"

// Macro to get a bool string
#define toBoolString(num) ((num) ? "True" : "False")

namespace cdi
{
    namespace strings
    {
        // Left and right trims of the given string (by reference) and returns it
        std::string trim(std::string &s);

        // Left trims the given string (by reference) and returns it
        std::string& lTrim(std::string& s);

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

        // Converts a string delimited by \0 to newlines
        std::string delimitedStringToNewlineString(std::string& retStr);

        // Converts storage bus type to string
        std::string storageBusTypeToString(STORAGE_BUS_TYPE busType);

        // Converts a string to a GUID, returns True if successful
        bool stringToGuid(std::string guidAsString, LPGUID guid);

        // Converts a USB_CONNECTION_STATUS to string
        std::string usbConnectionStatusToString(USB_CONNECTION_STATUS tmp);

        // Parses the SMART data into a string
        std::string smartToString(BYTE* smartBytes, UINT16 smartLength, BYTE* smartThresholds);

        // Split a string with a delim
        std::vector<std::string> split(std::string s, const char delim);

        // Converts a SMART id to a string name... these may not be perfect, but should be close.
        std::string smartIdToString(UINT8 smartId);

        // Converts the GptAttributes to a string
        std::string gptAttributesToString(ULONGLONG gptAttr);

        // Converts an ErrorControl to string
        std::string errorControlToString(DWORD errorControl);

        // Converts a StartType to string
        std::string startTypeToString(DWORD tmp);

        // Converts a SerivceType to string
        std::string serviceTypeToString(DWORD tmp);

        // Converts a file system flag to string
        std::string fileSystemFlagToString(UINT32 flag);

        // Converts driver type to string
        std::string driverTypeToString(DWORD driverType);

        // Gets a description of the given RESOURCEID
        std::string resourceTypeToDescription(RESOURCEID resourceType);

        // Gets an xml-safe representation of the inStr
        std::string stringToXmlSafeStr(std::string inStr);

        // Convert RAID/CSMI drive status to string
        std::string csmiDriveStatusToString(int driveStatus);

        // Convert RAID/CSMI drive usage to string
        std::string csmiDriveUsageToString(int driveUsage);

        // Convert RAID/CSMI drive type to string
        std::string csmiDriveTypeToString(int driveType);

        // Replaces over and over until the findStr is no longer in fullStr
        std::string recursiveReplace(std::string fullStr, std::string findStr, std::string replaceStr);

        // Convert CSMI IO Bus Type to string
        std::string csmiIoBusTypeToString(int busType);

        // Converts CSMI Controller flags to string
        std::string csmiControllerFlagsToString(int flags);

        // Converts an ATA String to normal string (byte swap every 2 bytes)
        std::string ataStringToString(std::string ataString);

        // Convert CSMI link rate to string
        std::string linkRateToString(int linkRate);
    }
}