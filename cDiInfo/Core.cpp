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

        std::string DriverProviderName = driverInfoData.ProviderName;
        addToMap(devAttrMap, DriverProviderName);

        std::string DriverType = std::to_string(driverInfoData.DriverType);
        addToMap(devAttrMap, DriverType);
    }

    // Try to get SCSI port info (this loop is only for getting the SCSI adapter/port for the controller itself, it's disks already have it)
    for (auto &scsiPortToDeviceId : scsiPortToDeviceIdMap)
    {
        if (toUpper(scsiPortToDeviceId.second) == toUpper(DeviceId))
        {
            std::string ScsiPortNumber = std::to_string(scsiPortToDeviceId.first);
            std::string ScsiAdapterPath = "\\\\.\\SCSI" + ScsiPortNumber + ":";
            addToMap(devAttrMap, ScsiAdapterPath);
            addToMap(devAttrMap, ScsiPortNumber);
            break;
        }
    }

    // Try to save the SP_DEVINFO_DATA... a bit sketchy but works!
    std::string __devInfoDataString(sizeof(SP_DEVINFO_DATA), '0');
    memcpy((void*)__devInfoDataString.c_str(), &devInfo, sizeof(SP_DEVINFO_DATA));
    addToMap(devAttrMap, __devInfoDataString);

    // Get DevNode Property Keys
    addOtherDevNodeProperties(devAttrMap, devInfo.DevInst);

    // Get Resource Descriptor Data
    addDeviceConfigurationAndResources(devAttrMap, devInfo.DevInst);

    return devAttrMap;
}

void addOtherDevNodeProperties(AttributeMap &attributeMap, DEVINST &devInst)
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

