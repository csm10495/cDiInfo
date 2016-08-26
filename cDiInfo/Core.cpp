/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Core.cpp - Core SetupDi-interfacing implementation file
*/

// Local includes
#include "Core.h"

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
std::vector<GUID> ALL_GUIDS = {
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

std::string getDevInfoProperty(HDEVINFO &devs, PSP_DEVINFO_DATA devInfo, DWORD property, TYPE retType)
{
    char buffer[8192] = { 0 };
    DWORD reqSize;
    DWORD dataType;

    if (SetupDiGetDeviceRegistryProperty(devs, devInfo, property, &dataType, (LPBYTE)buffer, 8192, &reqSize))
    {
        // Special handling since this is a structure...
        if (property == SPDRP_DEVICE_POWER_DATA)
        {
            return propertyBufferToString((BYTE*)buffer, 8192, DEVPROP_TYPE_BINARY, DEVPKEY_Device_PowerData);
        }

        std::string retStr = std::string(buffer, reqSize);
        if (retType == _INT_)
        {
            return std::to_string(*(DWORD*)retStr.c_str());
        }
        else if (retType == __STRING_)
        {
            return delimitedStringToNewlineString(retStr);
        }
        else if (retType == _GUID_)
        {
            GUID g = *((GUID*)retStr.c_str());
            return guidToString(g);
        }
        else if (retType == __WSTRING_)
        {
            std::wstring tmpWStr((wchar_t*)retStr.c_str());
            return wStringToString(tmpWStr);
        }
    }
    return std::string(UNAVAILABLE_ATTRIBUTE);
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
            return rTrim(deviceId);
        }
    }
    return UNAVAILABLE_ATTRIBUTE;
}

