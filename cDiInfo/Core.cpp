/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Core.cpp - Core SetupDi-interfacing implementation file
*/

// Local includes
#include "Core.h"

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
    char buffer[4096] = { 0 };
    DWORD reqSize;
    DWORD dataType;

    if (SetupDiGetDeviceRegistryProperty(devs, devInfo, property, &dataType, (LPBYTE)buffer, 4096, &reqSize))
    {
        std::string retStr = std::string(buffer, reqSize);
        if (retType == _INT_)
        {
            return std::to_string(*(DWORD*)retStr.c_str());
        }
        else if (retType == __STRING_)
        {
            // Handle \0 delimited lists
            std::replace(retStr.begin(), retStr.end(), '\0', '\n');
            retStr = std::regex_replace(retStr, std::regex("\n\n"), "\n");

            return retStr;
        }
        else if (retType == _GUID_)
        {
            wchar_t guidCharArr[4096] = { 0 };

            GUID g = *((GUID*)retStr.c_str());
            int size = StringFromGUID2(g, guidCharArr, 4096);

            std::wstring asWStr = std::wstring(guidCharArr);
            return std::string(asWStr.begin(), asWStr.end());
        }
        else if (retType == __WSTRING_)
        {
            std::wstring tmpWStr((wchar_t*)retStr.c_str());
            retStr = std::string(tmpWStr.begin(), tmpWStr.end());
            return retStr;
        }
    }
    return std::string(UNAVAILABLE_ATTRIBUTE);
}

bool getDriverInfoData(HDEVINFO &devs, SP_DEVINFO_DATA &devInfo, PSP_DRVINFO_DATA driverInfoData)
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
    CM_Get_Device_ID_Size(&deviceIdSize, devInst, 0);
    std::string deviceId(" ", deviceIdSize + 1);
    if (CM_Get_Device_ID(devInst, (char*)(deviceId.c_str()), deviceIdSize + 1, 0) == CR_SUCCESS)
    {
        deviceId[deviceIdSize] = ' ';
        return rTrim(deviceId);
    }
    return UNAVAILABLE_ATTRIBUTE;
}