void addDeviceConfigurationAndResources(AttributeMap &attributeMap, DEVINST &devInst)
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
            return;
        };

        while (true)
        {
            ULONG bufferSize = 0;
            cmRet = CM_Get_Res_Des_Data_Size(&bufferSize, nextLogConf, 0);
            if (cmRet != CR_SUCCESS)
            {
                CM_Free_Res_Des_Handle(nextLogConf);
                break;
            };

            BYTE* buffer = new BYTE[bufferSize];
            cmRet = CM_Get_Res_Des_Data(nextLogConf, buffer, bufferSize, 0);
            if (cmRet != CR_SUCCESS)
            {
                CM_Free_Res_Des_Handle(nextLogConf);
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


            if (CM_Get_Next_Res_Des(&nextLogConf, nextLogConf, ResType_All, &resourceType, 0) != CR_SUCCESS)
            {
                CM_Free_Res_Des_Handle(nextLogConf);
                break;
            }
        }

        auto a = CM_Get_Next_Log_Conf(&nextLogConf, firstLogConf, 0);
        if (a != CR_SUCCESS)
        {
            break;
        }
    }
    CM_Free_Log_Conf_Handle(firstLogConf);
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
        for (auto guid : ALL_GUIDS)
        {
            std::vector<AttributeMap> devAttrMap = getInterfaceAttributeMap(guid);
            interfaces.insert(std::end(interfaces), std::begin(devAttrMap), std::end(devAttrMap));
        }

        return interfaces;
    }

    HDEVINFO interfaceDevs = getInterfaceHDevInfo(classGuid);
    std::map<int, std::string> scsiPortToDeviceIdMap = getScsiPortToDeviceIdMap();

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

        if (SetupDiGetDeviceInterfaceDetail(interfaceDevs, &interfaceInfo, interfaceDetail, size, NULL, &devInfo))
        {
            AttributeMap devAttrMap = getDeviceAttributeMap(interfaceDevs, devInfo, scsiPortToDeviceIdMap);
            std::string DevicePath = interfaceDetail->DevicePath;
            addToMap(devAttrMap, DevicePath);

            char targetPaths[4096];
            // substr(4) to get rid of the \\.\ 
            DWORD targetPathsLength = QueryDosDevice(DevicePath.substr(4).c_str(), targetPaths, 4096);
            if (targetPathsLength != 0)
            {
                std::string MSDosDeviceName = std::string(targetPaths, targetPathsLength);
                MSDosDeviceName = delimitedStringToNewlineString(MSDosDeviceName);
                addToMap(devAttrMap, MSDosDeviceName);
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
                STORAGE_DEVICE_NUMBER storageDeviceNumber = { 0 };
                DWORD bytesReturned;
                if (DeviceIoControl(handle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &storageDeviceNumber, sizeof(STORAGE_DEVICE_NUMBER), &bytesReturned, NULL))
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
                if (DeviceIoControl(handle, IOCTL_STORAGE_GET_HOTPLUG_INFO, NULL, 0, &storageHotplugInfo, sizeof(STORAGE_HOTPLUG_INFO), &bytesReturned, NULL))
                {
                    std::string MediaRemovable = toBoolString(storageHotplugInfo.MediaRemovable);
                    addToMap(devAttrMap, MediaRemovable);

                    std::string MediaHotplug = toBoolString(storageHotplugInfo.MediaHotplug);
                    addToMap(devAttrMap, MediaHotplug);

                    std::string DeviceHotplug = toBoolString(storageHotplugInfo.DeviceHotplug);
                    addToMap(devAttrMap, DeviceHotplug);
                }

                // Get media serial number
                MEDIA_SERIAL_NUMBER_DATA  mediaSerialNumberData = { 0 };
                if (DeviceIoControl(handle, IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER, NULL, 0, &mediaSerialNumberData, sizeof(MEDIA_SERIAL_NUMBER_DATA), &bytesReturned, NULL))
                {
                    std::string MediaSerialNumber = std::string((char*)mediaSerialNumberData.SerialNumberData, mediaSerialNumberData.SerialNumberLength);
                    addToMap(devAttrMap, MediaSerialNumber);
                }

                // Get media serial number
                STORAGE_READ_CAPACITY storageReadCapacity = { 0 };
                if (DeviceIoControl(handle, IOCTL_STORAGE_READ_CAPACITY, NULL, 0, &storageReadCapacity, sizeof(STORAGE_READ_CAPACITY), &bytesReturned, NULL))
                {
                    std::string BlockLength = std::to_string(storageReadCapacity.BlockLength);
                    addToMap(devAttrMap, BlockLength);

                    std::string NumberOfBlocks = std::to_string(storageReadCapacity.NumberOfBlocks.QuadPart);
                    addToMap(devAttrMap, NumberOfBlocks);

                    std::string DiskLength = std::to_string(storageReadCapacity.DiskLength.QuadPart);
                    addToMap(devAttrMap, DiskLength);
                }

                // Get storage unique identifier
                BYTE b[4096] = { 0 };
                PSTORAGE_DEVICE_UNIQUE_IDENTIFIER pStorageDeviceUniqueIdentifer = (PSTORAGE_DEVICE_UNIQUE_IDENTIFIER)b;
                STORAGE_PROPERTY_QUERY storagePropertyQuery;
                memset(&storagePropertyQuery, 0, sizeof(STORAGE_PROPERTY_QUERY));
                storagePropertyQuery.PropertyId = StorageDeviceUniqueIdProperty;
                storagePropertyQuery.QueryType = PropertyStandardQuery;
                if (DeviceIoControl(handle, IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY), &b, 4096, &bytesReturned, NULL))
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
            std::string deviceId = getDeviceId(devInfo.DevInst);

            bool addToAttrMap = true;

            for (auto &i : completeDevicesAttrMap)
            {
                if (i["DeviceId"] == deviceId)
                {
                    addToAttrMap = false;
                    break;
                }
            }

            if (addToAttrMap)
            {
                AttributeMap& devAttrMap = getDeviceAttributeMap(deviceDevs, devInfo, scsiPortToDeviceIdMap);
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

            std::string spaces = "                           ";
            while (spaces.size() < i.first.size() + 2)
            {
                spaces += " ";
            }

            value = std::regex_replace(value, std::regex("\n"), "\n" + spaces);
            value = std::regex_replace(value, std::regex("\n\n"), "\n");

            printf("%-25s: %s\n", i.first.c_str(), value.c_str());
        }
    }
    std::cout << "*******************************************************" << std::endl;
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

std::vector<AttributeMap> getAttributeMapsWith(std::string key, std::string value)
{
    std::vector<AttributeMap> matchingAttributeMaps;

    std::vector<AttributeMap> devicesAttributeMap = getAllDevicesAttributeMap();
    for (auto &deviceAttrMap : devicesAttributeMap)
    {
        auto itr = deviceAttrMap.find(key);
        if (itr != deviceAttrMap.end())
        {
            if (SymMatchString(itr->second.c_str(), value.c_str(), FALSE))
            {
                matchingAttributeMaps.push_back(deviceAttrMap);
            }
        }
    }
    return matchingAttributeMaps;
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