AttributeMap getDeviceAttributeMap(HDEVINFO devs, SP_DEVINFO_DATA devInfo, DeviceIdToScsiPortMap &deviceIdToScsiPortMap)
{
    AttributeMap devAttrMap;

    std::string DeviceDesc = getDevInfoProperty(devs, &devInfo, SPDRP_DEVICEDESC, __STRING_);
    addToMap(devAttrMap, DeviceDesc);
    std::string HardwareId = getDevInfoProperty(devs, &devInfo, SPDRP_HARDWAREID, __STRING_);
    addToMap(devAttrMap, HardwareId);
    std::string FriendlyName = getDevInfoProperty(devs, &devInfo, SPDRP_FRIENDLYNAME, __STRING_);
    addToMap(devAttrMap, FriendlyName);
    std::string CompatibleIds = getDevInfoProperty(devs, &devInfo, SPDRP_COMPATIBLEIDS, __STRING_);
    addToMap(devAttrMap, CompatibleIds);
    std::string Unused0 = getDevInfoProperty(devs, &devInfo, SPDRP_UNUSED0, __STRING_);
    addToMap(devAttrMap, Unused0);
    std::string Service = getDevInfoProperty(devs, &devInfo, SPDRP_SERVICE, __STRING_);
    addToMap(devAttrMap, Service);
    std::string Unused1 = getDevInfoProperty(devs, &devInfo, SPDRP_UNUSED1, __STRING_);
    addToMap(devAttrMap, Unused1);
    std::string Unused2 = getDevInfoProperty(devs, &devInfo, SPDRP_UNUSED2, __STRING_);
    addToMap(devAttrMap, Unused2);
    std::string Class = getDevInfoProperty(devs, &devInfo, SPDRP_CLASS, __STRING_);
    addToMap(devAttrMap, Class);
    std::string ClassGuid = getDevInfoProperty(devs, &devInfo, SPDRP_CLASSGUID, __STRING_);
    addToMap(devAttrMap, ClassGuid);
    std::string Driver = getDevInfoProperty(devs, &devInfo, SPDRP_DRIVER, __STRING_);
    addToMap(devAttrMap, Driver);
    std::string ConfigFlags = getDevInfoProperty(devs, &devInfo, SPDRP_CONFIGFLAGS, _INT_);
    addToMap(devAttrMap, ConfigFlags);
    std::string Manufacturer = getDevInfoProperty(devs, &devInfo, SPDRP_MFG, __STRING_);
    addToMap(devAttrMap, Manufacturer);
    std::string LocationInformation = getDevInfoProperty(devs, &devInfo, SPDRP_LOCATION_INFORMATION, __STRING_);
    addToMap(devAttrMap, LocationInformation);
    std::string PhysicalDeviceObjectName = getDevInfoProperty(devs, &devInfo, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, __STRING_);
    addToMap(devAttrMap, PhysicalDeviceObjectName);
    std::string Capabilities = getDevInfoProperty(devs, &devInfo, SPDRP_CAPABILITIES, _INT_);
    addToMap(devAttrMap, Capabilities);
    std::string UiNumber = getDevInfoProperty(devs, &devInfo, SPDRP_UI_NUMBER, _INT_);
    addToMap(devAttrMap, UiNumber);
    std::string UpperFilters = getDevInfoProperty(devs, &devInfo, SPDRP_UPPERFILTERS, __STRING_);
    addToMap(devAttrMap, UpperFilters);
    std::string LowerFilters = getDevInfoProperty(devs, &devInfo, SPDRP_LOWERFILTERS, __STRING_);
    addToMap(devAttrMap, LowerFilters);
    std::string BusTypeGuid = getDevInfoProperty(devs, &devInfo, SPDRP_BUSTYPEGUID, _GUID_);
    addToMap(devAttrMap, BusTypeGuid);
    std::string LegacyBusType = getDevInfoProperty(devs, &devInfo, SPDRP_LEGACYBUSTYPE, _INT_);
    addToMap(devAttrMap, LegacyBusType);
    std::string BusNumber = getDevInfoProperty(devs, &devInfo, SPDRP_BUSNUMBER, _INT_);
    addToMap(devAttrMap, BusNumber);
    std::string EnumeratorName = getDevInfoProperty(devs, &devInfo, SPDRP_ENUMERATOR_NAME, __STRING_);
    addToMap(devAttrMap, EnumeratorName);
    std::string Security = getDevInfoProperty(devs, &devInfo, SPDRP_SECURITY, _INT_);
    addToMap(devAttrMap, Security);
    std::string SecuritySds = getDevInfoProperty(devs, &devInfo, SPDRP_SECURITY_SDS, _INT_);
    addToMap(devAttrMap, SecuritySds);
    std::string Devtype = getDevInfoProperty(devs, &devInfo, SPDRP_DEVTYPE, _INT_);
    addToMap(devAttrMap, Devtype);
    std::string Exclusive = getDevInfoProperty(devs, &devInfo, SPDRP_EXCLUSIVE, _INT_);
    addToMap(devAttrMap, Exclusive);
    std::string Characteristics = getDevInfoProperty(devs, &devInfo, SPDRP_CHARACTERISTICS, _INT_);
    addToMap(devAttrMap, Characteristics);
    std::string Address = getDevInfoProperty(devs, &devInfo, SPDRP_ADDRESS, _INT_);
    addToMap(devAttrMap, Address);
    std::string UiNumberDescFormat = getDevInfoProperty(devs, &devInfo, SPDRP_UI_NUMBER_DESC_FORMAT, __STRING_);
    addToMap(devAttrMap, UiNumberDescFormat);
    std::string PowerData = getDevInfoProperty(devs, &devInfo, SPDRP_DEVICE_POWER_DATA, _INT_);
    addToMap(devAttrMap, PowerData);
    std::string RemovalPolicy = getDevInfoProperty(devs, &devInfo, SPDRP_REMOVAL_POLICY, _INT_);
    addToMap(devAttrMap, RemovalPolicy);
    std::string RemovalPolicyHwDefault = getDevInfoProperty(devs, &devInfo, SPDRP_REMOVAL_POLICY_HW_DEFAULT, _INT_);
    addToMap(devAttrMap, RemovalPolicyHwDefault);
    std::string RemovalPolicyOverride = getDevInfoProperty(devs, &devInfo, SPDRP_REMOVAL_POLICY_OVERRIDE, _INT_);
    addToMap(devAttrMap, RemovalPolicyOverride);
    std::string InstallState = getDevInfoProperty(devs, &devInfo, SPDRP_INSTALL_STATE, _INT_);
    addToMap(devAttrMap, InstallState);
    std::string LocationPaths = getDevInfoProperty(devs, &devInfo, SPDRP_LOCATION_PATHS, __STRING_);
    addToMap(devAttrMap, LocationPaths);
    std::string BaseContainerId = getDevInfoProperty(devs, &devInfo, SPDRP_BASE_CONTAINERID, __WSTRING_);
    addToMap(devAttrMap, BaseContainerId);

    std::string DeviceId = getDeviceId(devInfo.DevInst);
    addToMap(devAttrMap, DeviceId);

    std::string ChildrenDeviceIds = NO_CHILDREN;
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
    rTrim(ChildrenDeviceIds);
    addToMap(devAttrMap, ChildrenDeviceIds);

    std::string ParentDeviceId = NO_PARENT;
    DEVINST parentDevInst = { 0 };
    if (CM_Get_Parent(&parentDevInst, devInfo.DevInst, 0) == CR_SUCCESS)
    {
        ParentDeviceId = getDeviceId(parentDevInst);
    }
    addToMap(devAttrMap, ParentDeviceId);

    ULONG pulStatus = 0;
    ULONG pulProblemNumber = 0;
    std::string NodeStatus = UNAVAILABLE_ATTRIBUTE;
    std::string NodeProblem = UNAVAILABLE_ATTRIBUTE;
    auto ret = CM_Get_DevNode_Status(&pulStatus, &pulProblemNumber, devInfo.DevInst, 0);
    if (ret == CR_SUCCESS)
    {
        NodeStatus = nodeStatusToString(pulStatus);
        if (pulStatus & DN_HAS_PROBLEM)
        {
            NodeProblem = cmProbToString(pulProblemNumber);
        }
        else
        {
            NodeProblem = NO_PROBLEM;
        }
    }
    addToMap(devAttrMap, NodeStatus);
    addToMap(devAttrMap, NodeProblem);

    ULONG pulDepth = 0;
    std::string NodeDepth = UNAVAILABLE_ATTRIBUTE;
    if (CM_Get_Depth(&pulDepth, devInfo.DevInst, 0) == CR_SUCCESS)
    {
        NodeDepth = std::to_string(pulDepth);
    }
    addToMap(devAttrMap, NodeDepth);

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
    rTrim(SiblingDeviceIds);
    addToMap(devAttrMap, SiblingDeviceIds);

    SP_DRVINFO_DATA driverInfoData = { 0 };
    driverInfoData.cbSize = sizeof(driverInfoData);

    if (getDriverInfoData(devs, devInfo, &driverInfoData))
    {
        std::string DriverDescription = driverInfoData.Description;
        addToMap(devAttrMap, DriverDescription);

        SYSTEMTIME st = { 0 };
        FileTimeToSystemTime(&driverInfoData.DriverDate, &st);
        std::string DriverDate = std::to_string(st.wMonth) + "/" + std::to_string(st.wDay) + "/" + std::to_string(st.wYear);
        addToMap(devAttrMap, DriverDate);

        std::string DriverVersion = std::to_string((driverInfoData.DriverVersion >> 48) & 0xFFFF) + "." + \
            std::to_string((driverInfoData.DriverVersion >> 32) & 0xFFFF) + "." + \
            std::to_string((driverInfoData.DriverVersion >> 16) & 0xFFFF) + "." + \
            std::to_string((driverInfoData.DriverVersion) & 0xFFFF);
        addToMap(devAttrMap, DriverVersion);

        std::string DriverMfgName = driverInfoData.MfgName;
        addToMap(devAttrMap, DriverMfgName);

        std::string DriverProvider = driverInfoData.ProviderName;
        addToMap(devAttrMap, DriverProvider);

        std::string DriverType = std::to_string(driverInfoData.DriverType);
        addToMap(devAttrMap, DriverType);
    }

    // Try to get SCSI port info (This is for interface-less devices... it's disks already have interfaces with device paths that can get SCSI info)
    auto itr = deviceIdToScsiPortMap.find(DeviceId);
    if (itr != deviceIdToScsiPortMap.end())
    {
        std::string ScsiPortNumber = std::to_string(itr->second);
        std::string ScsiAdapterPath = "\\\\.\\SCSI" + ScsiPortNumber + ":";
        addToMap(devAttrMap, ScsiAdapterPath);
        addToMap(devAttrMap, ScsiPortNumber);
    }

    // Try to save the SP_DEVINFO_DATA... a bit sketchy but works!
    std::string __devInfoDataString(sizeof(SP_DEVINFO_DATA), '0');
    memcpy((void*)__devInfoDataString.c_str(), &devInfo, sizeof(SP_DEVINFO_DATA));
    addToMap(devAttrMap, __devInfoDataString);

    // Get DevNode Property Keys
    addOtherDevNodeProperties(devAttrMap, devInfo.DevInst);

    // Get Resource Descriptor Data
    addDeviceConfigurationAndResources(devAttrMap, devInfo.DevInst);

    // Get service information from the registry
    std::string subKey = REGISTRY_SERVICES + Service;
    std::string ServiceImagePath = "";
    if (getStringFromRegistry(HKEY_LOCAL_MACHINE, subKey, "ImagePath", ServiceImagePath))
    {
        // Sometimes this path will start with \\SystemRoot\\System32 and sometimes \\System32\\
        //   Try to standardize it.
        char systemDirectory[MAX_PATH] = { '\0' };
        if (GetSystemDirectory(systemDirectory, MAX_PATH))
        {
            if (startsWith(SYSTEM_ROOT1, ServiceImagePath, true))
            {
                ServiceImagePath = systemDirectory + ServiceImagePath.substr(strlen(SYSTEM_ROOT1));
            }
            else if (startsWith(SYSTEM_ROOT2, ServiceImagePath, true))
            {
                ServiceImagePath = systemDirectory + ServiceImagePath.substr(strlen(SYSTEM_ROOT2));
            }
        }
        addToMap(devAttrMap, ServiceImagePath);
    }

    UINT64 u = 0;
    if (getUIntFromRegistry(HKEY_LOCAL_MACHINE, subKey, "ErrorControl", u))
    {
        std::string ServiceErrorControl = errorControlToString((DWORD)u);
        addToMap(devAttrMap, ServiceErrorControl);
    }

    u = 0;
    if (getUIntFromRegistry(HKEY_LOCAL_MACHINE, subKey, "Start", u))
    {
        std::string ServiceStartType = startTypeToString((DWORD)u);
        addToMap(devAttrMap, ServiceStartType);
    }
    u = 0;
    if (getUIntFromRegistry(HKEY_LOCAL_MACHINE, subKey, "Type", u))
    {
        std::string ServiceType = serviceTypeToString((DWORD)u);
        addToMap(devAttrMap, ServiceType);
    }

    std::string ObjectName = "";
    if (getStringFromRegistry(HKEY_LOCAL_MACHINE, subKey, "ObjectName", ObjectName))
    {
        addToMap(devAttrMap, ObjectName);
    }


    //GetSystemDirectory
    return devAttrMap;
}

