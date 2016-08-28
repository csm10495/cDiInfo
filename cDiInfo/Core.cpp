/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Core.cpp - Core SetupDi-interfacing implementation file
*/

// Local includes
#include "Core.h"

// #defines
// Adds to the devAttrSet if getDevInfoProperty passes
#define addIfHave(devAttrSet, devs, devInfo, prop, retType, name, desc, attr) if (getDevInfoProperty(devs, &devInfo, prop, retType, name, desc, attr)) \
                                                                                        {devAttrSet.insert(attr);}
// Replaces an item in the AttributeSet. This removes the old one ane inserts the new one. It erases the one with the new name and replaces it with the new item.
#define replaceInAttributeSet(attributeSet, attr)  {attributeSet.erase(attr); attributeSet.insert(attr);}

// from various WinApi files where importing would get an error (for some reason)
DEFINE_GUID(GUID_DEVINTERFACE_I2C, 0x2564AA4F, 0xDDDB, 0x4495, 0xB4, 0x97, 0x6A, 0xD4, 0xA8, 0x41, 0x63, 0xD7);
DEFINE_GUID(GUID_DEVINTERFACE_OPM_2_JTP, 0xE929EEA4, 0xB9F1, 0x407B, 0xAA, 0xB9, 0xAB, 0x08, 0xBB, 0x44, 0xFB, 0xF4);
DEFINE_GUID(GUID_DEVINTERFACE_OPM_2, 0x7F098726, 0x2EBB, 0x4FF3, 0xA2, 0x7F, 0x10, 0x46, 0xB9, 0x5D, 0xC5, 0x17);
DEFINE_GUID(GUID_DEVINTERFACE_BRIGHTNESS, 0xFDE5BBA4, 0xB3F9, 0x46FB, 0xBD, 0xAA, 0x07, 0x28, 0xCE, 0x31, 0x00, 0xB4);
DEFINE_GUID(GUID_DEVINTERFACE_BRIGHTNESS_2, 0x148A3C98, 0x0ECD, 0x465A, 0xB6, 0x34, 0xB0, 0x5F, 0x19, 0x5F, 0x77, 0x39);
DEFINE_GUID(GUID_DEVINTERFACE_MIRACAST_DISPLAY, 0xaf03f190, 0x22af, 0x48cb, 0x94, 0xbb, 0xb7, 0x8e, 0x76, 0xa2, 0x51, 0x7);
DEFINE_GUID(BUS1394_CLASS_GUID, 0x6BDD1FC1, 0x810F, 0x11d0, 0xBE, 0xC7, 0x08, 0x00, 0x2B, 0xE2, 0x09, 0x2F);
DEFINE_GUID(GUID_61883_CLASS, 0x7ebefbc0, 0x3200, 0x11d2, 0xb4, 0xc2, 0x0, 0xa0, 0xc9, 0x69, 0x7d, 0x7);
DEFINE_GUID(GUID_DEVICE_BATTERY, 0x72631e54L, 0x78A4, 0x11d0, 0xbc, 0xf7, 0x00, 0xaa, 0x00, 0xb7, 0xb3, 0x2a);
DEFINE_GUID(GUID_DEVICE_APPLICATIONLAUNCH_BUTTON, 0x629758eel, 0x986e, 0x4d9e, 0x8e, 0x47, 0xde, 0x27, 0xf8, 0xab, 0x05, 0x4d);
DEFINE_GUID(GUID_DEVICE_SYS_BUTTON, 0x4AFA3D53L, 0x74A7, 0x11d0, 0xbe, 0x5e, 0x00, 0xA0, 0xC9, 0x06, 0x28, 0x57);
DEFINE_GUID(GUID_DEVICE_LID, 0x4AFA3D52L, 0x74A7, 0x11d0, 0xbe, 0x5e, 0x00, 0xA0, 0xC9, 0x06, 0x28, 0x57);
DEFINE_GUID(GUID_DEVICE_THERMAL_ZONE, 0x4AFA3D51L, 0x74A7, 0x11d0, 0xbe, 0x5e, 0x00, 0xA0, 0xC9, 0x06, 0x28, 0x57);
DEFINE_GUID(GUID_DEVICE_FAN, 0x05ecd13dL, 0x81da, 0x4a2a, 0x8a, 0x4c, 0x52, 0x4f, 0x23, 0xdd, 0x4d, 0xc9);
DEFINE_GUID(GUID_DEVICE_PROCESSOR, 0x97fadb10L, 0x4e33, 0x40ae, 0x35, 0x9c, 0x8b, 0xef, 0x02, 0x9d, 0xbd, 0xd0);
DEFINE_GUID(GUID_DEVICE_MEMORY, 0x3fd0f03dL, 0x92e0, 0x45fb, 0xb7, 0x5c, 0x5e, 0xd8, 0xff, 0xb0, 0x10, 0x21);
DEFINE_GUID(GUID_DEVICE_ACPI_TIME, 0x97f99bf6L, 0x4497, 0x4f18, 0xbb, 0x22, 0x4b, 0x9f, 0xb2, 0xfb, 0xef, 0x9c);
DEFINE_GUID(GUID_DEVICE_MESSAGE_INDICATOR, 0XCD48A365L, 0xfa94, 0x4ce2, 0xa2, 0x32, 0xa1, 0xb7, 0x64, 0xe5, 0xd8, 0xb4);
DEFINE_GUID(GUID_DEVINTERFACE_THERMAL_COOLING, 0xdbe4373d, 0x3c81, 0x40cb, 0xac, 0xe4, 0xe0, 0xe5, 0xd0, 0x5f, 0xc, 0x9f);
DEFINE_GUID(GUID_DEVINTERFACE_OPM, 0xBF4672DE, 0x6B4E, 0x4BE4, 0xA3, 0x25, 0x68, 0xA9, 0x1E, 0xA4, 0x9C, 0x09);
DEFINE_GUID(GUID_DEVINTERFACE_SIDESHOW, 0x152e5811, 0xfeb9, 0x4b00, 0x90, 0xf4, 0xd3, 0x29, 0x47, 0xae, 0x16, 0x81);
DEFINE_GUID(GUID_AEPSERVICE_WIFIDIRECT_DEVICE, 0xcc29827c, 0x9caf, 0x4928, 0x99, 0xa9, 0x18, 0xf7, 0xc2, 0x38, 0x13, 0x89);
DEFINE_GUID(GUID_DEVINTERFACE_WIFIDIRECT_DEVICE, 0x439b20af, 0x8955, 0x405b, 0x99, 0xf0, 0xa6, 0x2a, 0xf0, 0xc6, 0x8d, 0x43);
DEFINE_GUID(GUID_DEVINTERFACE_SMARTCARD_READER, 0x50DD5230, 0xBA8A, 0x11D1, 0xBF, 0x5D, 0x00, 0x00, 0xF8, 0x05, 0xF5, 0x30);
DEFINE_GUID(GUID_DEVINTERFACE_USB_HUB, 0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8);
DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);
DEFINE_GUID(GUID_DEVINTERFACE_USB_HOST_CONTROLLER, 0x3abf6f2d, 0x71c4, 0x462a, 0x8a, 0x92, 0x1e, 0x68, 0x61, 0xe6, 0xaf, 0x27);

// A list of all DEVINTERFACE GUIDs
const std::vector<GUID> ALL_GUIDS = {
    GUID_DEVINTERFACE_DISK,
    GUID_DEVINTERFACE_CDROM,
    GUID_DEVINTERFACE_PARTITION,
    GUID_DEVINTERFACE_TAPE,
    GUID_DEVINTERFACE_WRITEONCEDISK,
    GUID_DEVINTERFACE_VOLUME,
    GUID_DEVINTERFACE_MEDIUMCHANGER,
    GUID_DEVINTERFACE_FLOPPY,
    GUID_DEVINTERFACE_CDCHANGER,
    GUID_DEVINTERFACE_STORAGEPORT,
    GUID_DEVINTERFACE_VMLUN,
    GUID_DEVINTERFACE_SES,
    GUID_DEVINTERFACE_HIDDEN_VOLUME,
    GUID_DEVINTERFACE_COMPORT,
    GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR,
    GUID_DEVINTERFACE_USB_DEVICE,
    GUID_DEVINTERFACE_USB_HOST_CONTROLLER,
    GUID_DEVINTERFACE_USB_HUB,
    GUID_DEVINTERFACE_WPD,
    GUID_DEVINTERFACE_WPD_PRIVATE,
    GUID_DEVINTERFACE_NET,
    GUID_DEVINTERFACE_HID,
    GUID_DEVINTERFACE_KEYBOARD,
    GUID_DEVINTERFACE_MOUSE,
    GUID_DEVINTERFACE_MODEM,
    GUID_DEVINTERFACE_PARALLEL,
    GUID_DEVINTERFACE_SENSOR,
    GUID_DEVINTERFACE_VIDEO_OUTPUT_ARRIVAL,
    GUID_DEVINTERFACE_MONITOR,
    GUID_DEVINTERFACE_IMAGE,
    GUID_DEVINTERFACE_I2C,
    GUID_DEVINTERFACE_DISPLAY_ADAPTER,
    GUID_DEVINTERFACE_BRIGHTNESS,
    GUID_DEVINTERFACE_BRIGHTNESS_2,
    GUID_DEVINTERFACE_OPM_2_JTP,
    GUID_DEVINTERFACE_OPM_2,
    GUID_DEVINTERFACE_OPM,
    GUID_DEVINTERFACE_MIRACAST_DISPLAY,
    GUID_BTHPORT_DEVICE_INTERFACE,
    BUS1394_CLASS_GUID,
    GUID_61883_CLASS,
    GUID_DEVICE_APPLICATIONLAUNCH_BUTTON,
    GUID_DEVICE_BATTERY,
    GUID_DEVICE_SYS_BUTTON,
    GUID_DEVICE_LID,
    GUID_DEVICE_THERMAL_ZONE,
    GUID_DEVICE_FAN,
    GUID_DEVICE_PROCESSOR,
    GUID_DEVICE_MEMORY,
    GUID_DEVICE_ACPI_TIME,
    GUID_DEVICE_MESSAGE_INDICATOR,
    GUID_DEVINTERFACE_THERMAL_COOLING,
    GUID_DISPLAY_DEVICE_ARRIVAL,
    GUID_DEVINTERFACE_PARCLASS,
    GUID_DEVINTERFACE_SIDESHOW,
    GUID_AEPSERVICE_WIFIDIRECT_DEVICE,
    GUID_DEVINTERFACE_WIFIDIRECT_DEVICE,
    GUID_DEVINTERFACE_SMARTCARD_READER,
    GUID_AVC_CLASS,
    GUID_VIRTUAL_AVC_CLASS,

    // Deprecated KSCAT_
    KSCATEGORY_AUDIO,
    KSCATEGORY_ACOUSTIC_ECHO_CANCEL,
    KSCATEGORY_IP_SINK,
    KSCATEGORY_BDA_NETWORK_EPG,
    KSCATEGORY_BDA_NETWORK_PROVIDER,
    KSCATEGORY_BDA_NETWORK_TUNER,
    KSCATEGORY_BDA_RECEIVER_COMPONENT,
    KSCATEGORY_BDA_TRANSPORT_INFORMATION,
    KSCATEGORY_BRIDGE,
    KSCATEGORY_CAPTURE,
    KSCATEGORY_CLOCK,
    KSCATEGORY_COMMUNICATIONSTRANSFORM,
    KSCATEGORY_CROSSBAR,
    KSCATEGORY_DATACOMPRESSOR,
    KSCATEGORY_DATADECOMPRESSOR,
    KSCATEGORY_DATATRANSFORM,
    KSCATEGORY_DRM_DESCRAMBLE,
    KSCATEGORY_ENCODER,
    KSCATEGORY_ESCALANTE_PLATFORM_DRIVER,
    KSCATEGORY_FILESYSTEM,
    KSCATEGORY_INTERFACETRANSFORM,
    KSCATEGORY_MEDIUMTRANSFORM,
    KSCATEGORY_MICROPHONE_ARRAY_PROCESSOR,
    KSCATEGORY_MIXER,
    KSCATEGORY_MULTIPLEXER,
    KSCATEGORY_NETWORK,
    KSCATEGORY_PROXY,
    KSCATEGORY_QUALITY,
    KSCATEGORY_REALTIME,
    KSCATEGORY_RENDER,
    KSCATEGORY_SPLITTER,
    KSCATEGORY_SYNTHESIZER,
    KSCATEGORY_TEXT,
    KSCATEGORY_TOPOLOGY,
    KSCATEGORY_TVAUDIO,
    KSCATEGORY_TVTUNER,
    KSCATEGORY_VBICODEC,
    KSCATEGORY_VIDEO,
    KSCATEGORY_VIRTUAL,
    KSCATEGORY_VPMUX,

    KSMFT_CATEGORY_AUDIO_DECODER,
    KSMFT_CATEGORY_AUDIO_EFFECT,
    KSMFT_CATEGORY_AUDIO_ENCODER,
    KSMFT_CATEGORY_DEMULTIPLEXER,
    KSMFT_CATEGORY_MULTIPLEXER,
    KSMFT_CATEGORY_OTHER,
    KSMFT_CATEGORY_VIDEO_DECODER,
    KSMFT_CATEGORY_VIDEO_EFFECT,
    KSMFT_CATEGORY_VIDEO_ENCODER,
    KSMFT_CATEGORY_VIDEO_PROCESSOR,
};