AttributeMap getDeviceAttributeMap(HDEVINFO &devs, SP_DEVINFO_DATA &devInfo, std::map<int, std::string> &scsiPortToDeviceIdMap)
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
    std::string Classguid = getDevInfoProperty(devs, &devInfo, SPDRP_CLASSGUID, __STRING_);
    addToMap(devAttrMap, Classguid);
    std::string Driver = getDevInfoProperty(devs, &devInfo, SPDRP_DRIVER, __STRING_);
    addToMap(devAttrMap, Driver);
    std::string Configflags = getDevInfoProperty(devs, &devInfo, SPDRP_CONFIGFLAGS, _INT_);
    addToMap(devAttrMap, Configflags);
    std::string Mfg = getDevInfoProperty(devs, &devInfo, SPDRP_MFG, __STRING_);
    addToMap(devAttrMap, Mfg);
    std::string LocationInformation = getDevInfoProperty(devs, &devInfo, SPDRP_LOCATION_INFORMATION, __STRING_);
    addToMap(devAttrMap, LocationInformation);
    std::string PhysicalDeviceObjectName = getDevInfoProperty(devs, &devInfo, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, __STRING_);
    addToMap(devAttrMap, PhysicalDeviceObjectName);
    std::string Capabilities = getDevInfoProperty(devs, &devInfo, SPDRP_CAPABILITIES, _INT_);
    addToMap(devAttrMap, Capabilities);
    std::string UiNumber = getDevInfoProperty(devs, &devInfo, SPDRP_UI_NUMBER, _INT_);
    addToMap(devAttrMap, UiNumber);
    std::string Upperfilters = getDevInfoProperty(devs, &devInfo, SPDRP_UPPERFILTERS, __STRING_);
    addToMap(devAttrMap, Upperfilters);
    std::string Lowerfilters = getDevInfoProperty(devs, &devInfo, SPDRP_LOWERFILTERS, __STRING_);
    addToMap(devAttrMap, Lowerfilters);
    std::string BusTypeGuid = getDevInfoProperty(devs, &devInfo, SPDRP_BUSTYPEGUID, _GUID_);
    addToMap(devAttrMap, BusTypeGuid);
    std::string Legacybustype = getDevInfoProperty(devs, &devInfo, SPDRP_LEGACYBUSTYPE, _INT_);
    addToMap(devAttrMap, Legacybustype);
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
    std::string DevicePowerData = getDevInfoProperty(devs, &devInfo, SPDRP_DEVICE_POWER_DATA, _INT_);
    addToMap(devAttrMap, DevicePowerData);
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
    std::string BaseContainerid = getDevInfoProperty(devs, &devInfo, SPDRP_BASE_CONTAINERID, __WSTRING_);
    addToMap(devAttrMap, BaseContainerid);

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
    std::string NodeProblemNumber = UNAVAILABLE_ATTRIBUTE;
    auto ret = CM_Get_DevNode_Status(&pulStatus, &pulProblemNumber, devInfo.DevInst, 0);
    if (ret == CR_SUCCESS)
    {
        NodeStatus = std::to_string(pulStatus);
        NodeProblemNumber = std::to_string(pulProblemNumber);
    }
    addToMap(devAttrMap, NodeStatus);
    addToMap(devAttrMap, NodeProblemNumber);


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

        std::string DriverProviderName = driverInfoData.ProviderName;
        addToMap(devAttrMap, DriverProviderName);

        std::string DriverType = std::to_string(driverInfoData.DriverType);
        addToMap(devAttrMap, DriverType);
    }

    // Try to get SCSI port info
    for (auto &scsiPortToDeviceId : scsiPortToDeviceIdMap)
    {
        if (toUpper(scsiPortToDeviceId.second) == toUpper(DeviceId))
        {
            std::string ScsiAdapterPath = "\\\\.\\SCSI" + std::to_string(scsiPortToDeviceId.first) + ":";
            addToMap(devAttrMap, ScsiAdapterPath);
        }
    }

    // Try to save the SP_DEVINFO_DATA... a bit sketchy but works!
    std::string __devInfoDataString(sizeof(SP_DEVINFO_DATA), '0');
    memcpy((void*)__devInfoDataString.c_str(), &devInfo, sizeof(SP_DEVINFO_DATA));
    addToMap(devAttrMap, __devInfoDataString);

    return devAttrMap;
}