void addOtherDevNodeProperties(AttributeMap &attributeMap, DEVINST devInst)
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

        std::string key = propertyKeyToString(propertyKeyArray[i]);
        std::string value = propertyBufferToString(propertyBuffer, propertyBufferSize, propertyType, propertyKeyArray[i]);
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

        if (attributeMap.find(modifiedKey) == attributeMap.end())
        {
            attributeMap[modifiedKey] = value;
        }
        else // the modified key is in the map already. If it's value matches this one don't add.
             // If the value doesn't match add this value under the original (non-modified) key
        {
            if (toUpper(std::string(attributeMap[modifiedKey])) != toUpper(std::string(value)))
            {
                attributeMap[key] = value;
            }
        }

        delete[] propertyBuffer;
    }

end:
    delete[] propertyKeyArray;
}

void addDeviceConfigurationAndResources(AttributeMap &attributeMap, DEVINST devInst)
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

            std::string resourceTypeAsString = resourceTypeToString(resourceType);
            std::string resourceTypeAsStringWithNumber = resourceTypeAsString + "0";
            std::string resourceAsString = resourceToString(buffer, bufferSize, resourceType);
            delete[] buffer;

            while (attributeMap.find(resourceTypeAsStringWithNumber) != attributeMap.end())
            {
                int endNum = atoi(resourceTypeAsStringWithNumber.substr(resourceTypeAsString.size()).c_str());
                endNum++;
                resourceTypeAsStringWithNumber = resourceTypeAsString + std::to_string(endNum);
            }
            attributeMap[resourceTypeAsStringWithNumber] = resourceAsString;

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

void addInterfaceConfigurationAndResources(AttributeMap &attributeMap)
{
    // Make sure we have an interface path
    auto itr = attributeMap.find("DevicePath");
    if (itr == attributeMap.end())
    {
        return;
    }
    std::wstring interfacePath = stringToWString(itr->second);

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

        std::string key = propertyKeyToString(propertyKeyArray[i]);
        std::string value = propertyBufferToString(propertyBuffer, propertyBufferSize, propertyType, propertyKeyArray[i]);
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

        if (attributeMap.find(modifiedKey) == attributeMap.end())
        {
            attributeMap[modifiedKey] = value;
        }
        else // the modified key is in the map already. If it's value matches this one don't add.
             // If the value doesn't match add this value under the original (non-modified) key
        {
            if (toUpper(std::string(attributeMap[modifiedKey])) != toUpper(std::string(value)))
            {
                attributeMap[key] = value;
            }
        }
        delete[] propertyBuffer;
    }

    delete[] propertyKeyArray;
}