HDEVINFO getAllClassesHDevInfo()
{
    HDEVINFO devs = INVALID_HANDLE_VALUE;

    devs = SetupDiGetClassDevsEx(NULL,
        NULL,
        NULL,
        DIGCF_ALLCLASSES | DIGCF_PRESENT,
        NULL,
        NULL,
        NULL);

    return devs;
}

HDEVINFO getInterfaceHDevInfo(GUID classGuid)
{
    HDEVINFO devs = INVALID_HANDLE_VALUE;

    devs = SetupDiGetClassDevsEx(&classGuid,
        NULL,
        NULL,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE,
        NULL,
        NULL,
        NULL);

    return devs;
}

bool getDevInfoProperty(HDEVINFO &devs, PSP_DEVINFO_DATA devInfo, DWORD property, TYPE retType, std::string name, std::string description, cdi::attr::Attribute &attr)
{
    char buffer[8192] = { 0 };
    DWORD reqSize;
    DWORD dataType;

    if (SetupDiGetDeviceRegistryProperty(devs, devInfo, property, &dataType, (LPBYTE)buffer, 8192, &reqSize))
    {
        // Special handling since this is a structure...
        if (property == SPDRP_DEVICE_POWER_DATA)
        {
            attr = cdi::attr::Attribute((BYTE*)buffer, reqSize, name, description, cdi::strings::propertyBufferToString((BYTE*)buffer, reqSize, DEVPROP_TYPE_BINARY, DEVPKEY_Device_PowerData));
            return true;
        }

        std::string retStr = std::string(buffer, reqSize);
        if (retType == _INT_)
        {
            attr = cdi::attr::Attribute((BYTE*)buffer, reqSize, name, description, std::to_string(*(DWORD*)retStr.c_str()));
            return true;
        }
        else if (retType == __STRING_)
        {
            attr = cdi::attr::Attribute((BYTE*)buffer, reqSize, name, description, cdi::strings::delimitedStringToNewlineString(retStr));
            return true;
        }
        else if (retType == _GUID_)
        {
            GUID g = *((GUID*)retStr.c_str());
            attr = cdi::attr::Attribute((BYTE*)buffer, reqSize, name, description, cdi::strings::guidToString(g));
            return true;
        }
        else if (retType == __WSTRING_)
        {
            std::wstring tmpWStr((wchar_t*)retStr.c_str());
            attr = cdi::attr::Attribute((BYTE*)buffer, reqSize, name, description, cdi::strings::wStringToString(tmpWStr));
            return true;
        }
    }
    return false;
}

bool getDriverInfoData(HDEVINFO devs, SP_DEVINFO_DATA devInfo, PSP_DRVINFO_DATA driverInfoData)
{
    SP_DEVINSTALL_PARAMS deviceInstallParams = { 0 };
    deviceInstallParams.cbSize = sizeof(SP_DEVINSTALL_PARAMS);

    if (!SetupDiGetDeviceInstallParams(devs, &devInfo, &deviceInstallParams)) {
        return false;
    }

    deviceInstallParams.FlagsEx |= (DI_FLAGSEX_INSTALLEDDRIVER | DI_FLAGSEX_ALLOWEXCLUDEDDRVS);

    if (SetupDiSetDeviceInstallParams(devs, &devInfo, &deviceInstallParams))
    {
        //
        // we were able to specify this flag, so proceed the easy way
        // we should get a list of no more than 1 driver
        //
        if (!SetupDiBuildDriverInfoList(devs, &devInfo, SPDIT_CLASSDRIVER)) {
            return false;
        }
        if (!SetupDiEnumDriverInfo(devs, &devInfo, SPDIT_CLASSDRIVER,
            0, driverInfoData)) {
            return false;
        }
        //
        // we've selected the current driver
        //
        return true;
    }

    return false;

}

std::string getDeviceId(DEVINST &devInst)
{
    ULONG deviceIdSize;
    if (CM_Get_Device_ID_Size(&deviceIdSize, devInst, 0) == CR_SUCCESS)
    {
        std::string deviceId(" ", deviceIdSize + 1);
        if (CM_Get_Device_ID(devInst, (char*)(deviceId.c_str()), deviceIdSize + 1, 0) == CR_SUCCESS)
        {
            deviceId[deviceIdSize] = ' ';
            return cdi::strings::rTrim(deviceId);
        }
    }
    return UNAVAILABLE_ATTRIBUTE;
}