std::vector<AttributeMap> getInterfaceAttributeMap(GUID classGuid)
{
    std::vector<AttributeMap> interfaces;

    if (classGuid == GUID_NULL)
    {
        for (auto &guid : ALL_GUIDS)
        {
            std::vector<AttributeMap> devAttrMap = getInterfaceAttributeMap(guid);
            interfaces.insert(std::end(interfaces), std::begin(devAttrMap), std::end(devAttrMap));
        }

        return interfaces;
    }

    HDEVINFO interfaceDevs = getInterfaceHDevInfo(classGuid);

    if (interfaceDevs == INVALID_HANDLE_VALUE)
    {
        return std::vector<AttributeMap>();
    }

    SP_DEVINFO_DATA devInfo;
    devInfo.cbSize = sizeof(SP_DEVINFO_DATA);

    SP_INTERFACE_DEVICE_DATA interfaceInfo;
    interfaceInfo.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);

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

        std::map<int, std::string> scsiPortToDeviceIdMap = getScsiPortToDeviceIdMap();

        if (SetupDiGetDeviceInterfaceDetail(interfaceDevs, &interfaceInfo, interfaceDetail, size, NULL, &devInfo))
        {
            AttributeMap devAttrMap = getDeviceAttributeMap(interfaceDevs, devInfo, scsiPortToDeviceIdMap);
            std::string DevicePath = interfaceDetail->DevicePath;
            addToMap(devAttrMap, DevicePath);

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
                STORAGE_DEVICE_NUMBER storageDeviceNumber = { 0 };
                DWORD bytesReturned;
                if (DeviceIoControl(handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &storageDeviceNumber, sizeof(STORAGE_DEVICE_NUMBER), &bytesReturned, NULL))
                {
                    std::string PhysicalDrivePath = "\\\\.\\PHYSICALDRIVE" + std::to_string(storageDeviceNumber.DeviceNumber);
                    addToMap(devAttrMap, PhysicalDrivePath);

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

                        std::string ScsiPathId = std::to_string(scsiAddress.PathId);
                        addToMap(devAttrMap, ScsiPathId);

                        std::string ScsiLun = std::to_string(scsiAddress.Lun);
                        addToMap(devAttrMap, ScsiLun);

                        std::string ScsiTargetId = std::to_string(scsiAddress.TargetId);
                        addToMap(devAttrMap, ScsiTargetId);

                        std::string ScsiPortNumber = std::to_string(scsiAddress.PortNumber);
                        addToMap(devAttrMap, ScsiPortNumber);
                    }
                }

                CloseHandle(handle);
            }
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

    std::vector<AttributeMap> completeDevicesAttrMap;

    std::map<int, std::string> scsiPortToDeviceIdMap = getScsiPortToDeviceIdMap();

    if (deviceDevs == INVALID_HANDLE_VALUE)
    {
        goto exit;
    }

    {
        completeDevicesAttrMap = getInterfaceAttributeMap(GUID_NULL);
        for (int devIndex = 0; SetupDiEnumDeviceInfo(deviceDevs, devIndex, &devInfo); devIndex++)
        {
            AttributeMap devAttrMap = getDeviceAttributeMap(deviceDevs, devInfo, scsiPortToDeviceIdMap);

            bool addToAttrMap = true;

            for (auto &i : completeDevicesAttrMap)
            {
                if (i["HardwareId"] == devAttrMap["HardwareId"] && i["CompatibleIds"] == devAttrMap["CompatibleIds"] && i["BusTypeGuid"] == devAttrMap["BusTypeGuid"] &&
                    i["LocationPaths"] == devAttrMap["LocationPaths"] && i["PhysicalDeviceObjectName"] == devAttrMap["PhysicalDeviceObjectName"] &&
                    i["LocationInformation"] == devAttrMap["LocationInformation"])
                {
                    addToAttrMap = false;
                    break;
                }
            }

            if (addToAttrMap)
            {
                completeDevicesAttrMap.push_back(devAttrMap);
            }
        }
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
    for (auto &i : attrMap)
    {
        // Hide 'private' attributes
        if (!startsWith("__", i.first, true))
        {
            std::string value = i.second;
            value = std::regex_replace(value, std::regex("\n"), "\n                           ");
            value = std::regex_replace(value, std::regex("\n\n"), "\n");

            printf("%-25s: %s\n", i.first.c_str(), value.c_str());
        }
    }
    std::cout << "*******************************************************" << std::endl;
    std::cout << "*******************************************************" << std::endl;
}

AttributeMap getAttributeMapWith(std::string key, std::string value)
{
    std::vector<AttributeMap> devicesAttributeMap = getAllDevicesAttributeMap();
    for (auto &deviceAttrMap : devicesAttributeMap)
    {
        if (deviceAttrMap.find(key) != deviceAttrMap.end())
        {
            if (toUpper(rTrim(deviceAttrMap[key])) == toUpper(rTrim(value)))
            {
                return deviceAttrMap;
            }
        }
    }
    return AttributeMap();
}

DEVINST getDevInstWith(std::string &key, std::string &value)
{
    AttributeMap deviceAttrMap = getAttributeMapWith(key, value);
    if (!deviceAttrMap.empty())
    {
        PSP_DEVINFO_DATA pDevInfoData = (PSP_DEVINFO_DATA)deviceAttrMap["__devInfoDataString"].c_str();
        return pDevInfoData->DevInst;
    }
    return NULL;
}

std::map<int, std::string> getScsiPortToDeviceIdMap()
{
    std::map<int, std::string> scsiPortToDeviceIdMap;
    std::map<int, std::string> scsiPortToDriver;
    std::map < std::string, std::vector < std::string>> driverToDeviceIdsMap;

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
        scsiPortToDeviceIdMap[port] = deviceIds[0];

        deviceIds.erase(deviceIds.begin());
    }

    return scsiPortToDeviceIdMap;
}