std::vector<AttributeMap> getInterfaceAttributeMap(GUID classGuid)
{
    std::vector<AttributeMap> interfaces;

    if (classGuid == GUID_NULL)
    {
#ifdef MULTITHREADED
        std::vector<std::future<std::vector<AttributeMap>>> vectorFutureVectorOfAttributeMaps;
#else // SINGLETHEADED
        std::vector<std::vector<AttributeMap>> vectorOfVectorOfAttributeMaps;
#endif // SINGLETHEADED
        for (auto guid : ALL_GUIDS)
        {
#ifdef MULTITHREADED
            // multithreading
            vectorFutureVectorOfAttributeMaps.push_back(std::async(getInterfaceAttributeMap, guid));
#else // SINGLETHEADED
            vectorOfVectorOfAttributeMaps.push_back(getInterfaceAttributeMap(guid));
#endif // SINGLETHEADED
        }

#ifdef MULTITHREADED
        // join
        for (std::future<std::vector<AttributeMap>> &futureVectorOfAttributeMaps : vectorFutureVectorOfAttributeMaps)
        {
            for (AttributeMap &attributeMap : futureVectorOfAttributeMaps.get())
            {
#else // SINGLETHEADED
        for (std::vector<AttributeMap> &vectorOfAttributeMaps : vectorOfVectorOfAttributeMaps)
        {
            for (AttributeMap &attributeMap : vectorOfAttributeMaps)
            {
#endif // SINGLETHEADED
                bool merged = false;
                for (AttributeMap &interfaceMap : interfaces)
                {
                    if (attributeMap["DeviceId"] == interfaceMap["DeviceId"])
                    {
                        attributeMap = mergeAttributeMaps(interfaceMap, attributeMap);
                        merged = true;
                        break;
                    }
                }

                if (!merged)
                {
                    interfaces.push_back(attributeMap);
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
        return std::vector<AttributeMap>();
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
            AttributeMap devAttrMap;
            std::string InterfaceClassGuid = guidToString(interfaceInfo.InterfaceClassGuid);
            std::string DevicePath = interfaceDetail->DevicePath;

#ifdef MULTITHREADED
            std::future<AttributeMap> futureDevAttrMap = std::async(getDeviceAttributeMap, interfaceDevs, devInfo, ref(deviceIdToScsiPortMap));
#else // SINGLETHREADED
            devAttrMap = getDeviceAttributeMap(interfaceDevs, devInfo, deviceIdToScsiPortMap);
#endif // SINGLETHREADED

            AttributeMap otherAttrMap = getAttributeMapFromDevicePath(DevicePath, msDosDeviceNameToDriveLetterMap);

#ifdef MULTITHREADED
            devAttrMap = mergeAttributeMaps(futureDevAttrMap.get(), otherAttrMap);
#else // SINGLETHREADED
            mergeAttributeMaps(devAttrMap, otherAttrMap);
#endif // SINGLETHREADED

            addToMap(devAttrMap, InterfaceClassGuid);

            interfaces.push_back(devAttrMap);

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

std::vector<AttributeMap> getAllDevicesAttributeMap()
{
    HDEVINFO deviceDevs = getAllClassesHDevInfo();

    SP_DEVINFO_DATA devInfo;
    devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

    std::future<std::vector<AttributeMap>> futureCompleteDevicesAttrMap;
    std::vector<AttributeMap> completeDevicesAttrMap;

    DeviceIdToScsiPortMap deviceIdToScsiPortMap = getDeviceIdToScsiPortMap();

    if (deviceDevs == INVALID_HANDLE_VALUE)
    {
        goto exit;
    }

    {

#ifdef MULTITHREADED
        futureCompleteDevicesAttrMap = std::async(getInterfaceAttributeMap, GUID_NULL);
        std::vector<std::future<AttributeMap>> devAttrMapsToAdd;
#else // SINGLETHREADED
        completeDevicesAttrMap = getInterfaceAttributeMap(GUID_NULL);
        std::vector<AttributeMap> devAttrMapsToAdd;
#endif // SINGLETHREADED

        std::map<std::string, SP_DEVINFO_DATA> deviceIdToInfoData;

        for (int devIndex = 0; SetupDiEnumDeviceInfo(deviceDevs, devIndex, &devInfo); devIndex++)
        {
            std::string deviceId = getDeviceId(devInfo.DevInst);
            deviceIdToInfoData[deviceId] = devInfo;
        }

#ifdef MULTITHREADED
        completeDevicesAttrMap = futureCompleteDevicesAttrMap.get();
#endif // MULTITHEADED

        for (auto &dIAI : deviceIdToInfoData)
        {
            bool needToAdd = true;
            for (AttributeMap &i : completeDevicesAttrMap)
            {
                if (i["DeviceId"] == dIAI.first)
                {
                    needToAdd = false;
                    break;
                }
            }

            if (needToAdd)
            {
#ifdef MULTITHREADED
                devAttrMapsToAdd.push_back(std::async(getDeviceAttributeMap, deviceDevs, dIAI.second, ref(deviceIdToScsiPortMap)));
#else // SINGLETHREADED
                devAttrMapsToAdd.push_back(getDeviceAttributeMap(deviceDevs, dIAI.second, ref(deviceIdToScsiPortMap)));
#endif // SINGLETHREADED
            }
        }

#ifdef MULTITHREADED
        for (std::future<AttributeMap> &futureAttrMap : devAttrMapsToAdd)
        {
            completeDevicesAttrMap.push_back(futureAttrMap.get());
        }
#else // SINGLETHREADED
        for (AttributeMap &attrMap : devAttrMapsToAdd)
        {
            completeDevicesAttrMap.push_back(attrMap);
        }
#endif // SINGLETHREADED
    }

exit:
    // Frees the device HDEVINFO
    if (deviceDevs != INVALID_HANDLE_VALUE)
    {
        SetupDiDestroyDeviceInfoList(deviceDevs);
    }

    return completeDevicesAttrMap;
}

void printAttributeMap(AttributeMap &attrMap)
{
    std::regex newlineRegex = std::regex("\n");
    for (auto i : attrMap)
    {
        // Hide 'private' attributes
        if (!startsWith("__", i.first, true))
        {
            std::string value = i.second;

            std::string spaces = "                           ";
            while (spaces.size() < i.first.size() + 2)
            {
                spaces += " ";
            }

            value = std::regex_replace(value, newlineRegex, "\n" + spaces);

            printf("%-25s: %s\n", i.first.c_str(), value.c_str());
        }
    }
    puts("*******************************************************");
}

AttributeMap getAttributeMapWith(std::string key, std::string value)
{
    std::vector<AttributeMap> devicesAttributeMap = getAllDevicesAttributeMap();
    for (auto &deviceAttrMap : devicesAttributeMap)
    {
        auto itr = deviceAttrMap.find(key);
        if (itr != deviceAttrMap.end())
        {
            if (SymMatchString(itr->second.c_str(), value.c_str(), FALSE))
            {
                return deviceAttrMap;
            }
        }
    }
    return AttributeMap();
}

DEVINST getDevInstWith(std::string key, std::string value)
{
    AttributeMap deviceAttrMap = getAttributeMapWith(key, value);
    if (!deviceAttrMap.empty())
    {
        PSP_DEVINFO_DATA pDevInfoData = (PSP_DEVINFO_DATA)deviceAttrMap[DEVINFO_DATA_STRING].c_str();
        return pDevInfoData->DevInst;
    }
    return NULL;
}

DeviceIdToScsiPortMap getDeviceIdToScsiPortMap()
{
    DeviceIdToScsiPortMap deviceIdToScsiPort; printf("hi");
    std::map<int, std::string> scsiPortToDriver;
    std::map <std::string, std::vector <std::string>> driverToDeviceIdsMap;

    int currentPort = 0;
    std::string currentDriver;
    while (getStringFromRegistry(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port " + std::to_string(currentPort), "Driver", currentDriver))
    {
        scsiPortToDriver[currentPort] = rTrim(currentDriver);
        currentPort++;

        if (driverToDeviceIdsMap.find(currentDriver) == driverToDeviceIdsMap.end())
        {
            std::vector<std::string> deviceIds;
            std::string deviceId;
            int currentDeviceIdIndex = 0;
            std::string subKey = "SYSTEM\\CurrentControlSet\\Services\\" + std::string(currentDriver) + "\\Enum";
            while (getStringFromRegistry(HKEY_LOCAL_MACHINE, subKey, std::to_string(currentDeviceIdIndex), deviceId))
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

AttributeMap &mergeAttributeMaps(AttributeMap &oldAMap, AttributeMap &newAMap)
{
    for (auto &i : oldAMap)
    {
        // It's rare but sometimes the newAMap won't have something in the oldAMap (or vice-versa)
        if (newAMap.find(i.first) != newAMap.end())
        {
            // Don't merge private vars... take 1 of them
            if (i.second != newAMap[i.first] & !startsWith("__", i.first, true))
            {
                // Merge by adding attributes that are different
                i.second += "\n" + newAMap[i.first];
            }
        }
    }

    // If something is just in the newAMap, we need to bring it over to the oldAMap
    for (auto &i : newAMap)
    {
        if (oldAMap.find(i.first) == oldAMap.end())
        {
            oldAMap[i.first] = i.second;
        }
    }

    return oldAMap;
}

AttributeMap getAttributeMapFromDevicePath(std::string DevicePath, std::map<std::string, std::string> &msDosDeviceNameToDriveLetterMap)
{
    AttributeMap devAttrMap;
    devAttrMap["DevicePath"] = DevicePath;

    char targetPaths[4096];
    // substr(4) to get rid of the \\.\ 
    DWORD targetPathsLength = QueryDosDevice(DevicePath.substr(4).c_str(), targetPaths, 4096);
    if (targetPathsLength != 0)
    {
        std::string MSDosDeviceName = std::string(targetPaths, targetPathsLength);
        MSDosDeviceName = delimitedStringToNewlineString(MSDosDeviceName);
        addToMap(devAttrMap, MSDosDeviceName);

        // Map to drive letter... not quite sure what would happen if a volume spans multple MSDosDevices...
        if (msDosDeviceNameToDriveLetterMap.find(MSDosDeviceName) != msDosDeviceNameToDriveLetterMap.end())
        {
            std::string DriveLetter = msDosDeviceNameToDriveLetterMap[MSDosDeviceName];
            addToMap(devAttrMap, DriveLetter);

            std::string driveLetterWithBackslash = DriveLetter + "\\";
            memset(&targetPaths, '\0', sizeof(targetPaths));

            // Volume information
            if (GetVolumeNameForVolumeMountPoint(driveLetterWithBackslash.c_str(), targetPaths, sizeof(targetPaths)))
            {
                std::string VolumeName = targetPaths;
                addToMap(devAttrMap, VolumeName);
            }
        }
    }

    addInterfaceConfigurationAndResources(devAttrMap);

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
        BYTE b[4096] = { 0 };
        DWORD bytesReturned;
        STORAGE_DEVICE_NUMBER storageDeviceNumber = { 0 };
        if (DeviceIoControl(handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &storageDeviceNumber, sizeof(STORAGE_DEVICE_NUMBER), &bytesReturned, NULL) && bytesReturned != 0)
        {
            std::string PhysicalDrivePath = "\\\\.\\PHYSICALDRIVE" + std::to_string(storageDeviceNumber.DeviceNumber);
            addToMap(devAttrMap, PhysicalDrivePath);

            // substr(4) to get rid of the \\.\ 
            DWORD targetPathsLength = QueryDosDevice(PhysicalDrivePath.substr(4).c_str(), targetPaths, 4096);
            if (targetPathsLength != 0)
            {
                std::string MSDosPhysicalDriveDeviceName = std::string(targetPaths, targetPathsLength);
                MSDosPhysicalDriveDeviceName = delimitedStringToNewlineString(MSDosPhysicalDriveDeviceName);
                addToMap(devAttrMap, MSDosPhysicalDriveDeviceName);
            }

            std::string PartitionNumber;
            if (storageDeviceNumber.PartitionNumber == (DWORD)-1)
            {
                PartitionNumber = "<Device Cannot Be Partitioned>";
            }
            else
            {
                PartitionNumber = std::to_string(storageDeviceNumber.PartitionNumber);
            }
            addToMap(devAttrMap, PartitionNumber);

            std::string DeviceType = std::to_string(storageDeviceNumber.DeviceType);
            addToMap(devAttrMap, DeviceType);
        }

        SCSI_ADDRESS scsiAddress = { 0 };
        if (DeviceIoControl(handle, IOCTL_SCSI_GET_ADDRESS, NULL, 0, &scsiAddress, sizeof(SCSI_ADDRESS), &bytesReturned, NULL))
        {
            if (scsiAddress.Length == sizeof(SCSI_ADDRESS))
            {
                std::string ScsiAdapterPath = "\\\\.\\SCSI" + std::to_string(scsiAddress.PortNumber) + ":";
                addToMap(devAttrMap, ScsiAdapterPath);

                // substr(4) to get rid of the \\.\ 
                DWORD targetPathsLength = QueryDosDevice(ScsiAdapterPath.substr(4).c_str(), targetPaths, 4096);
                if (targetPathsLength != 0)
                {
                    std::string MSDosAdapterName = std::string(targetPaths, targetPathsLength);
                    MSDosAdapterName = delimitedStringToNewlineString(MSDosAdapterName);
                    addToMap(devAttrMap, MSDosAdapterName);
                }

                std::string ScsiPathId = std::to_string(scsiAddress.PathId);
                addToMap(devAttrMap, ScsiPathId);

                std::string ScsiLun = std::to_string(scsiAddress.Lun);
                addToMap(devAttrMap, ScsiLun);

                std::string ScsiTargetId = std::to_string(scsiAddress.TargetId);
                addToMap(devAttrMap, ScsiTargetId);

                std::string ScsiPortNumber = std::to_string(scsiAddress.PortNumber);
                addToMap(devAttrMap, ScsiPortNumber);

                // Get IO_SCSI_CAPABILITIES
                IO_SCSI_CAPABILITIES ioScsiCapabilities = { 0 };
                if (DeviceIoControl(handle, IOCTL_SCSI_GET_CAPABILITIES, NULL, 0, &ioScsiCapabilities, sizeof(IO_SCSI_CAPABILITIES), &bytesReturned, NULL))
                {
                    std::string MaximumTransferLength = std::to_string(ioScsiCapabilities.MaximumTransferLength);
                    addToMap(devAttrMap, MaximumTransferLength);

                    std::string MaximumPhysicalPages = std::to_string(ioScsiCapabilities.MaximumPhysicalPages);
                    addToMap(devAttrMap, MaximumPhysicalPages);

                    std::string SupportedAsynchronousEvents = toBoolString(ioScsiCapabilities.SupportedAsynchronousEvents);
                    addToMap(devAttrMap, SupportedAsynchronousEvents);

                    std::string AlignmentMask = std::to_string(ioScsiCapabilities.AlignmentMask);
                    addToMap(devAttrMap, AlignmentMask);

                    std::string TaggedQueuing = toBoolString(ioScsiCapabilities.TaggedQueuing);
                    addToMap(devAttrMap, TaggedQueuing);

                    std::string AdapterScansDown = toBoolString(ioScsiCapabilities.AdapterScansDown);
                    addToMap(devAttrMap, AdapterScansDown);

                    std::string AdapterUsesPio = toBoolString(ioScsiCapabilities.AdapterUsesPio);
                    addToMap(devAttrMap, AdapterUsesPio);
                }
            }
        }

        // Get hotplug info
        STORAGE_HOTPLUG_INFO storageHotplugInfo = { 0 };
        if (DeviceIoControl(handle, IOCTL_STORAGE_GET_HOTPLUG_INFO, NULL, 0, &storageHotplugInfo, sizeof(STORAGE_HOTPLUG_INFO), &bytesReturned, NULL) && bytesReturned != 0)
        {
            std::string MediaRemovable = toBoolString(storageHotplugInfo.MediaRemovable);
            addToMap(devAttrMap, MediaRemovable);

            std::string MediaHotplug = toBoolString(storageHotplugInfo.MediaHotplug);
            addToMap(devAttrMap, MediaHotplug);

            std::string DeviceHotplug = toBoolString(storageHotplugInfo.DeviceHotplug);
            addToMap(devAttrMap, DeviceHotplug);
        }

        // Get media serial number
        PMEDIA_SERIAL_NUMBER_DATA  mediaSerialNumberData = (PMEDIA_SERIAL_NUMBER_DATA)b;
        if (DeviceIoControl(handle, IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER, NULL, 0, &mediaSerialNumberData, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string MediaSerialNumber = std::string((char*)mediaSerialNumberData->SerialNumberData, mediaSerialNumberData->SerialNumberLength);
            addToMap(devAttrMap, MediaSerialNumber);
        }

        // Get media serial number
        STORAGE_READ_CAPACITY storageReadCapacity = { 0 };
        if (DeviceIoControl(handle, IOCTL_STORAGE_READ_CAPACITY, NULL, 0, &storageReadCapacity, sizeof(STORAGE_READ_CAPACITY), &bytesReturned, NULL) && bytesReturned != 0)
        {
            std::string BlockLength = std::to_string(storageReadCapacity.BlockLength);
            addToMap(devAttrMap, BlockLength);

            std::string NumberOfBlocks = std::to_string(storageReadCapacity.NumberOfBlocks.QuadPart);
            addToMap(devAttrMap, NumberOfBlocks);

            std::string DiskLength = std::to_string(storageReadCapacity.DiskLength.QuadPart);
            addToMap(devAttrMap, DiskLength);
        }

        // Get storage unique identifier
        PSTORAGE_DEVICE_UNIQUE_IDENTIFIER pStorageDeviceUniqueIdentifer = (PSTORAGE_DEVICE_UNIQUE_IDENTIFIER)b;
        STORAGE_PROPERTY_QUERY storagePropertyQuery;
        memset(&storagePropertyQuery, 0, sizeof(STORAGE_PROPERTY_QUERY));
        storagePropertyQuery.PropertyId = StorageDeviceUniqueIdProperty;
        storagePropertyQuery.QueryType = PropertyStandardQuery;
        if (DeviceIoControl(handle, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY), &b, 4096, &bytesReturned, NULL) && bytesReturned != 0)
        {
            PSTORAGE_DEVICE_ID_DESCRIPTOR pStorageDeviceIdDescriptor = (PSTORAGE_DEVICE_ID_DESCRIPTOR)(b + pStorageDeviceUniqueIdentifer->StorageDeviceIdOffset);

            PSTORAGE_DEVICE_DESCRIPTOR pStorageDeviceDescriptor = (PSTORAGE_DEVICE_DESCRIPTOR)(b + pStorageDeviceUniqueIdentifer->StorageDeviceOffset);

            std::string CommandQueueing = toBoolString(pStorageDeviceDescriptor->CommandQueueing);
            addToMap(devAttrMap, CommandQueueing);

            if (pStorageDeviceDescriptor->VendorIdOffset != 0)
            {
                std::string VendorId = rTrim(std::string((char*)pStorageDeviceDescriptor + pStorageDeviceDescriptor->VendorIdOffset));
                addToMap(devAttrMap, VendorId);
            }

            if (pStorageDeviceDescriptor->ProductIdOffset != 0)
            {
                std::string ProductId = rTrim(std::string((char*)pStorageDeviceDescriptor + pStorageDeviceDescriptor->ProductIdOffset));
                addToMap(devAttrMap, ProductId);
            }

            if (pStorageDeviceDescriptor->ProductRevisionOffset != 0)
            {
                std::string ProductRevision = rTrim(std::string((char*)pStorageDeviceDescriptor + pStorageDeviceDescriptor->ProductRevisionOffset));
                addToMap(devAttrMap, ProductRevision);
            }

            if (pStorageDeviceDescriptor->SerialNumberOffset != 0)
            {
                std::string SerialNumber = rTrim(std::string((char*)pStorageDeviceDescriptor + pStorageDeviceDescriptor->SerialNumberOffset + 1));
                addToMap(devAttrMap, SerialNumber);
            }

            std::string BusType = storageBusTypeToString(pStorageDeviceDescriptor->BusType);
            addToMap(devAttrMap, BusType);

            PSTORAGE_DEVICE_LAYOUT_SIGNATURE pStorageDeviceLayoutSignature = (PSTORAGE_DEVICE_LAYOUT_SIGNATURE)(b + pStorageDeviceUniqueIdentifer->DriveLayoutSignatureOffset);
            // If MBR give that info, otherwise give GPT GUID
            if (pStorageDeviceLayoutSignature->Mbr)
            {
                std::string MbrSignature = std::to_string(pStorageDeviceLayoutSignature->DeviceSpecific.MbrSignature);
                addToMap(devAttrMap, MbrSignature)
            }
            else
            {
                std::string GptDiskId = guidToString(pStorageDeviceLayoutSignature->DeviceSpecific.GptDiskId);
                addToMap(devAttrMap, GptDiskId);
            }

            // Everything together... useful because a user can use CompareStorageDuids(...) to check for exact/sub matches
            std::string StorageDUID = byteArrayToString(b, bytesReturned);
            addToMap(devAttrMap, StorageDUID);
        }

        if (DeviceIoControl(handle, IOCTL_HID_GET_MANUFACTURER_STRING, NULL, NULL, b, 128, &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string Manufacturer = wStringToString((wchar_t*)b);
            addToMap(devAttrMap, Manufacturer);
        }

        if (DeviceIoControl(handle, IOCTL_HID_GET_PRODUCT_STRING, NULL, NULL, b, 128, &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string ProductId = wStringToString((wchar_t*)b);
            addToMap(devAttrMap, ProductId);
        }

        if (DeviceIoControl(handle, IOCTL_HID_GET_SERIALNUMBER_STRING, NULL, NULL, b, 128, &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string SerialNumber = wStringToString((wchar_t*)b);
            addToMap(devAttrMap, SerialNumber);
        }

        STORAGE_PREDICT_FAILURE storagePredictFailure = { 0 };
        if (DeviceIoControl(handle, IOCTL_STORAGE_PREDICT_FAILURE, NULL, NULL, &storagePredictFailure, sizeof(STORAGE_PREDICT_FAILURE), &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string PredictFailure = toBoolString(storagePredictFailure.PredictFailure);
            addToMap(devAttrMap, PredictFailure);

            // Get SMART Thresholds
            memset(&b, 0, sizeof(b));
            PSENDCMDINPARAMS smartSendCmdParams = (PSENDCMDINPARAMS)b;
            smartSendCmdParams->cBufferSize = sizeof(b);
            IDEREGS ideRegs = { 0 };
            ideRegs.bFeaturesReg = READ_THRESHOLDS;
            ideRegs.bCylLowReg = SMART_CYL_LOW;
            ideRegs.bCylHighReg = SMART_CYL_HI;
            ideRegs.bCommandReg = SMART_CMD;
            smartSendCmdParams->irDriveRegs = ideRegs;
            BYTE* smartThresholds = NULL;

            if (DeviceIoControl(handle, SMART_RCV_DRIVE_DATA, b, sizeof(b), b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 512)
            {
                // This math comes from the SMART Thresholds being in the last 512 bytes. Don't care about the rest.
                smartThresholds = b + (bytesReturned - READ_THRESHOLD_BUFFER_SIZE);
            }

            std::string SMARTData = smartToString((BYTE*)storagePredictFailure.VendorSpecific, READ_THRESHOLD_BUFFER_SIZE, smartThresholds);

            // SMART Return Status (should say if a threshold exceeded condition)
            memset(&b, 0, sizeof(b));
            smartSendCmdParams->cBufferSize = sizeof(b);
            ideRegs.bFeaturesReg = RETURN_SMART_STATUS;
            ideRegs.bCylLowReg = SMART_CYL_LOW;
            ideRegs.bCylHighReg = SMART_CYL_HI;
            ideRegs.bCommandReg = SMART_CMD;
            smartSendCmdParams->irDriveRegs = ideRegs;
            if (DeviceIoControl(handle, SMART_SEND_DRIVE_COMMAND, b, sizeof(b), b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
            {
                std::string SMARTReturnStatus = "Healthy";
                if (ideRegs.bCylLowReg == BAD_SMART_LOW && ideRegs.bCylHighReg == BAD_SMART_HIGH)
                {
                    SMARTReturnStatus = "Threshold Exceeded Condition";
                }
                addToMap(devAttrMap, SMARTReturnStatus);
            }

            addToMap(devAttrMap, SMARTData);
        }

        DISK_GEOMETRY_EX diskGeoEx = { 0 };
        if (DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, NULL, &diskGeoEx, sizeof(DISK_GEOMETRY_EX), &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string DiskSize = std::to_string(diskGeoEx.DiskSize.QuadPart) + " Bytes" + " (" + std::to_string(diskGeoEx.DiskSize.QuadPart / (double)BYTES_IN_GIGABYTE) + " Gigabytes)";
            addToMap(devAttrMap, DiskSize);
        }

        USB_HUB_CAPABILITIES_EX usbHubCapabilities = { 0 };
        if (DeviceIoControl(handle, IOCTL_USB_GET_HUB_CAPABILITIES_EX, NULL, NULL, &usbHubCapabilities, sizeof(USB_HUB_CAPABILITIES_EX), &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string HubIsHighSpeedCapable = toBoolString(usbHubCapabilities.CapabilityFlags.HubIsHighSpeedCapable);
            addToMap(devAttrMap, HubIsHighSpeedCapable);

            std::string HubIsHighSpeed = toBoolString(usbHubCapabilities.CapabilityFlags.HubIsHighSpeed);
            addToMap(devAttrMap, HubIsHighSpeed);

            std::string HubIsMultiTransactionCapable = toBoolString(usbHubCapabilities.CapabilityFlags.HubIsMultiTtCapable);
            addToMap(devAttrMap, HubIsMultiTransactionCapable);

            std::string HubIsMultiTransaction = toBoolString(usbHubCapabilities.CapabilityFlags.HubIsMultiTt);
            addToMap(devAttrMap, HubIsMultiTransaction);

            std::string HubIsRoot = toBoolString(usbHubCapabilities.CapabilityFlags.HubIsRoot);
            addToMap(devAttrMap, HubIsRoot);

            std::string HubIsArmedWakeOnConnect = toBoolString(usbHubCapabilities.CapabilityFlags.HubIsArmedWakeOnConnect);
            addToMap(devAttrMap, HubIsArmedWakeOnConnect);

            std::string HubIsBusPowered = toBoolString(usbHubCapabilities.CapabilityFlags.HubIsBusPowered);
            addToMap(devAttrMap, HubIsBusPowered);
        }

        USB_NODE_INFORMATION usbNodeInfo;
        memset(&usbNodeInfo, 0, sizeof(USB_NODE_INFORMATION));
        // MSDN says to pass the USB_NODE_INFORMATION as input as well and set NodeType... I don't think it does anything on input... Also requesting more than 6 because it seems to pass with invalid data at 6.
        if (DeviceIoControl(handle, IOCTL_USB_GET_NODE_INFORMATION, &usbNodeInfo, sizeof(USB_NODE_INFORMATION), &usbNodeInfo, sizeof(USB_NODE_INFORMATION), &bytesReturned, NULL) && bytesReturned > 6)
        {
            if (usbNodeInfo.NodeType == UsbMIParent)
            {
                std::string ParentUSBNodeType = "UsbMiParent";
                addToMap(devAttrMap, ParentUSBNodeType);

                std::string ParentNumberOfInterfaces = std::to_string(usbNodeInfo.u.MiParentInformation.NumberOfInterfaces);
                addToMap(devAttrMap, ParentNumberOfInterfaces);
            }
            else if (usbNodeInfo.NodeType == UsbHub)
            {
                std::string ParentUSBNodeType = "UsbHub";
                addToMap(devAttrMap, ParentUSBNodeType);

                std::string ParentNumberOfHubPorts = std::to_string(usbNodeInfo.u.HubInformation.HubDescriptor.bNumberOfPorts);
                addToMap(devAttrMap, ParentNumberOfHubPorts);

                // Apparently there is more info on this in the USB spec... possible todo: decode it.
                std::string ParentHubCharacteristics = std::to_string(usbNodeInfo.u.HubInformation.HubDescriptor.wHubCharacteristics);
                addToMap(devAttrMap, ParentHubCharacteristics);

                // Number is in 2-ms intervals
                std::string ParentHubTimeToPowerOn = std::to_string(usbNodeInfo.u.HubInformation.HubDescriptor.bPowerOnToPowerGood * 2) + " Milliseconds";
                addToMap(devAttrMap, ParentHubTimeToPowerOn);

                // Number is in milliamperes
                std::string ParentHubMaxCurrent = std::to_string(usbNodeInfo.u.HubInformation.HubDescriptor.bHubControlCurrent) + " Milliamperes";
                addToMap(devAttrMap, ParentHubMaxCurrent);

                std::string ParentIsBusPowered = toBoolString(usbNodeInfo.u.HubInformation.HubIsBusPowered);
                addToMap(devAttrMap, ParentIsBusPowered);
            }
        }

        memset(&b, 0, sizeof(b));
        PUSB_ROOT_HUB_NAME usbRootHubName = (PUSB_ROOT_HUB_NAME)b;
        if (DeviceIoControl(handle, IOCTL_USB_GET_ROOT_HUB_NAME, NULL, NULL, usbRootHubName, 1024, &bytesReturned, NULL) && bytesReturned > 0 && usbRootHubName->ActualLength > 0)
        {
            std::string RootHubName = wStringToString(std::wstring((wchar_t*)usbRootHubName->RootHubName));
            addToMap(devAttrMap, RootHubName);
        }

        memset(&b, 0, sizeof(b));
        PVOLUME_DISK_EXTENTS volumeDiskExtents = (PVOLUME_DISK_EXTENTS)b;
        if (DeviceIoControl(handle, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, NULL, &b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            std::string NumberOfDiskExtents = std::to_string(volumeDiskExtents->NumberOfDiskExtents);
            addToMap(devAttrMap, NumberOfDiskExtents);

            DISK_EXTENT * thisExtent = volumeDiskExtents->Extents;
            for (size_t i = 0; i < volumeDiskExtents->NumberOfDiskExtents; i++)
            {
                std::string DiskExtent = "Disk Number: " + std::to_string(thisExtent->DiskNumber) + "\n";
                DiskExtent += "Starting Offset: Byte " + std::to_string(thisExtent->StartingOffset.QuadPart) + "\n";
                DiskExtent += "Extent Length: " + std::to_string(thisExtent->ExtentLength.QuadPart) + " Bytes";
                devAttrMap["DiskExtent" + std::to_string(i)] = DiskExtent;

                // Should do pointer math and move to the next extent
                thisExtent++;
            }
        }

        if (DeviceIoControl(handle, FSCTL_GET_NTFS_VOLUME_DATA, NULL, NULL, &b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            PNTFS_VOLUME_DATA_BUFFER p = (PNTFS_VOLUME_DATA_BUFFER)b;

            std::string VolumeSerialNumber = std::to_string(p->VolumeSerialNumber.QuadPart);
            addToMap(devAttrMap, VolumeSerialNumber);

            std::string NumberSectors = std::to_string(p->NumberSectors.QuadPart);
            addToMap(devAttrMap, NumberSectors);

            std::string TotalClusters = std::to_string(p->TotalClusters.QuadPart);
            addToMap(devAttrMap, TotalClusters);

            std::string FreeClusters = std::to_string(p->FreeClusters.QuadPart);
            addToMap(devAttrMap, FreeClusters);

            std::string TotalReserved = std::to_string(p->TotalReserved.QuadPart);
            addToMap(devAttrMap, TotalReserved);

            std::string BytesPerSector = std::to_string(p->BytesPerSector);
            addToMap(devAttrMap, BytesPerSector);

            std::string BytesPerCluster = std::to_string(p->BytesPerCluster);
            addToMap(devAttrMap, BytesPerCluster);

            std::string BytesPerFileRecordSegment = std::to_string(p->BytesPerFileRecordSegment);
            addToMap(devAttrMap, BytesPerFileRecordSegment);

            std::string ClustersPerFileRecordSegment = std::to_string(p->ClustersPerFileRecordSegment);
            addToMap(devAttrMap, ClustersPerFileRecordSegment);

            std::string MftValidDataLength = std::to_string(p->MftValidDataLength.QuadPart);
            addToMap(devAttrMap, MftValidDataLength);

            std::string MftStartLcn = std::to_string(p->MftStartLcn.QuadPart);
            addToMap(devAttrMap, MftStartLcn);

            std::string Mft2StartLcn = std::to_string(p->Mft2StartLcn.QuadPart);
            addToMap(devAttrMap, Mft2StartLcn);

            std::string MftZoneStart = std::to_string(p->MftZoneStart.QuadPart);
            addToMap(devAttrMap, MftZoneStart);

            std::string MftZoneEnd = std::to_string(p->MftZoneEnd.QuadPart);
            addToMap(devAttrMap, MftZoneEnd);
        }

        if (DeviceIoControl(handle, IOCTL_VOLUME_GET_GPT_ATTRIBUTES, NULL, NULL, &b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            PVOLUME_GET_GPT_ATTRIBUTES_INFORMATION p = (PVOLUME_GET_GPT_ATTRIBUTES_INFORMATION)b;

            std::string GPTAttributes = gptAttributesToString(p->GptAttributes);
            addToMap(devAttrMap, GPTAttributes);
        }

        if (DeviceIoControl(handle, IOCTL_SERIAL_GET_COMMSTATUS, NULL, 0, b, sizeof(b), &bytesReturned, NULL) && bytesReturned > 0)
        {
            PULONG baud = (PULONG)b;
            std::string BaudRate = std::to_string(*baud);
            addToMap(devAttrMap, BaudRate);
        }

        memset(&b, 0, sizeof(b));
        DWORD volumeSerialNumber = 0;
        DWORD maximumComponentLength = 0;
        DWORD fileSystemFlags = 0;
        wchar_t fileSystemNameBuffer[4096] = { '\0' };
        if (GetVolumeInformationByHandleW(handle, (LPWSTR)b, sizeof(b), &volumeSerialNumber, &maximumComponentLength, &fileSystemFlags, fileSystemNameBuffer, 4096))
        {
            std::string VolumeInformation = wStringToString(std::wstring((wchar_t*)b));
            addToMap(devAttrMap, VolumeInformation);

            std::string VolumeSerialNumber = std::to_string(volumeSerialNumber);
            addToMap(devAttrMap, VolumeSerialNumber);

            std::string MaximumComponentLength = std::to_string(maximumComponentLength);
            addToMap(devAttrMap, MaximumComponentLength);

            std::string FileSystemFlags = fileSystemFlagToString(fileSystemFlags);
            addToMap(devAttrMap, FileSystemFlags);

            std::string FileSystemName = wStringToString(std::wstring(fileSystemNameBuffer));
            addToMap(devAttrMap, FileSystemName);
        }

        CloseHandle(handle);
    }

    return devAttrMap;
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