cdi::attr::AttributeSet getDeviceAttributeSet(HDEVINFO devs, SP_DEVINFO_DATA devInfo, DeviceIdToScsiPortMap &deviceIdToScsiPortMap)
{
    cdi::attr::AttributeSet devAttrSet;

    cdi::attr::Attribute attr;

    // Todo: cleanup parsing of these... A lot of them can have string parsings
    addIfHave(devAttrSet, devs, devInfo, SPDRP_DEVICEDESC, __STRING_, "DeviceDesc", "A description for the device.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_HARDWAREID, __STRING_, "HardwareId", "Vendor defined identification string used to match a device to an inf file to denote hardware compatibility.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_FRIENDLYNAME, __STRING_, "FriendlyName", "A user-friendly name for a device.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_COMPATIBLEIDS, __STRING_, "CompatibleIds", "Vendor defined identification string used to match a device to an inf file in order of decreasing suitability. Used after no HardwareId matches.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_UNUSED0, _INT_, "Unused0", "In theory, should be unsused. Could be used for vendor unique purposes.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_SERVICE, __STRING_, "Service", "The service name for the device driver.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_UNUSED1, _INT_, "Unused1", "In theory, should be unsused. Could be used for vendor unique purposes.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_UNUSED2, _INT_, "Unused2", "In theory, should be unsused. Could be used for vendor unique purposes.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_CLASS, __STRING_, "Class", "The device setup class name.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_CLASSGUID, __STRING_, "ClassGuid", "A globally unique identifier for the device setup class.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_DRIVER, __STRING_, "DriverKey", "The registry software key for the given driver.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_CONFIGFLAGS, _INT_, "ConfigFlags", "Bitwise or of the device's configuration flags.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_MFG, __STRING_, "Manufacturer", "The device manufacturer.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_LOCATION_INFORMATION, __STRING_, "LocationInformation", "The hardware location of the device.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, __STRING_, "PhysicalDeviceObjectName", "The name associated with the device's physical device object.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_CAPABILITIES, _INT_, "Capabilities", "Bitwise or of device capability flags.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_UI_NUMBER, _INT_, "UiNumber", "A number associated with the device. Usually it is a user-perceived slot number.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_UPPERFILTERS, __STRING_, "UpperFilterDrivers", "Names of the device's upper filter drivers.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_LOWERFILTERS, __STRING_, "LowerFilterDrivers", "Names of the device's upper filter drivers.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_BUSTYPEGUID, _GUID_, "BusTypeGuid", "Globally unique identifier for the device's bus.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_LEGACYBUSTYPE, _INT_, "LegacyBusType", "Legacy bus type of the device instance.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_BUSNUMBER, _INT_, "BusNumber", "Bus number of the specified bus type that this device instance represents.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_ENUMERATOR_NAME, __STRING_, "EnumeratorName", "Name of the enumerator for the device instance", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_SECURITY, _INT_, "SecurityDescriptor", "Contains security information associated with the object.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_SECURITY_SDS, __STRING_, "SecurityDescriptorString", "Contains security information associated with the object as a string.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_DEVTYPE, _INT_, "DeviceType", "The type of the underlying hardware for the driver.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_EXCLUSIVE, _INT_, "Exclusive", "If True, the device can be opened for exclusive use.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_CHARACTERISTICS, _INT_, "Characteristics", "A bitwise or of the device characteristic flags.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_ADDRESS, _INT_, "Address", "Bus specific address of the device instance.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_UI_NUMBER_DESC_FORMAT, __STRING_, "UiNumberDescFormat", "A printf-compatible format string used to display the UiNumber.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_DEVICE_POWER_DATA, _INT_, "PowerData", "Includes a CM_POWER_DATA structure with information about device power management and capabilities.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_REMOVAL_POLICY, _INT_, "RemovalPolicy", "A device property representing the current removal policy for the device instance.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_REMOVAL_POLICY_HW_DEFAULT, _INT_, "RemovalPolicyHwDefault", "A device property representing the HW default removal policy for the device instance.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_REMOVAL_POLICY_OVERRIDE, _INT_, "RemovalPolicyOverride", "A device property representing the override removal policy for the device instance.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_INSTALL_STATE, _INT_, "InstallState", "The installation state of the device instance.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_LOCATION_PATHS, __STRING_, "LocationPaths", "The location of the device instance in the device tree.", attr);
    addIfHave(devAttrSet, devs, devInfo, SPDRP_BASE_CONTAINERID, __WSTRING_, "BaseContainerId", "GUID value of the base container identifer.", attr);

    auto descAttr = devAttrSet.find(ATTRWITHNAME("UiNumberDescFormat"));
    auto numberAttr = devAttrSet.find(ATTRWITHNAME("UiNumber"));
    if (descAttr != devAttrSet.end() && numberAttr != devAttrSet.end())
    {
        char c[4096] = { '\0' };
        sprintf_s(c, descAttr->getValue<char*>(), numberAttr->getValue<char*>());
        devAttrSet.insert(cdi::attr::Attribute("UiNumberFormatted", "The current, unique, identifier for the instance of this device.", c));
    }

    std::string DeviceId = getDeviceId(devInfo.DevInst);
    devAttrSet.insert(cdi::attr::Attribute("DeviceId", "The current, unique, identifier for the instance of this device.", DeviceId));

    std::string ChildrenDeviceIds = "";
    DEVINST childDevInst = { 0 };
    if (CM_Get_Child(&childDevInst, devInfo.DevInst, 0) == CR_SUCCESS)
    {
        ChildrenDeviceIds = "";
        ChildrenDeviceIds += getDeviceId(childDevInst) + "\n";
        while (CM_Get_Sibling(&childDevInst, childDevInst, 0) != CR_NO_SUCH_DEVNODE)
        {
            ChildrenDeviceIds += getDeviceId(childDevInst) + "\n";
        }
    }
    cdi::strings::rTrim(ChildrenDeviceIds);
    if (!ChildrenDeviceIds.empty())
    {
        devAttrSet.insert(cdi::attr::Attribute("ChildrenDeviceIds", "The current, unique, identifiers for all children of this device.", ChildrenDeviceIds));
    }

    std::string ParentDeviceId = "";
    DEVINST parentDevInst = { 0 };
    if (CM_Get_Parent(&parentDevInst, devInfo.DevInst, 0) == CR_SUCCESS)
    {
        ParentDeviceId = getDeviceId(parentDevInst);
    }
    cdi::strings::rTrim(ParentDeviceId);
    if (!ParentDeviceId.empty())
    {
        devAttrSet.insert(cdi::attr::Attribute("ParentDeviceId", "The current, unique, identifers for all parents of this device.", ParentDeviceId));
    }

    std::string SiblingDeviceIds = "";
    // Make a copy to not mess up the standard DevInst
    SP_DEVINFO_DATA sib;
    memcpy(&sib, &devInfo, sizeof(SP_DEVINFO_DATA));
    while (CM_Get_Sibling(&sib.DevInst, sib.DevInst, 0) == CR_SUCCESS)
    {
        SiblingDeviceIds += getDeviceId(sib.DevInst) + "\n";
    }
    if (SiblingDeviceIds.empty())
    {
        SiblingDeviceIds = NO_SIBLINGS;
    }
    cdi::strings::rTrim(SiblingDeviceIds);
    devAttrSet.insert(cdi::attr::Attribute("SiblingDeviceIds", "The current, unique, identifers for all siblings of this device.", SiblingDeviceIds));

    ULONG pulStatus = 0;
    ULONG pulProblemNumber = 0;
    std::string NodeStatus = UNAVAILABLE_ATTRIBUTE;
    std::string NodeProblem = UNAVAILABLE_ATTRIBUTE;
    auto ret = CM_Get_DevNode_Status(&pulStatus, &pulProblemNumber, devInfo.DevInst, 0);
    if (ret == CR_SUCCESS)
    {
        NodeStatus = cdi::strings::nodeStatusToString(pulStatus);
        devAttrSet.insert(cdi::attr::Attribute((BYTE*)&pulStatus, sizeof(ULONG), "NodeStatus", "Status for the device node in the local machine's device tree", NodeStatus));
        if (pulStatus & DN_HAS_PROBLEM)
        {
            NodeProblem = cdi::strings::cmProbToString(pulProblemNumber);
        }
        else
        {
            NodeProblem = NO_PROBLEM;
        }
        devAttrSet.insert(cdi::attr::Attribute((BYTE*)&pulProblemNumber, sizeof(ULONG), "NodeProblem", "Problem with the device node in the local machine's device tree", NodeProblem));
    }

    ULONG pulDepth = 0;
    if (CM_Get_Depth(&pulDepth, devInfo.DevInst, 0) == CR_SUCCESS)
    {
        devAttrSet.insert(cdi::attr::Attribute((BYTE*)&pulDepth, sizeof(ULONG), "NodeDepth", "Depth of the device node in the local machine's device tree", std::to_string(pulDepth)));
    }

    SP_DRVINFO_DATA driverInfoData = { 0 };
    driverInfoData.cbSize = sizeof(driverInfoData);

    if (getDriverInfoData(devs, devInfo, &driverInfoData))
    {
        devAttrSet.insert(cdi::attr::Attribute("DriverDescription", "A description for the device driver.", driverInfoData.Description));

        SYSTEMTIME st = { 0 };
        FileTimeToSystemTime(&driverInfoData.DriverDate, &st);
        std::string DriverDate = std::to_string(st.wMonth) + "/" + std::to_string(st.wDay) + "/" + std::to_string(st.wYear);
        devAttrSet.insert(cdi::attr::Attribute((BYTE*)&driverInfoData.DriverDate, sizeof(FILETIME), "DriverDate", "Build date for the device driver.", DriverDate));

        std::string DriverVersion = std::to_string((driverInfoData.DriverVersion >> 48) & 0xFFFF) + "." + \
            std::to_string((driverInfoData.DriverVersion >> 32) & 0xFFFF) + "." + \
            std::to_string((driverInfoData.DriverVersion >> 16) & 0xFFFF) + "." + \
            std::to_string((driverInfoData.DriverVersion) & 0xFFFF);
        devAttrSet.insert(cdi::attr::Attribute((BYTE*)&driverInfoData.DriverVersion, sizeof(UINT64), "DriverVersion", "Version for the given device driver.", DriverVersion));


        devAttrSet.insert(cdi::attr::Attribute("DriverMfgName", "The manufacturer of the device driver.", driverInfoData.MfgName));
        devAttrSet.insert(cdi::attr::Attribute("DriverProvider", "The manufacturer of the device driver.", driverInfoData.ProviderName));
        devAttrSet.insert(cdi::attr::Attribute((BYTE*)&driverInfoData.DriverType, sizeof(DWORD), "DriverType", "Represents if the device is a compatible or class driver", cdi::strings::driverTypeToString(driverInfoData.DriverType)));
    }

    // Try to get SCSI port info (This is for interface-less devices... it's disks already have interfaces with device paths that can get SCSI info)
    auto itr = deviceIdToScsiPortMap.find(DeviceId);
    if (itr != deviceIdToScsiPortMap.end())
    {
        std::string ScsiPortNumber = std::to_string(itr->second);
        std::string ScsiAdapterPath = "\\\\.\\SCSI" + ScsiPortNumber + ":";
        devAttrSet.insert(cdi::attr::Attribute("ScsiAdapterPath", "SCSI path for the device's adapter.", ScsiAdapterPath));
        devAttrSet.insert(cdi::attr::Attribute((BYTE*)&itr->second, sizeof(UINT16), "ScsiPortNumber", "Represents if the device is a compatible or class driver", ScsiPortNumber));
    }

    // Try to save the SP_DEVINFO_DATA... a bit sketchy but works!
    std::string __devInfoDataString(sizeof(SP_DEVINFO_DATA), '0');
    memcpy((void*)__devInfoDataString.c_str(), &devInfo, sizeof(SP_DEVINFO_DATA));
    devAttrSet.insert(cdi::attr::Attribute("__devInfoDataString", "A SP_DEVINFO_DATA structure for the given device instance.", __devInfoDataString));


    // Get DevNode Property Keys
    addOtherDevNodeProperties(devAttrSet, devInfo.DevInst);

    // Get Resource Descriptor Data
    addDeviceConfigurationAndResources(devAttrSet, devInfo.DevInst);

    // Get service information from the registry
    auto service = devAttrSet.find(ATTRWITHNAME("Service"));
    if (service != devAttrSet.end())
    {
        std::string subKey = REGISTRY_SERVICES + service->getValue<std::string>();
        std::string ServiceImagePath = "";
        if (cdi::registry::getStringFromRegistry(HKEY_LOCAL_MACHINE, subKey, "ImagePath", ServiceImagePath))
        {
            // Sometimes this path will start with \\SystemRoot\\System32 and sometimes \\System32\\
            //   Try to standardize it.
            char systemDirectory[MAX_PATH] = { '\0' };
            if (GetSystemDirectory(systemDirectory, MAX_PATH))
            {
                if (cdi::strings::startsWith(SYSTEM_ROOT1, ServiceImagePath, true))
                {
                    ServiceImagePath = systemDirectory + ServiceImagePath.substr(strlen(SYSTEM_ROOT1));
                }
                else if (cdi::strings::startsWith(SYSTEM_ROOT2, ServiceImagePath, true))
                {
                    ServiceImagePath = systemDirectory + ServiceImagePath.substr(strlen(SYSTEM_ROOT2));
                }
            }
            devAttrSet.insert(cdi::attr::Attribute("ServiceImagePath", "The location of the exe or driver for the given device.", ServiceImagePath));
        }

        UINT64 u = 0;
        if (cdi::registry::getUIntFromRegistry(HKEY_LOCAL_MACHINE, subKey, "ErrorControl", u))
        {
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&u, sizeof(DWORD), "ServiceErrorControl", "The location of the exe or driver for the given device.", cdi::strings::errorControlToString((DWORD)u)));
        }

        u = 0;
        if (cdi::registry::getUIntFromRegistry(HKEY_LOCAL_MACHINE, subKey, "Start", u))
        {
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&u, sizeof(DWORD), "ServiceStartType", "Specifies how the device is started.", cdi::strings::startTypeToString((DWORD)u)));
        }
        u = 0;
        if (cdi::registry::getUIntFromRegistry(HKEY_LOCAL_MACHINE, subKey, "Type", u))
        {
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&u, sizeof(DWORD), "ServiceType", "Specifies the type of the device driver.", cdi::strings::serviceTypeToString((DWORD)u)));
        }

        std::string ObjectName = "";
        if (cdi::registry::getStringFromRegistry(HKEY_LOCAL_MACHINE, subKey, "ObjectName", ObjectName))
        {
            devAttrSet.insert(cdi::attr::Attribute("ObjectName", "The name of the driver object that the I/O Manager uses to load the device driver.", ObjectName));
        }
    }

    return devAttrSet;
}

