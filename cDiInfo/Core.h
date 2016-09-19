/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Core.h - Core SetupDi-interfacing header file
*/

#pragma once

// Local includes
#include "Attribute.h"
#include "Enumerations.h"
#include "Registry.h"
#include "Strings.h"

// STL includes
#include <algorithm>
#include <future>
#include <map>
#include <string>
#include <vector>

// WinApi Includes
#include <Windows.h>
#include <initguid.h> // Need this for the GUIDs to be generated
#include <ks.h>       // Needs to be before ksmedia.h
#include <Ksmedia.h>  // Needs to be before bdamedia.h
#include <bdatypes.h> // Needs to be before bdamedia.h

#include <avc.h>
#include <Bdamedia.h>
#include <Bthdef.h>
#include <cfgmgr32.h>
#include <DbgHelp.h>
#include <Hidclass.h>
#include <ioevent.h>
#include <Ndisguid.h>
#include <Ntddkbd.h>
#include <Ntddmodm.h>
#include <Ntddmou.h>
#include <Ntddpar.h>
#pragma warning ( push )
#pragma warning( disable: 4091 )
#include <Ntddscsi.h> // Disable the warning this throws
#pragma warning( pop )
#include <Ntddstor.h>
#include <Ntddvdeo.h>
#include <PortableDevice.h>
#define _NTSCSI_USER_MODE_
#include <scsi.h>
#include <Sensors.h>
#include <SetupAPI.h>
#include <storduid.h>
#include <usbioctl.h>
#include <Usbiodef.h>
#include <Wiaintfc.h>

// This is missing from Ntddstor.h for some reason
typedef struct _MEDIA_SERIAL_NUMBER_DATA {
    ULONG SerialNumberLength;
    ULONG Result;
    ULONG Reserved[2];
    UCHAR SerialNumberData[1];
} MEDIA_SERIAL_NUMBER_DATA, *PMEDIA_SERIAL_NUMBER_DATA;

// An AttributeMap is a map of string to string
//   name of attribute -> string of attribute value
typedef std::map<std::string, std::string> AttributeMap;

// Used to map a DeviceId to a SCSI Port
typedef std::map<std::string, UINT16> DeviceIdToScsiPortMap;

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
// Used as a (sketchy) way of passing a DEVINST in a string vector
#define DEVINFO_DATA_STRING "__devInfoDataString"
// Possible aliases to SystemRoot
#define SYSTEM_ROOT1 "\\SystemRoot\\System32"
#define SYSTEM_ROOT2 "System32"
// Here to avoid warnings from importing ntddser.h
#define IOCTL_SERIAL_GET_COMMSTATUS	0x1b006c

// Get an HDEVINFO for all classes 
HDEVINFO getAllClassesHDevInfo();

// Get an HDEVINFO for all interfaces of a given GUID
HDEVINFO getInterfaceHDevInfo(GUID classGuid);

// Takes a complete HDEVINFO, specific device PSP_DEV_INFO_DATA, DWORD property and TYPE
// The TYPE is used to get a string representation of the given property
bool getDevInfoProperty(HDEVINFO &devs, PSP_DEVINFO_DATA devInfo, DWORD property, TYPE retType, std::string name, std::string description, cdi::attr::Attribute &attr);

// Takes a complete HDEVINFO, corresponding SP_DEVINFO_DATA. Returns True if we are able
//   to get driver information. If returning True, the PSP_DRVINFO_DATA is updated
bool getDriverInfoData(HDEVINFO devs, SP_DEVINFO_DATA devInfo, PSP_DRVINFO_DATA driverInfoData);

// Gets the Device Id for the given DEVINST
std::string getDeviceId(DEVINST &devInst);

// Takes a complete HDEVINFO and SP_DEVINFO_DATA and returns an AttributeSet
//   this is a collection of 'attributes'
cdi::attr::AttributeSet getDeviceAttributeSet(HDEVINFO devs, SP_DEVINFO_DATA devInfo, DeviceIdToScsiPortMap &deviceIdToScsiPortMap);

// Goes through all DevNode Propertiy Keys and add missing properties to the AttributeSet
void addOtherDevNodeProperties(cdi::attr::AttributeSet &attributeSet, DEVINST devInst);

// Goes through all resouce descriptors for the device and adds their info to the AttributeSet
void addDeviceConfigurationAndResources(cdi::attr::AttributeSet &attributeSet, DEVINST devInst);

// Goes through all resouce descriptors for the interface and adds their info to the AttributeSet
void addInterfaceConfigurationAndResources(cdi::attr::AttributeSet &attributeSet);

// Gets a vector of interfaces based on the given GUID. If the GUID is GUID_NULL, all DEVINTERFACE GUIDs will be used
cdi::attr::AttributeSetVector getInterfaceAttributeSet(GUID classGuid);

// Gets a vector of all devices and their AttributeSets
cdi::attr::AttributeSetVector getAllDevicesAttributeSet();

// Gets a mapping from Device Id to SCSI Port
DeviceIdToScsiPortMap getDeviceIdToScsiPortMap();

// Merges two AttributeSets. If anything differs adds on a new line
cdi::attr::AttributeSet &mergeAttributeSets(cdi::attr::AttributeSet &oldSet, cdi::attr::AttributeSet &newSet);

// Gets attributes via the DevicePath
cdi::attr::AttributeSet getAttributeSetFromDevicePath(std::string DevicePath, std::map<std::string, std::string> &msDosDeviceNameToDriveLetterMap);

// Gets a map from MSDosDeviceName to drive letter
std::map<std::string, std::string> getMsDosDeviceNameToDriveLetterMap();