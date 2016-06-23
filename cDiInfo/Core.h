/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Core.h - Core SetupDi-interfacing header file
*/

#pragma once

// Local includes
#include "Enumerations.h"
#include "Registry.h"
#include "Strings.h"

// STL includes
#include <algorithm>
#include <map>
#include <regex>
#include <string>
#include <vector>

// WinApi Includes
#include <Windows.h>
#include <cfgmgr32.h>
#pragma warning ( push )
#pragma warning( disable: 4091 )
#include <Ntddscsi.h> // Disable the warning this throws
#pragma warning( pop )
#include <SetupAPI.h>

// An AttributeMap is a map of string to string
//   name of attribute -> string of attribute value
typedef std::map<std::string, std::string> AttributeMap;

// Used to designate that we couldn't get this attribute
#define UNAVAILABLE_ATTRIBUTE "<Attribute Unavailable>"
// Used to designate an inability to find children
#define NO_CHILDREN "<No Children Found>"
// Used to designate an inability to find the parent
#define NO_PARENT "<No Parent Found>"
// Used to designate an inability to find siblings
#define NO_SIBLINGS "<No Siblings Found>"
// Used if no problem is detected
#define NO_PROBLEM "<No Problem Detected>"
// Take a map and add a given item. Use the name of the field as the key of the map.
#define addToMap(map, itm) map[#itm] = itm

// Get an HDEVINFO for all classes 
HDEVINFO getAllClassesHDevInfo();

// Get an HDEVINFO for all interfaces of a given GUID
HDEVINFO getInterfaceHDevInfo(GUID classGuid);

// Takes a complete HDEVINFO, specific device PSP_DEV_INFO_DATA, DWORD property and TYPE
// The TYPE is used to return a string representation of the given property
std::string getDevInfoProperty(HDEVINFO &devs, PSP_DEVINFO_DATA devInfo, DWORD property, TYPE retType);

// Takes a complete HDEVINFO, corresponding SP_DEVINFO_DATA. Returns True if we are able
//   to get driver information. If returning True, the PSP_DRVINFO_DATA is updated
bool getDriverInfoData(HDEVINFO &devs, SP_DEVINFO_DATA &devInfo, PSP_DRVINFO_DATA driverInfoData);

// Gets the Device Id for the given DEVINST
std::string getDeviceId(DEVINST &devInst);

// Turns the CM_Prob number into a string of the status
std::string cmProbToString(ULONG problemNumber);

// Gets a string of the node status from the numeric mask
std::string nodeStatusToString(ULONG status);

// Takes a complete HDEVINFO and SP_DEVINFO_DATA and returns a map of string:string
//   this is a collection of 'attributes'
AttributeMap getDeviceAttributeMap(HDEVINFO &devs, SP_DEVINFO_DATA &devInfo, std::map<int, std::string> &scsiPortToDeviceIdMap);

// Gets a vector of interfaces based on the given GUID. If the GUID is GUID_NULL, all DEVINTERFACE GUIDs will be used
std::vector<AttributeMap> getInterfaceAttributeMap(GUID classGuid);

// Gets a vector of all devices and their AttributeMaps
std::vector<AttributeMap> getAllDevicesAttributeMap();

// Prints a given AttributeMap
void printAttributeMap(AttributeMap &attrMap);

// Gets a AttributeMap that has a matching key and value in it's AttributeMap
AttributeMap getAttributeMapWith(std::string key, std::string value);

// Gets a DEVINST that has a matching key and value in it's AttributeMap
DEVINST getDevInstWith(std::string &key, std::string &value);

// Gets a mapping from SCSI port to device id
std::map<int, std::string> getScsiPortToDeviceIdMap();