void addOtherDevNodeProperties(cdi::attr::AttributeSet &attributeSet, DEVINST devInst)
{
    // First get number of properties
    ULONG propertyKeyCount = 0;
    CONFIGRET retVal = CM_Get_DevNode_Property_Keys(devInst, NULL, &propertyKeyCount, 0);
    if (retVal != CR_BUFFER_SMALL)
    {
        return;
    }

    // delete me later!
    DEVPROPKEY *propertyKeyArray = new DEVPROPKEY[propertyKeyCount];

    if (CM_Get_DevNode_Property_Keys(devInst, propertyKeyArray, &propertyKeyCount, 0) != CR_SUCCESS)
    {
        goto end;
    }

    size_t numUnknownKeys = 0;
    for (size_t i = 0; i < propertyKeyCount; i++)
    {
        DEVPROPTYPE propertyType;
        ULONG propertyBufferSize = 0;
        if (CM_Get_DevNode_PropertyW(devInst, &propertyKeyArray[i], &propertyType, NULL, &propertyBufferSize, 0) != CR_BUFFER_SMALL)
        {
            continue;
        }

        BYTE* propertyBuffer = new BYTE[propertyBufferSize];
        if (CM_Get_DevNode_PropertyW(devInst, &propertyKeyArray[i], &propertyType, propertyBuffer, &propertyBufferSize, 0) != CR_SUCCESS)
        {
            delete[] propertyBuffer;
            continue;
        }

        std::string key = cdi::strings::propertyKeyToString(propertyKeyArray[i]);
        std::string value = cdi::strings::propertyBufferToString(propertyBuffer, propertyBufferSize, propertyType, propertyKeyArray[i]);
        std::string modifiedKey;
        size_t underscoreLoc = key.find("_") + 1;
        if (underscoreLoc == std::string::npos)
        {
            modifiedKey = key;
        }
        else
        {
            modifiedKey = key.substr(underscoreLoc);
        }

        if (attributeSet.find(ATTRWITHNAME(modifiedKey)) == attributeSet.end())
        {
            attributeSet.insert(cdi::attr::Attribute(propertyBuffer, propertyBufferSize, modifiedKey, key, value));
        }
        else // the modified key is in the map already. If it's value matches this one don't add.
             // If the value doesn't match add this value under the original (non-modified) key
        {
            if (cdi::strings::toUpper(std::string(attributeSet.find(ATTRWITHNAME(modifiedKey))->getValue<std::string>())) != cdi::strings::toUpper(std::string(value)))
            {
                cdi::attr::Attribute newAttr = cdi::attr::Attribute(*attributeSet.find(ATTRWITHNAME(modifiedKey)), "\n" + value);
                replaceInAttributeSet(attributeSet, newAttr);
            }

        }

        delete[] propertyBuffer;
    }

end:
    delete[] propertyKeyArray;
}

void addDeviceConfigurationAndResources(cdi::attr::AttributeSet &attributeSet, DEVINST devInst)
{
    LOG_CONF  firstLogConf;
    CONFIGRET cmRet = CM_Get_First_Log_Conf(&firstLogConf, devInst, ALLOC_LOG_CONF);
    if (cmRet != CR_SUCCESS)
    {
        return;
    };

    while (true)
    {
        LOG_CONF nextLogConf;
        RESOURCEID resourceType;
        cmRet = CM_Get_Next_Res_Des(&nextLogConf, firstLogConf, ResType_All, &resourceType, 0);
        if (cmRet != CR_SUCCESS)
        {
            CM_Free_Res_Des_Handle(firstLogConf);
            CM_Free_Log_Conf_Handle(firstLogConf);
            firstLogConf = NULL;
            return;
        };

        while (true)
        {
            ULONG bufferSize = 0;
            cmRet = CM_Get_Res_Des_Data_Size(&bufferSize, nextLogConf, 0);
            if (cmRet != CR_SUCCESS)
            {
                CM_Free_Res_Des_Handle(nextLogConf);
                nextLogConf = NULL;
                break;
            };

            BYTE* buffer = new BYTE[bufferSize];
            cmRet = CM_Get_Res_Des_Data(nextLogConf, buffer, bufferSize, 0);
            if (cmRet != CR_SUCCESS)
            {
                CM_Free_Res_Des_Handle(nextLogConf);
                nextLogConf = NULL;
                delete[] buffer;
                continue;
            }

            std::string resourceTypeAsString = cdi::strings::resourceTypeToString(resourceType);
            std::string resourceTypeAsStringWithNumber = resourceTypeAsString + "0";
            std::string resourceAsString = cdi::strings::resourceToString(buffer, bufferSize, resourceType);

            while (attributeSet.find(ATTRWITHNAME(resourceTypeAsStringWithNumber)) != attributeSet.end())
            {
                int endNum = atoi(resourceTypeAsStringWithNumber.substr(resourceTypeAsString.size()).c_str());
                endNum++;
                resourceTypeAsStringWithNumber = resourceTypeAsString + std::to_string(endNum);
            }
            attributeSet.insert(cdi::attr::Attribute(buffer, bufferSize, resourceTypeAsStringWithNumber, cdi::strings::resourceTypeToDescription(resourceType), resourceAsString));
            delete[] buffer;

            LOG_CONF old = nextLogConf;
            if (CM_Get_Next_Res_Des(&nextLogConf, nextLogConf, ResType_All, &resourceType, 0) != CR_SUCCESS)
            {
                CM_Free_Res_Des_Handle(nextLogConf);
                nextLogConf = NULL;
                break;
            }
            else
            {
                CM_Free_Res_Des_Handle(old);
                old = NULL;
            }
        }

        auto a = CM_Get_Next_Log_Conf(&nextLogConf, firstLogConf, 0);
        if (a != CR_SUCCESS)
        {
            break;
        }

    }
    CM_Free_Log_Conf_Handle(firstLogConf);
    firstLogConf = NULL;
}

void addInterfaceConfigurationAndResources(cdi::attr::AttributeSet &attributeSet)
{
    // Make sure we have an interface path
    auto itr = attributeSet.find(ATTRWITHNAME("DevicePath"));
    if (itr == attributeSet.end())
    {
        return;
    }
    std::wstring interfacePath = cdi::strings::stringToWString(itr->getValue<std::string>());

    // Get size of array
    ULONG propertyKeyCount = 0;
    if (CM_Get_Device_Interface_Property_KeysW(interfacePath.c_str(), NULL, &propertyKeyCount, 0) != CR_BUFFER_SMALL)
    {
        return;
    }

    // Get the array
    DEVPROPKEY* propertyKeyArray = new DEVPROPKEY[propertyKeyCount];
    if (CM_Get_Device_Interface_Property_KeysW(interfacePath.c_str(), propertyKeyArray, &propertyKeyCount, 0) != CR_SUCCESS)
    {
        delete[] propertyKeyArray;
        return;
    }

    ULONG propertyBufferSize = 0;
    // Get property values
    for (size_t i = 0; i < propertyKeyCount; i++)
    {
        DEVPROPTYPE propertyType;
        if (CM_Get_Device_Interface_PropertyW(interfacePath.c_str(), &propertyKeyArray[i], &propertyType, NULL, &propertyBufferSize, 0) != CR_BUFFER_SMALL)
        {
            continue;
        }

        BYTE* propertyBuffer = new BYTE[propertyBufferSize];
        if (CM_Get_Device_Interface_PropertyW(interfacePath.c_str(), &propertyKeyArray[i], &propertyType, propertyBuffer, &propertyBufferSize, 0) != CR_SUCCESS)
        {
            delete[] propertyBuffer;
            continue;
        }

        std::string key = cdi::strings::propertyKeyToString(propertyKeyArray[i]);
        std::string value = cdi::strings::propertyBufferToString(propertyBuffer, propertyBufferSize, propertyType, propertyKeyArray[i]);
        std::string modifiedKey;
        size_t underscoreLoc = key.find("_") + 1;
        if (underscoreLoc == std::string::npos)
        {
            modifiedKey = key;
        }
        else
        {
            modifiedKey = key.substr(underscoreLoc);
        }

        if (attributeSet.find(ATTRWITHNAME(modifiedKey)) == attributeSet.end())
        {
            attributeSet.insert(cdi::attr::Attribute(propertyBuffer, propertyBufferSize, modifiedKey, key, value));
        }
        else // the modified key is in the Set already. If it's value matches this one don't add.
             // If the value doesn't match add this value under the original (non-modified) key
        {
            if (cdi::strings::toUpper(std::string(attributeSet.find(ATTRWITHNAME(modifiedKey))->getValue<std::string>())) != cdi::strings::toUpper(std::string(value)))
            {
                cdi::attr::Attribute newAttr = cdi::attr::Attribute(*attributeSet.find(ATTRWITHNAME(modifiedKey)), "\n" + value);
                replaceInAttributeSet(attributeSet, newAttr);
            }
        }
        delete[] propertyBuffer;
    }

    delete[] propertyKeyArray;
}

std::vector<cdi::attr::AttributeSet> getInterfaceAttributeSet(GUID classGuid)
{
    std::vector<cdi::attr::AttributeSet> interfaces;

    if (classGuid == GUID_NULL)
    {
#ifdef MULTITHREADED
        std::vector<std::future<std::vector<cdi::attr::AttributeSet>>> vectorFutureVectorOfAttributeSets;
#else // SINGLETHEADED
        std::vector<std::vector<cdi::attr::AttributeSet>> vectorOfVectorOfAttributeSets;
#endif // SINGLETHEADED
        for (auto guid : ALL_GUIDS)
        {
#ifdef MULTITHREADED
            // multithreading
            vectorFutureVectorOfAttributeSets.push_back(std::async(getInterfaceAttributeSet, guid));
#else // SINGLETHEADED
            vectorOfVectorOfAttributeSets.push_back(getInterfaceAttributeSet(guid));
#endif // SINGLETHEADED
        }

#ifdef MULTITHREADED
        // join
        for (std::future<std::vector<cdi::attr::AttributeSet>> &futureVectorOfAttributeSets : vectorFutureVectorOfAttributeSets)
        {
            for (cdi::attr::AttributeSet &attributeSet : futureVectorOfAttributeSets.get())
            {
#else // SINGLETHEADED
        for (std::vector<cdi::attr::AttributeSet> &vectorOfAttributeSets : vectorOfVectorOfAttributeSets)
        {
            for (cdi::attr::AttributeSet &attributeSet : vectorOfAttributeSets)
            {
#endif // SINGLETHEADED
                bool merged = false;
                for (cdi::attr::AttributeSet &interfaceSet : interfaces)
                {
                    if (attributeSet.find(ATTRWITHNAME("DeviceId"))->getValue<std::string>() == interfaceSet.find(ATTRWITHNAME("DeviceId"))->getValue<std::string>())
                    {
                        attributeSet = mergeAttributeSets(interfaceSet, attributeSet);
                        merged = true;
                        break;
                    }
                }

                if (!merged)
                {
                    interfaces.push_back(attributeSet);
                }
            }
        }

        return interfaces;
    }

    HDEVINFO interfaceDevs = getInterfaceHDevInfo(classGuid);
    DeviceIdToScsiPortMap deviceIdToScsiPortMap = getDeviceIdToScsiPortMap();
    std::map<std::string, std::string> msDosDeviceNameToDriveLetterMap = getMsDosDeviceNameToDriveLetterMap();

    if (interfaceDevs == INVALID_HANDLE_VALUE)
    {
        return std::vector<cdi::attr::AttributeSet>();
    }

    SP_DEVINFO_DATA devInfo;
    devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

    SP_DEVICE_INTERFACE_DATA interfaceInfo;
    interfaceInfo.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    PSP_DEVICE_INTERFACE_DETAIL_DATA interfaceDetail = { 0 };

    DWORD size = 0;

    char * buffer = NULL;

    // In theory we would get device paths here...
    for (int interfaceIndex = 0; ;interfaceIndex++)
    {
        BOOL result = SetupDiEnumDeviceInfo(interfaceDevs, interfaceIndex, &devInfo);

        if (!result)
        {
            if (GetLastError() == ERROR_NO_MORE_ITEMS)
            {
                break;
            }
            continue;
        }

        result = SetupDiEnumDeviceInterfaces(interfaceDevs, NULL, &classGuid, interfaceIndex, &interfaceInfo);

        if (!result)
        {
            if (GetLastError() == ERROR_NO_MORE_ITEMS)
            {
                break;
            }
            continue;
        }

        SetupDiGetDeviceInterfaceDetail(interfaceDevs, &interfaceInfo, NULL, 0, &size, NULL);

        buffer = new char[size];
        interfaceDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)buffer;
        interfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        if (SetupDiGetDeviceInterfaceDetail(interfaceDevs, &interfaceInfo, interfaceDetail, size, NULL, &devInfo))
        {
            cdi::attr::AttributeSet devAttrSet;
            std::string DevicePath = interfaceDetail->DevicePath;

#ifdef MULTITHREADED
            std::future<cdi::attr::AttributeSet> futureDevAttrSet = std::async(getDeviceAttributeSet, interfaceDevs, devInfo, ref(deviceIdToScsiPortMap));
#else // SINGLETHREADED
            devAttrSet = getDeviceAttributeSet(interfaceDevs, devInfo, deviceIdToScsiPortMap);
#endif // SINGLETHREADED

            cdi::attr::AttributeSet otherAttrSet = getAttributeSetFromDevicePath(DevicePath, msDosDeviceNameToDriveLetterMap);

#ifdef MULTITHREADED
            devAttrSet = mergeAttributeSets(futureDevAttrSet.get(), otherAttrSet);
#else // SINGLETHREADED
            mergeAttributeSets(devAttrSet, otherAttrSet);
#endif // SINGLETHREADED

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&interfaceInfo.InterfaceClassGuid, sizeof(GUID), "InterfaceClassGuid", "A globally unique identifier for this particular interface type.", cdi::strings::guidToString(interfaceInfo.InterfaceClassGuid)));

            interfaces.push_back(devAttrSet);

        }

        delete[] buffer;
        buffer = NULL;
    }

    // Frees the interface HDEVINFO
    if (interfaceDevs != INVALID_HANDLE_VALUE)
    {
        SetupDiDestroyDeviceInfoList(interfaceDevs);
    }
    // Frees the allocated buffer
    if (buffer != NULL)
    {
        delete[] buffer;
        buffer = NULL;
    }

    return interfaces;
}

