/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Features.cpp - Features implementation file
*/

// Local includes
#include "Features.h"

std::vector<std::string> sampleAttributeKeys = {
    "DeviceId",
    "DeviceDesc",
    "FriendlyName",
    "HardwareId",
    "Driver",
    "Manufacturer",
    "BusNumber",
    "ChildrenDeviceIds",
    "ParentDeviceId",
    "SiblingDeviceIds",
    "DriverDescription",
    "DriverMfgName",
    "ScsiAdapterPath",
    "ScsiPortNumber",
    "ScsiPathId",
    "ScsiLun",
    "ScsiTargetId",
    "DevicePath",
    "PhysicalDrivePath",
    "MaximumTransferLength",
    "DeviceHotplug",
    "DiskLength",
    "VendorId",
    "ProductRevision",
    "SerialNumber",
};

STATUS disableDevice(DEVINST devInst)
{
    if (devInst == NULL)
    {
        return FAILED_TO_GET_MATCHING_DEVINST;
    }

    DWORD cmRet = CM_Disable_DevNode(devInst, CM_DISABLE_UI_NOT_OK | CM_DISABLE_ABSOLUTE);

    if (cmRet == CR_SUCCESS)
    {
        return SUCCESS;
    }
    else
    {
        std::cerr << "CM_Disable_DevNode(...) returned " << cmRet << " (" << cmRetValueToString(cmRet) << ")" << std::endl;
        return CM_FAILED;
    }
}

STATUS enableDevice(DEVINST devInst)
{
    if (devInst == NULL)
    {
        return FAILED_TO_GET_MATCHING_DEVINST;
    }

    DWORD cmRet = CM_Enable_DevNode(devInst, 0);

    if (cmRet == CR_SUCCESS)
    {
        return SUCCESS;
    }
    else
    {
        std::cerr << "CM_Enable_DevNode(...) returned " << cmRet << " (" << cmRetValueToString(cmRet) << ")" << std::endl;
        return CM_FAILED;
    }
}

void printAllInfo()
{
    std::vector<AttributeMap> allDevsAttrMap = getAllDevicesAttributeMap();
    for (auto i : allDevsAttrMap)
    {
        printAttributeMap(i);
    }
}

void printVectorOfStrings(std::vector<std::string>& vec, std::string title)
{
    printf("%s\n", title.c_str());
    auto regex = std::regex("\n");
    for (auto i : vec)
    {
        i = std::regex_replace(i, regex, "\n  ");
        printf("  %s\n", rTrim(i).c_str());
    }
}

std::vector<std::string> getSampleAttributeKeys()
{
    return sampleAttributeKeys;
}

std::vector<std::string> getEnumerators()
{
    std::vector<std::string> enumerators;

    for (ULONG i = 0; ;i++)
    {
        char buf[MAX_DEVICE_ID_LEN] = { '\0' };
        ULONG bufLen = MAX_DEVICE_ID_LEN;
        if (CM_Enumerate_Enumerators(i, buf, &bufLen, 0) != CR_SUCCESS)
        {
            break;
        }
        enumerators.push_back(std::string(buf, bufLen));
    }

    return enumerators;
}

std::vector<std::string> getClasses()
{
    std::vector<std::string> classes;

    for (ULONG i = 0; ;i++)
    {
        GUID guid;
        if (CM_Enumerate_Classes(i, &guid,  0) != CR_SUCCESS)
        {
            break;
        }
        classes.push_back(guidToString(guid));
    }

    return classes;
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

std::vector<std::string> getAttributesWith(std::string key, std::string value)
{
    std::vector<AttributeMap> &attributeMapsWith = getAttributeMapsWith(key, value);
    
    std::vector<std::string> retVec;

    for (auto &i : attributeMapsWith)
    {
        // The key/value has to be there at this point
        retVec.push_back(i[key]);
    }

    return retVec;
}

std::vector<AttributeMap> getAttributeMapsWithout(std::string key, std::string value)
{
    std::vector<AttributeMap> matchingAttributeMaps;

    std::vector<AttributeMap> devicesAttributeMap = getAllDevicesAttributeMap();
    for (auto &deviceAttrMap : devicesAttributeMap)
    {
        auto itr = deviceAttrMap.find(key);
        if (itr != deviceAttrMap.end())
        {
            if (!SymMatchString(itr->second.c_str(), value.c_str(), FALSE))
            {
                matchingAttributeMaps.push_back(deviceAttrMap);
            }
        }
        else
        {
            matchingAttributeMaps.push_back(deviceAttrMap);
        }
    }
    return matchingAttributeMaps;
}