std::vector<cdi::attr::AttributeSet> getAllDevicesAttributeSet()
{
    HDEVINFO deviceDevs = getAllClassesHDevInfo();

    SP_DEVINFO_DATA devInfo;
    devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

    std::future<std::vector<cdi::attr::AttributeSet>> futureCompleteDevicesAttrSet;
    std::vector<cdi::attr::AttributeSet> completeDevicesAttrSet;

    DeviceIdToScsiPortMap deviceIdToScsiPortMap = getDeviceIdToScsiPortMap();

    if (deviceDevs == INVALID_HANDLE_VALUE)
    {
        goto exit;
    }

    {

#ifdef MULTITHREADED
        futureCompleteDevicesAttrSet = std::async(getInterfaceAttributeSet, GUID_NULL);
        std::vector<std::future<cdi::attr::AttributeSet>> devAttrSetsToAdd;
#else // SINGLETHREADED
        completeDevicesAttrSet = getInterfaceAttributeSet(GUID_NULL);
        std::vector<cdi::attr::AttributeSet> devAttrSetsToAdd;
#endif // SINGLETHREADED

        std::map<std::string, SP_DEVINFO_DATA> deviceIdToInfoData;

        for (int devIndex = 0; SetupDiEnumDeviceInfo(deviceDevs, devIndex, &devInfo); devIndex++)
        {
            std::string deviceId = getDeviceId(devInfo.DevInst);
            deviceIdToInfoData[deviceId] = devInfo;
        }

#ifdef MULTITHREADED
        completeDevicesAttrSet = futureCompleteDevicesAttrSet.get();
#endif // MULTITHEADED

        for (auto &dIAI : deviceIdToInfoData)
        {
            bool needToAdd = true;
            for (cdi::attr::AttributeSet &i : completeDevicesAttrSet)
            {
                if (i.find(ATTRWITHNAME("DeviceId"))->getValue<std::string>() == dIAI.first)
                {
                    needToAdd = false;
                    break;
                }
            }

            if (needToAdd)
            {
#ifdef MULTITHREADED
                devAttrSetsToAdd.push_back(std::async(getDeviceAttributeSet, deviceDevs, dIAI.second, ref(deviceIdToScsiPortMap)));
#else // SINGLETHREADED
                devAttrSetsToAdd.push_back(getDeviceAttributeSet(deviceDevs, dIAI.second, ref(deviceIdToScsiPortMap)));
#endif // SINGLETHREADED
            }
        }

#ifdef MULTITHREADED
        for (std::future<cdi::attr::AttributeSet> &futureAttrSet : devAttrSetsToAdd)
        {
            completeDevicesAttrSet.push_back(futureAttrSet.get());
        }
#else // SINGLETHREADED
        for (cdi::attr::AttributeSet &attrSet : devAttrSetsToAdd)
        {
            completeDevicesAttrSet.push_back(attrSet);
        }
#endif // SINGLETHREADED
    }

exit:
    // Frees the device HDEVINFO
    if (deviceDevs != INVALID_HANDLE_VALUE)
    {
        SetupDiDestroyDeviceInfoList(deviceDevs);
    }

    return completeDevicesAttrSet;
}

void printAttributeSet(cdi::attr::AttributeSet &attrMap)
{
    std::regex newlineRegex = std::regex("\n");
    for (auto i : attrMap)
    {
        // Hide 'private' attributes
        if (!cdi::strings::startsWith("__", i.getName(), true))
        {
            std::string value = i.getValue<std::string>();

            std::string spaces = "                           ";
            while (spaces.size() < i.getName().size() + 2)
            {
                spaces += " ";
            }

            value = std::regex_replace(value, newlineRegex, "\n" + spaces);

            printf("%-25s: %s\n", i.getName().c_str(), value.c_str());
        }
    }
    puts("*******************************************************");
}

DeviceIdToScsiPortMap getDeviceIdToScsiPortMap()
{
    DeviceIdToScsiPortMap deviceIdToScsiPort;
    std::map<int, std::string> scsiPortToDriver;
    std::map <std::string, std::vector <std::string>> driverToDeviceIdsMap;

    int currentPort = 0;
    std::string currentDriver;
    while (cdi::registry::getStringFromRegistry(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port " + std::to_string(currentPort), "Driver", currentDriver))
    {
        scsiPortToDriver[currentPort] = cdi::strings::rTrim(currentDriver);
        currentPort++;

        if (driverToDeviceIdsMap.find(currentDriver) == driverToDeviceIdsMap.end())
        {
            std::vector<std::string> deviceIds;
            std::string deviceId;
            int currentDeviceIdIndex = 0;
            std::string subKey = "SYSTEM\\CurrentControlSet\\Services\\" + std::string(currentDriver) + "\\Enum";
            while (cdi::registry::getStringFromRegistry(HKEY_LOCAL_MACHINE, subKey, std::to_string(currentDeviceIdIndex), deviceId))
            {
                deviceIds.push_back(deviceId);
                currentDeviceIdIndex++;
            }
            driverToDeviceIdsMap[currentDriver] = deviceIds;
        }
    }

    for (auto &i : scsiPortToDriver)
    {
        int port = i.first;
        std::string driver = i.second;
        auto &deviceIds = driverToDeviceIdsMap[driver];
        deviceIdToScsiPort[*deviceIds.rbegin()] = port;

        deviceIds.pop_back();
    }

    return deviceIdToScsiPort;
}

cdi::attr::AttributeSet &mergeAttributeSets(cdi::attr::AttributeSet &oldSet, cdi::attr::AttributeSet &newSet)
{
    cdi::attr::AttributeSet thingsToAddToOld;

    for (const cdi::attr::Attribute &oldSetAttr : oldSet)
    {
        auto newAttrItr = newSet.find(oldSetAttr);
        // It's rare but sometimes the newASet won't have something in the oldASet (or vice-versa)
        if (newAttrItr != newSet.end() && !cdi::strings::startsWith("__", newAttrItr->getName(), true))
        {
            // new set and old set have same item (name)
            if (oldSetAttr.getValue<std::string>() != newAttrItr->getValue<std::string>())
            {
                // delete current item in set to replace it with updated one
                auto n = cdi::attr::Attribute(oldSetAttr, "\n" + newAttrItr->getValue<std::string>());
                thingsToAddToOld.insert(n);
            }
        }
    }

    // Remove stale elements and add new from previous loop
    for (const cdi::attr::Attribute &attr : thingsToAddToOld)
    {
        replaceInAttributeSet(oldSet, attr);
    }

    // If something is just in the newASet, we need to bring it over to the oldASet
    for (auto &newSetAttr : newSet)
    {
        if (oldSet.find(newSetAttr) == oldSet.end())
        {
            oldSet.insert(newSetAttr);
        }
    }

    return oldSet;
}

cdi::attr::AttributeSet getAttributeSetFromDevicePath(std::string DevicePath, std::map<std::string, std::string> &msDosDeviceNameToDriveLetterMap)
{
    cdi::attr::AttributeSet devAttrSet;
    devAttrSet.insert(cdi::attr::Attribute("DevicePath", "A path used to send I/O Control commands to the interface.", DevicePath));

    char targetPaths[4096];
    // substr(4) to get rid of the \\.\ 
    DWORD targetPathsLength = QueryDosDevice(DevicePath.substr(4).c_str(), targetPaths, 4096);
    if (targetPathsLength != 0)
    {
        std::string MSDosDeviceName = std::string(targetPaths, targetPathsLength);
        MSDosDeviceName = cdi::strings::delimitedStringToNewlineString(MSDosDeviceName);
        devAttrSet.insert(cdi::attr::Attribute("MSDosDeviceName", "A named device object that is often created by a non-Windows Driver Model (WDM). Usually these device names are part of the \\DosDevices\\ directory. Well known MS-DOS device names are required to be used for specific well-known device types to maintain compatibility with user-mode applications.", MSDosDeviceName));

        // Map to drive letter... not quite sure what would happen if a volume spans multple MSDosDevices...
        if (msDosDeviceNameToDriveLetterMap.find(MSDosDeviceName) != msDosDeviceNameToDriveLetterMap.end())
        {
            std::string DriveLetter = msDosDeviceNameToDriveLetterMap[MSDosDeviceName];
            devAttrSet.insert(cdi::attr::Attribute("DriveLetter", "The drive letter corresponding with this device.", DriveLetter));
            std::string driveLetterWithBackslash = DriveLetter + "\\";
            memset(&targetPaths, '\0', sizeof(targetPaths));

            // Volume information
            if (GetVolumeNameForVolumeMountPoint(driveLetterWithBackslash.c_str(), targetPaths, sizeof(targetPaths)))
            {
                std::string VolumeName = targetPaths;
                devAttrSet.insert(cdi::attr::Attribute("VolumeName", "The volume GUID path of the form \\\\?\\Volume{GUID}\\ where GUID identifies the volume.", targetPaths));
            }
        }
    }

    addInterfaceConfigurationAndResources(devAttrSet);


    // See if we can find a PHYSICALDRIVE path
    HANDLE handle = CreateFile(DevicePath.c_str(),
        GENERIC_WRITE | GENERIC_READ,
        (FILE_SHARE_READ | FILE_SHARE_WRITE),
        NULL,
        OPEN_EXISTING,
        NULL,
        NULL);

    if (handle != INVALID_HANDLE_VALUE)
    {
        BYTE b[8096] = { 0 }; // reuse buffer
        BYTE * b2 = b + sizeof(b) / 2; // secondary buffer (same one just the second half)
        DWORD bytesReturned;
        PSTORAGE_DEVICE_NUMBER storageDeviceNumber = (PSTORAGE_DEVICE_NUMBER)b;
        if (DeviceIoControl(handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, storageDeviceNumber, sizeof(STORAGE_DEVICE_NUMBER), &bytesReturned, NULL) && bytesReturned != 0)
        {
            std::string PhysicalDrivePath = "\\\\.\\PHYSICALDRIVE" + std::to_string(storageDeviceNumber->DeviceNumber);
            devAttrSet.insert(cdi::attr::Attribute("PhysicalDrivePath", "A device path to the physical drive. Often equivalent to another device path that will have more device information including GUID.", PhysicalDrivePath));
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageDeviceNumber->DeviceNumber, sizeof(DWORD), "DeviceNumber", "The number corresponding with this particular physical drive. Can be placed on the end of \\\\.\\PHYSICALDRIVE to form a device path.", std::to_string(storageDeviceNumber->DeviceNumber)));

            // Check for valid partition
            if (storageDeviceNumber->PartitionNumber != (DWORD)-1)
            {
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageDeviceNumber->PartitionNumber, sizeof(DWORD), "PartitionNumber", "The number partition for this drive.", std::to_string(storageDeviceNumber->PartitionNumber)));
            }

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageDeviceNumber->DeviceType, sizeof(DWORD), "DeviceType", "The type of the underlying hardware for the drive.", std::to_string(storageDeviceNumber->DeviceType)));

            // substr(4) to get rid of the \\.\ 
            DWORD targetPathsLength = QueryDosDevice(PhysicalDrivePath.substr(4).c_str(), (char*)b, 4096);
            if (targetPathsLength != 0)
            {
                std::string MSDosPhysicalDriveDeviceName = std::string((char*)b, targetPathsLength);
                MSDosPhysicalDriveDeviceName = cdi::strings::delimitedStringToNewlineString(MSDosPhysicalDriveDeviceName);
                devAttrSet.insert(cdi::attr::Attribute("MSDosPhysicalDriveDeviceName", "MSDos device path as an alias to this device's \\\\.\\PHYSICALDRIVE# path", MSDosPhysicalDriveDeviceName));
            }
        }

        PSCSI_ADDRESS scsiAddress = (PSCSI_ADDRESS)b;
        if (DeviceIoControl(handle, IOCTL_SCSI_GET_ADDRESS, NULL, 0, scsiAddress, sizeof(SCSI_ADDRESS), &bytesReturned, NULL) && bytesReturned > 0)
        {
            if (scsiAddress->Length == sizeof(SCSI_ADDRESS))
            {
                std::string ScsiAdapterPath = "\\\\.\\SCSI" + std::to_string(scsiAddress->PortNumber) + ":";
                devAttrSet.insert(cdi::attr::Attribute("ScsiAdapterPath", "A device path to the device's SCSI Adapter.", ScsiAdapterPath));

                // substr(4) to get rid of the \\.\ 
                DWORD targetPathsLength = QueryDosDevice(ScsiAdapterPath.substr(4).c_str(), targetPaths, 4096);
                if (targetPathsLength != 0)
                {
                    std::string MSDosAdapterName = std::string(targetPaths, targetPathsLength);
                    MSDosAdapterName = cdi::strings::delimitedStringToNewlineString(MSDosAdapterName);
                    devAttrSet.insert(cdi::attr::Attribute("MSDosAdapterName", "MSDos device path as an alias to this device's \\\\.\\SCSI#: path", MSDosAdapterName));
                }

                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&scsiAddress->PathId, sizeof(UCHAR), "ScsiPathId", "SCSI Path Id. Often points to a specific device behind the host bus (SCSI) adapter.", std::to_string(scsiAddress->PathId)));

                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&scsiAddress->Lun, sizeof(UCHAR), "ScsiLun", "SCSI Logical Unit Number. Often points to a specific device handler behind a specific path id and target id behind the host bus (SCSI) adapter.", std::to_string(scsiAddress->Lun)));

                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&scsiAddress->TargetId, sizeof(UCHAR), "ScsiTargetId", "SCSI Target Id. Often points to a specific device handler behind a specific path id behind the host bus (SCSI) adapter.", std::to_string(scsiAddress->TargetId)));

                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&scsiAddress->PortNumber, sizeof(UCHAR), "ScsiPortNumber", "SCSI Port Number. Often points to a specific host bus (SCSI) adapter.", std::to_string(scsiAddress->PortNumber)));

                // Get IO_SCSI_CAPABILITIES... I think this may make more sense if sent to the adapter instead of device... :)
                PIO_SCSI_CAPABILITIES ioScsiCapabilities = (PIO_SCSI_CAPABILITIES)b;
                if (DeviceIoControl(handle, IOCTL_SCSI_GET_CAPABILITIES, NULL, 0, ioScsiCapabilities, sizeof(IO_SCSI_CAPABILITIES), &bytesReturned, NULL) && bytesReturned > 0)
                {
                    devAttrSet.insert(cdi::attr::Attribute((BYTE*)&ioScsiCapabilities->MaximumTransferLength, sizeof(ULONG), "MaximumTransferLength", "Maximum size, in bytes, of a single SCSI request block (SRB).", std::to_string(ioScsiCapabilities->MaximumTransferLength)));

                    devAttrSet.insert(cdi::attr::Attribute((BYTE*)&ioScsiCapabilities->MaximumPhysicalPages, sizeof(ULONG), "MaximumPhysicalPages", "Maximum number of physical pages per data buffer.", std::to_string(ioScsiCapabilities->MaximumPhysicalPages)));

                    devAttrSet.insert(cdi::attr::Attribute((BYTE*)&ioScsiCapabilities->SupportedAsynchronousEvents, sizeof(ULONG), "SupportedAsynchronousEvents", "Whether or not the host adapter supports SCSI asynchronous receive-event operations.", toBoolString(ioScsiCapabilities->SupportedAsynchronousEvents)));

                    devAttrSet.insert(cdi::attr::Attribute((BYTE*)&ioScsiCapabilities->AlignmentMask, sizeof(ULONG), "AlignmentMask", "The alignment mask for data transfers. Transfer data must be aligned on an address that is an integer multiple of this value.", std::to_string(ioScsiCapabilities->AlignmentMask)));

                    devAttrSet.insert(cdi::attr::Attribute((BYTE*)&ioScsiCapabilities->TaggedQueuing, sizeof(BOOLEAN), "TaggedQueuing", "Whether or not the device's host adapter supports tagged queuing.", toBoolString(ioScsiCapabilities->TaggedQueuing)));

                    devAttrSet.insert(cdi::attr::Attribute((BYTE*)&ioScsiCapabilities->AdapterScansDown, sizeof(BOOLEAN), "AdapterScansDown", "Whether or not the device's host adapter scans down for BIOS devices.", toBoolString(ioScsiCapabilities->AdapterScansDown)));

                    devAttrSet.insert(cdi::attr::Attribute((BYTE*)&ioScsiCapabilities->AdapterUsesPio, sizeof(BOOLEAN), "AdapterUsesPio", "Whether or not the device's host adapter uses programmed I/O.", toBoolString(ioScsiCapabilities->AdapterUsesPio)));
                }
            }
        }

        // Get hotplug info
        PSTORAGE_HOTPLUG_INFO storageHotplugInfo = (PSTORAGE_HOTPLUG_INFO)b;
        if (DeviceIoControl(handle, IOCTL_STORAGE_GET_HOTPLUG_INFO, NULL, 0, storageHotplugInfo, sizeof(STORAGE_HOTPLUG_INFO), &bytesReturned, NULL) && bytesReturned > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageHotplugInfo->MediaRemovable, sizeof(BOOLEAN), "MediaRemovable", "Specifies whether the device media is removable.", toBoolString(storageHotplugInfo->MediaRemovable)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageHotplugInfo->MediaHotplug, sizeof(BOOLEAN), "MediaHotplug", "Specifies whether the device media is lockable.", toBoolString(storageHotplugInfo->MediaHotplug)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageHotplugInfo->DeviceHotplug, sizeof(BOOLEAN), "DeviceHotplug", "Specifies whether the device is a hotplug device.", toBoolString(storageHotplugInfo->DeviceHotplug)));
        }

        // Get media serial number
        PMEDIA_SERIAL_NUMBER_DATA  mediaSerialNumberData = (PMEDIA_SERIAL_NUMBER_DATA)b;
        if (DeviceIoControl(handle, IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER, NULL, 0, mediaSerialNumberData, 512, &bytesReturned, NULL) && bytesReturned > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute("MediaSerialNumber", "USB media serial number for a USB device.", std::string((char*)mediaSerialNumberData->SerialNumberData, mediaSerialNumberData->SerialNumberLength)));
        }

        // Get media serial number
        PSTORAGE_READ_CAPACITY storageReadCapacity = (PSTORAGE_READ_CAPACITY)b;
        if (DeviceIoControl(handle, IOCTL_STORAGE_READ_CAPACITY, NULL, 0, storageReadCapacity, sizeof(STORAGE_READ_CAPACITY), &bytesReturned, NULL) && bytesReturned > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageReadCapacity->BlockLength, sizeof(ULONG), "BlockLength", "The number of bytes in a block from this device.", std::to_string(storageReadCapacity->BlockLength)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageReadCapacity->NumberOfBlocks.QuadPart, sizeof(LARGE_INTEGER), "NumberOfBlocks", "The number of blocks on the storage disk.", std::to_string(storageReadCapacity->NumberOfBlocks.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storageReadCapacity->DiskLength.QuadPart, sizeof(LARGE_INTEGER), "DiskLength", "The size of the storage disk in bytes.", std::to_string(storageReadCapacity->DiskLength.QuadPart)));
        }

        // Get storage unique identifier
        PSTORAGE_DEVICE_UNIQUE_IDENTIFIER pStorageDeviceUniqueIdentifer = (PSTORAGE_DEVICE_UNIQUE_IDENTIFIER)b;
        STORAGE_PROPERTY_QUERY storagePropertyQuery;
        memset(&storagePropertyQuery, 0, sizeof(STORAGE_PROPERTY_QUERY));
        storagePropertyQuery.PropertyId = StorageDeviceUniqueIdProperty;
        storagePropertyQuery.QueryType = PropertyStandardQuery;
        if (DeviceIoControl(handle, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY), &b, 4096, &bytesReturned, NULL) && bytesReturned > 0)
        {
            PSTORAGE_DEVICE_ID_DESCRIPTOR pStorageDeviceIdDescriptor = (PSTORAGE_DEVICE_ID_DESCRIPTOR)(b + pStorageDeviceUniqueIdentifer->StorageDeviceIdOffset);

            PSTORAGE_DEVICE_DESCRIPTOR pStorageDeviceDescriptor = (PSTORAGE_DEVICE_DESCRIPTOR)(b + pStorageDeviceUniqueIdentifer->StorageDeviceOffset);

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&pStorageDeviceDescriptor->CommandQueueing, sizeof(BOOLEAN), "CommandQueueing", "Indicates if the device supports outstanding commands via SCSI tagged queuing or another mechanism. The Storport driver is responsible for synchronizing the commands.", toBoolString(pStorageDeviceDescriptor->CommandQueueing)));

            if (pStorageDeviceDescriptor->VendorIdOffset != 0)
            {
                devAttrSet.insert(cdi::attr::Attribute("VendorId", "Unique identifier for the device vendor.", cdi::strings::rTrim(std::string((char*)pStorageDeviceDescriptor + pStorageDeviceDescriptor->VendorIdOffset))));
            }

            if (pStorageDeviceDescriptor->ProductIdOffset != 0)
            {
                devAttrSet.insert(cdi::attr::Attribute("ProductId", "Unique identifier for this product from the given vendor. Sometimes refered to as a \"model string\".", cdi::strings::rTrim(std::string((char*)pStorageDeviceDescriptor + pStorageDeviceDescriptor->ProductIdOffset))));
            }

            if (pStorageDeviceDescriptor->ProductRevisionOffset != 0)
            {
                devAttrSet.insert(cdi::attr::Attribute("ProductRevision", "Unique identifier for the revision of host software on the device. Sometimes refered to as a \"firmware version\".", cdi::strings::rTrim(std::string((char*)pStorageDeviceDescriptor + pStorageDeviceDescriptor->ProductRevisionOffset))));
            }

            if (pStorageDeviceDescriptor->SerialNumberOffset != 0)
            {
                devAttrSet.insert(cdi::attr::Attribute("SerialNumber", "(Theoretically) Unique identifier for this particular build of this device with this firmware.", cdi::strings::rTrim(std::string((char*)pStorageDeviceDescriptor + pStorageDeviceDescriptor->SerialNumberOffset))));
            }

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&pStorageDeviceDescriptor->BusType, sizeof(STORAGE_BUS_TYPE), "BusType", "Specifies the type of bus to which the device is connected.", cdi::strings::storageBusTypeToString(pStorageDeviceDescriptor->BusType)));

            PSTORAGE_DEVICE_LAYOUT_SIGNATURE pStorageDeviceLayoutSignature = (PSTORAGE_DEVICE_LAYOUT_SIGNATURE)(b + pStorageDeviceUniqueIdentifer->DriveLayoutSignatureOffset);
            // If MBR give that info, otherwise give GPT GUID
            if (pStorageDeviceLayoutSignature->Mbr)
            {
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&pStorageDeviceLayoutSignature->DeviceSpecific.MbrSignature, sizeof(ULONG), "MbrSignature", "Signature for the drive's master boot record (MBR).", std::to_string(pStorageDeviceLayoutSignature->DeviceSpecific.MbrSignature)));
            }
            else
            {
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&pStorageDeviceLayoutSignature->DeviceSpecific.GptDiskId, sizeof(GUID), "GptDiskId", "Signature for the drive's GUID partition table (GPT).", cdi::strings::guidToString(pStorageDeviceLayoutSignature->DeviceSpecific.GptDiskId)));
            }

            // Everything together... useful because a user can use CompareStorageDuids(...) to check for exact/sub matches
            devAttrSet.insert(cdi::attr::Attribute(b, bytesReturned, "StorageDUID", "Storage device unique identifer. Can be used via WinApi to find this device uniquely via exact and sub matches.", cdi::strings::byteArrayToString(b, bytesReturned)));

            // Bus specific data
            if (pStorageDeviceDescriptor->RawPropertiesLength > 0)
            {
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&pStorageDeviceDescriptor->RawDeviceProperties, pStorageDeviceDescriptor->RawPropertiesLength, "BusSpecificRawProperties", "Bus specific property data.", cdi::strings::byteArrayToString((BYTE*)&pStorageDeviceDescriptor->RawDeviceProperties, pStorageDeviceDescriptor->RawPropertiesLength)));
            }
        }

        // Another way to get manufacturer
        if (DeviceIoControl(handle, IOCTL_HID_GET_MANUFACTURER_STRING, NULL, NULL, b, 128, &bytesReturned, NULL) && bytesReturned > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute("Manufacturer", "The manufacturer of the device.", cdi::strings::wStringToString((wchar_t*)b)));
        }

        // FW / model string
        if (DeviceIoControl(handle, IOCTL_HID_GET_PRODUCT_STRING, NULL, NULL, b, 128, &bytesReturned, NULL) && bytesReturned > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute("ProductId", "Unique identifier for this product from the given vendor. Sometimes refered to as a \"model string\".", cdi::strings::wStringToString((wchar_t*)b)));
        }

        // Serial number
        if (DeviceIoControl(handle, IOCTL_HID_GET_SERIALNUMBER_STRING, NULL, NULL, b, 128, &bytesReturned, NULL) && bytesReturned > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute("SerialNumber", "(Theoretically) Unique identifier for this particular build of this device with this firmware.", cdi::strings::wStringToString((wchar_t*)b)));
        }

        // SMART / failure prediction information
        PSTORAGE_PREDICT_FAILURE storagePredictFailure = PSTORAGE_PREDICT_FAILURE(b);
        if (DeviceIoControl(handle, IOCTL_STORAGE_PREDICT_FAILURE, NULL, NULL, storagePredictFailure, sizeof(STORAGE_PREDICT_FAILURE), &bytesReturned, NULL) && bytesReturned > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storagePredictFailure->PredictFailure, sizeof(BOOLEAN), "PredictFailure", "Using Self-Monitoring and Reporting Technology (SMART) data, a prediction as to if device failure is imminent.", toBoolString(storagePredictFailure->PredictFailure)));

            // Get SMART Thresholds
            PSENDCMDINPARAMS smartSendCmdParams = (PSENDCMDINPARAMS)b2;
            smartSendCmdParams->cBufferSize = sizeof(b) / 2; // using secondary buffer
            IDEREGS ideRegs = { 0 };
            ideRegs.bFeaturesReg = READ_THRESHOLDS;
            ideRegs.bCylLowReg = SMART_CYL_LOW;
            ideRegs.bCylHighReg = SMART_CYL_HI;
            ideRegs.bCommandReg = SMART_CMD;
            smartSendCmdParams->irDriveRegs = ideRegs;
            BYTE* smartThresholds = NULL;

            if (DeviceIoControl(handle, SMART_RCV_DRIVE_DATA, b2 , smartSendCmdParams->cBufferSize, b2, smartSendCmdParams->cBufferSize, &bytesReturned, NULL) && bytesReturned > 512)
            {
                // This math comes from the SMART Thresholds being in the last 512 bytes. Don't care about the rest.
                smartThresholds = b2 + (bytesReturned - READ_THRESHOLD_BUFFER_SIZE);
            }

            std::string SMARTData = cdi::strings::smartToString((BYTE*)storagePredictFailure->VendorSpecific, READ_THRESHOLD_BUFFER_SIZE, smartThresholds);
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&storagePredictFailure->VendorSpecific, READ_THRESHOLD_BUFFER_SIZE, "SMARTData", "Self-Monitoring and Reporting Technology (SMART) data. Used to diagnose the state and potential for failure of a device.", SMARTData));

            // SMART Return Status (should say if a threshold exceeded condition)
            smartSendCmdParams->cBufferSize = sizeof(b);
            ideRegs.bFeaturesReg = RETURN_SMART_STATUS;
            ideRegs.bCylLowReg = SMART_CYL_LOW;
            ideRegs.bCylHighReg = SMART_CYL_HI;
            ideRegs.bCommandReg = SMART_CMD;
            smartSendCmdParams->irDriveRegs = ideRegs;
            if (DeviceIoControl(handle, SMART_SEND_DRIVE_COMMAND, b2, smartSendCmdParams->cBufferSize, b2, smartSendCmdParams->cBufferSize, &bytesReturned, NULL) && bytesReturned > 0)
            {
                std::string SMARTReturnStatus = "Healthy";
                if (ideRegs.bCylLowReg == BAD_SMART_LOW && ideRegs.bCylHighReg == BAD_SMART_HIGH)
                {
                    SMARTReturnStatus = "Threshold Exceeded Condition";
                }
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&ideRegs, sizeof(IDEREGS), "SMARTReturnStatus", "Status determined via SMART metrics.", SMARTReturnStatus));
            }
        }

        // Another mechanism for getter disk length
        PDISK_GEOMETRY_EX diskGeoEx = (PDISK_GEOMETRY_EX)b;
        if (DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, NULL, diskGeoEx, sizeof(DISK_GEOMETRY_EX), &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string DiskLength = std::to_string(diskGeoEx->DiskSize.QuadPart) + " Bytes" + " (" + std::to_string(diskGeoEx->DiskSize.QuadPart / (double)BYTES_IN_GIGABYTE) + " Gigabytes)";
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&diskGeoEx->DiskSize.QuadPart, sizeof(ULONGLONG), "DiskLength", "The size of the disk in bytes.", DiskLength));
        }

        // Gets the capabilities of the USB hub
        PUSB_HUB_CAPABILITIES_EX usbHubCapabilities = (PUSB_HUB_CAPABILITIES_EX)b;
        if (DeviceIoControl(handle, IOCTL_USB_GET_HUB_CAPABILITIES_EX, NULL, NULL, usbHubCapabilities, sizeof(USB_HUB_CAPABILITIES_EX), &bytesReturned, NULL) && bytesReturned > 0)
        {

            ULONG HubIsHighSpeedCapable = usbHubCapabilities->CapabilityFlags.HubIsHighSpeedCapable & 1;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&HubIsHighSpeedCapable, sizeof(ULONG), "HubIsHighSpeedCapable", "Designates if the hub is capable of high speed USB (2.0 / 480 Mbits).", toBoolString(usbHubCapabilities->CapabilityFlags.HubIsHighSpeedCapable)));
            
            ULONG HubIsHighSpeed = usbHubCapabilities->CapabilityFlags.HubIsHighSpeed & 1;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&HubIsHighSpeed, sizeof(ULONG), "HubIsHighSpeed", "Designates if the hub is operating at high speed (~480 Mbits).", toBoolString(usbHubCapabilities->CapabilityFlags.HubIsHighSpeed)));

            ULONG HubIsMultiTransactionCapable = usbHubCapabilities->CapabilityFlags.HubIsMultiTtCapable & 1;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&HubIsMultiTransactionCapable, sizeof(ULONG), "HubIsMultiTransactionCapable", "Designates if the hub is capable of doing multiple transactions/translations simultaneously.", toBoolString(usbHubCapabilities->CapabilityFlags.HubIsMultiTtCapable)));

            ULONG HubIsMultiTransaction = usbHubCapabilities->CapabilityFlags.HubIsMultiTt & 1;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&HubIsMultiTransaction, sizeof(ULONG), "HubIsMultiTransaction", "Designates if the hub is configured to do multiple transactions/translations simultaneously.", toBoolString(usbHubCapabilities->CapabilityFlags.HubIsMultiTt)));

            ULONG HubIsRoot = usbHubCapabilities->CapabilityFlags.HubIsRoot & 1;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&HubIsRoot, sizeof(ULONG), "HubIsRoot", "Designates if the hub is the root hub.", toBoolString(usbHubCapabilities->CapabilityFlags.HubIsRoot)));

            ULONG HubIsArmedWakeOnConnect = usbHubCapabilities->CapabilityFlags.HubIsArmedWakeOnConnect & 1;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&HubIsArmedWakeOnConnect, sizeof(ULONG), "HubIsArmedWakeOnConnect", "Designates if the hub is armed to wake when a device is connected to the hub.", toBoolString(usbHubCapabilities->CapabilityFlags.HubIsArmedWakeOnConnect)));

            ULONG HubIsBusPowered = usbHubCapabilities->CapabilityFlags.HubIsBusPowered & 1;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&HubIsBusPowered, sizeof(ULONG), "HubIsBusPowered", "Designates if the hub is bus-powered or self-powered.", toBoolString(usbHubCapabilities->CapabilityFlags.HubIsBusPowered)));
        }


        PUSB_NODE_INFORMATION usbNodeInfo = (PUSB_NODE_INFORMATION)b;
        memset(&b, 0, sizeof(USB_NODE_INFORMATION));
        // MSDN says to pass the USB_NODE_INFORMATION as input as well and set NodeType... I don't think it does anything on input... Also requesting more than 6 because it seems to pass with invalid data at 6.
        if (DeviceIoControl(handle, IOCTL_USB_GET_NODE_INFORMATION, usbNodeInfo, sizeof(USB_NODE_INFORMATION), usbNodeInfo, sizeof(USB_NODE_INFORMATION), &bytesReturned, NULL) && bytesReturned > 6)
        {
            if (usbNodeInfo->NodeType == UsbMIParent)
            {
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&usbNodeInfo->NodeType, sizeof(USB_HUB_NODE), "ParentUSBNodeType", "The type of the parent USB device.", "UsbMiParent"));

                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&usbNodeInfo->u.MiParentInformation.NumberOfInterfaces, sizeof(ULONG), "ParentNumberOfInterfaces", "The number of interfaces on the composite device.", std::to_string(usbNodeInfo->u.MiParentInformation.NumberOfInterfaces)));
            }
            else if (usbNodeInfo->NodeType == UsbHub)
            {
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&usbNodeInfo->NodeType, sizeof(USB_HUB_NODE), "ParentUSBNodeType", "The type of the parent USB device.", "UsbHub"));

                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&usbNodeInfo->u.HubInformation.HubDescriptor.bNumberOfPorts, sizeof(UCHAR), "ParentNumberOfHubPorts", "The number of ports on the hub.", std::to_string(usbNodeInfo->u.HubInformation.HubDescriptor.bNumberOfPorts)));

                // Apparently there is more info on this in the USB spec... possible todo: decode it.
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&usbNodeInfo->u.HubInformation.HubDescriptor.wHubCharacteristics, sizeof(USHORT), "ParentHubCharacteristics", "The USB specification characteristics of the hub.", std::to_string(usbNodeInfo->u.HubInformation.HubDescriptor.wHubCharacteristics)));

                // Number is in 2-ms intervals
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&usbNodeInfo->u.HubInformation.HubDescriptor.bPowerOnToPowerGood, sizeof(UCHAR), "ParentHubTimeToPowerOn", "Time in 2 millisecond intervals that it takes for the device to power on completely.", std::to_string(usbNodeInfo->u.HubInformation.HubDescriptor.bPowerOnToPowerGood * 2) + " Milliseconds"));

                // Number is in milliamperes
                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&usbNodeInfo->u.HubInformation.HubDescriptor.bHubControlCurrent, sizeof(UCHAR), "ParentHubMaxCurrent", "Maximum power requirements for the hub's controller.", std::to_string(usbNodeInfo->u.HubInformation.HubDescriptor.bHubControlCurrent) + " Milliamperes"));

                devAttrSet.insert(cdi::attr::Attribute((BYTE*)&usbNodeInfo->u.HubInformation.HubIsBusPowered, sizeof(BOOLEAN), "ParentIsBusPowered", "Denotes if the parent is powered by the hub/bus or self-powered.", toBoolString(usbNodeInfo->u.HubInformation.HubIsBusPowered)));
            }
        }

        PUSB_ROOT_HUB_NAME usbRootHubName = (PUSB_ROOT_HUB_NAME)b;
        if (DeviceIoControl(handle, IOCTL_USB_GET_ROOT_HUB_NAME, NULL, NULL, usbRootHubName, 1024, &bytesReturned, NULL) && bytesReturned > 0 && usbRootHubName->ActualLength > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute("RootHubName", "Symbolic name for the root hub.", cdi::strings::wStringToString(std::wstring((wchar_t*)usbRootHubName->RootHubName))));
        }

        PVOLUME_DISK_EXTENTS volumeDiskExtents = (PVOLUME_DISK_EXTENTS)b;
        if (DeviceIoControl(handle, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, NULL, &b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&volumeDiskExtents->NumberOfDiskExtents, sizeof(DWORD), "NumberOfDiskExtents", "The number of disks in the given volume.", std::to_string(volumeDiskExtents->NumberOfDiskExtents)));

            DISK_EXTENT * thisExtent = volumeDiskExtents->Extents;
            for (size_t i = 0; i < volumeDiskExtents->NumberOfDiskExtents; i++)
            {
                std::string DiskExtent = "Disk Number: " + std::to_string(thisExtent->DiskNumber) + "\n";
                DiskExtent += "Starting Offset: Byte " + std::to_string(thisExtent->StartingOffset.QuadPart) + "\n";
                DiskExtent += "Extent Length: " + std::to_string(thisExtent->ExtentLength.QuadPart) + " Bytes";

                devAttrSet.insert(cdi::attr::Attribute((BYTE*)thisExtent, sizeof(DISK_EXTENT), "DiskExtent" + std::to_string(i), "Information about a given disk and the extent of the current volume.", DiskExtent));

                // Should do pointer math and move to the next extent
                thisExtent++;
            }
        }

        // Get file system information.
        if (DeviceIoControl(handle, FSCTL_GET_NTFS_VOLUME_DATA, NULL, NULL, &b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            PNTFS_VOLUME_DATA_BUFFER p = (PNTFS_VOLUME_DATA_BUFFER)b;

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->VolumeSerialNumber.QuadPart, sizeof(LARGE_INTEGER), "VolumeSerialNumber", "The serial number for the volume, assigned by the operating system when the disk is formatted.", std::to_string(p->VolumeSerialNumber.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->NumberSectors.QuadPart, sizeof(LARGE_INTEGER), "NumberSectors", "The number of sectors in the specified volume.", std::to_string(p->NumberSectors.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->TotalClusters.QuadPart, sizeof(LARGE_INTEGER), "TotalClusters", "The total number of clusters in the given volume.", std::to_string(p->TotalClusters.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->FreeClusters.QuadPart, sizeof(LARGE_INTEGER), "FreeClusters", "The number of free clusters in the given volume.", std::to_string(p->FreeClusters.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->TotalReserved.QuadPart, sizeof(LARGE_INTEGER), "TotalReserved", "The number of reserved clusters in the given volume.", std::to_string(p->TotalReserved.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->BytesPerSector, sizeof(DWORD), "BytesPerSector", "The number of bytes in a sector on the specified volume.", std::to_string(p->BytesPerSector)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->BytesPerCluster, sizeof(DWORD), "BytesPerCluster", "The number of bytes in a cluster on the specified volume. This is also known as the cluster factor.", std::to_string(p->BytesPerCluster)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->BytesPerFileRecordSegment, sizeof(DWORD), "BytesPerFileRecordSegment", "The number of bytes in a file record segment.", std::to_string(p->BytesPerFileRecordSegment)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->ClustersPerFileRecordSegment, sizeof(DWORD), "ClustersPerFileRecordSegment", "The number of clusters in a file record segment.", std::to_string(p->ClustersPerFileRecordSegment)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->MftValidDataLength.QuadPart, sizeof(LARGE_INTEGER), "MftValidDataLength", "The length of the master file table, in bytes.", std::to_string(p->MftValidDataLength.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->MftStartLcn.QuadPart, sizeof(LARGE_INTEGER), "MftStartLcn", "The starting logical cluster number of the master file table.", std::to_string(p->MftStartLcn.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->Mft2StartLcn.QuadPart, sizeof(LARGE_INTEGER), "Mft2StartLcn", "The starting logical cluster number of the master file table mirror.", std::to_string(p->Mft2StartLcn.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->MftZoneStart.QuadPart, sizeof(LARGE_INTEGER), "MftZoneStart", "The starting logical cluster number of the master file table zone.", std::to_string(p->MftZoneStart.QuadPart)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&p->MftZoneEnd.QuadPart, sizeof(LARGE_INTEGER), "MftZoneEnd", "The ending logical cluster number of the master file table zone.", std::to_string(p->MftZoneEnd.QuadPart)));
        }

        if (DeviceIoControl(handle, IOCTL_VOLUME_GET_GPT_ATTRIBUTES, NULL, NULL, &b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            PULONGLONG p = (PULONGLONG)b;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)b, sizeof(VOLUME_GET_GPT_ATTRIBUTES_INFORMATION), "GPTAttributes", "Attributes of the GUID Partition Table (GPT).", cdi::strings::gptAttributesToString(*p)));
        }

        if (DeviceIoControl(handle, IOCTL_SERIAL_GET_COMMSTATUS, NULL, 0, b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            PULONG baud = (PULONG)b;
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)b, sizeof(ULONG), "BaudRate", "Various flags associated with the volume's file system.", std::to_string(*baud)));
        }

        DWORD volumeSerialNumber = 0;
        DWORD maximumComponentLength = 0;
        DWORD fileSystemFlags = 0;
        DWORD fileSystemNameBufferSize = (sizeof(b) - 2048) / sizeof(wchar_t);
        wchar_t* fileSystemNameBuffer = (wchar_t*)(b + 2048); //use the same buffer, just shift down rather far to avoid overlap
        if (GetVolumeInformationByHandleW(handle, (LPWSTR)b, sizeof(b), &volumeSerialNumber, &maximumComponentLength, &fileSystemFlags, fileSystemNameBuffer, fileSystemNameBufferSize))
        {
            devAttrSet.insert(cdi::attr::Attribute("VolumeInformation", "The name of the specified volume.", cdi::strings::wStringToString((wchar_t*)b)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&volumeSerialNumber, sizeof(DWORD), "VolumeSerialNumber", "The serial number for the volume, assigned by the operating system when the disk is formatted.", std::to_string(volumeSerialNumber)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&maximumComponentLength, sizeof(DWORD), "MaximumComponentLength", "The maximum length in wide characters the file name component of the volume's file system supports. This is the portion of a file name between backslashes.", std::to_string(maximumComponentLength)));

            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&fileSystemFlags, sizeof(DWORD), "FileSystemFlags", "Various flags associated with the volume's file system.", cdi::strings::fileSystemFlagToString(fileSystemFlags)));

            std::wstring FileSystemNameBuffer(fileSystemNameBuffer);
            devAttrSet.insert(cdi::attr::Attribute((BYTE*)&fileSystemNameBuffer, FileSystemNameBuffer.size(), "FileSystemName", "The name of the type of file system.", cdi::strings::wStringToString(FileSystemNameBuffer)));
        }

        CloseHandle(handle);
    }

    return devAttrSet;
}

std::map<std::string, std::string> getMsDosDeviceNameToDriveLetterMap()
{
    std::map<std::string, std::string> retMap;

    // go through all drive letters
    for (char i = 'A'; i <= 'Z'; i++)
    {
        std::string letterStr = std::string(1, i) + ":";
        char msDosPath[MAX_PATH] = { '\0' };
        if (QueryDosDevice(letterStr.c_str(), msDosPath, MAX_PATH))
        {
            retMap[msDosPath] = letterStr;
        }
    }

    return retMap;
}