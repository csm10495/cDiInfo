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
    std::vector<cdi::attr::AttributeSet> allDevsAttrMap = getAllDevicesAttributeSet();
    for (auto i : allDevsAttrMap)
    {
        printAttributeSet(i);
    }
}

void printVectorOfStrings(std::vector<std::string>& vec, std::string title, bool printStars)
{
    printf("%s\n", title.c_str());
    auto regex = std::regex("\n");
    for (auto i : vec)
    {
        i = std::regex_replace(i, regex, "\n  ");
        printf("%s\n", rTrim(i).c_str());
        if (printStars)
        {
            printf("\n*******************\n");
        }
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
        if (CM_Enumerate_Classes(i, &guid, 0) != CR_SUCCESS)
        {
            break;
        }
        classes.push_back(guidToString(guid));
    }

    return classes;
}

std::vector<cdi::attr::AttributeSet> getAttributeSetsWith(std::string key, std::string value)
{
    std::vector<cdi::attr::AttributeSet> matchingAttributeSets;

    std::vector<cdi::attr::AttributeSet> devicesAttributeSet = getAllDevicesAttributeSet();
    for (auto &deviceAttrSet : devicesAttributeSet)
    {
        auto itr = deviceAttrSet.find(ATTRWITHNAME(key));
        if (itr != deviceAttrSet.end())
        {
            if (SymMatchString(itr->getValue<std::string>().c_str(), value.c_str(), FALSE))
            {
                matchingAttributeSets.push_back(deviceAttrSet);
            }
        }
    }
    return matchingAttributeSets;
}

std::vector<cdi::attr::AttributeSet> getAttributesWith(std::string key, std::string value, std::string alt)
{
    std::vector<cdi::attr::AttributeSet> devicesAttributeSet = getAllDevicesAttributeSet();
    std::vector<cdi::attr::AttributeSet> retAS;

    std::vector<std::string> otherAttributeNames = split(alt, ',');
    // trim each string
    std::for_each(otherAttributeNames.begin(), otherAttributeNames.end(), trim);

    for (cdi::attr::AttributeSet &attributeSet : devicesAttributeSet)
    {
        cdi::attr::AttributeSet inAS;
        
        // make sure this attribute Set has the key... if not continue
        auto itr = attributeSet.find(ATTRWITHNAME(key));
        if (itr != attributeSet.end())
        {
            if (SymMatchString(itr->getValue<std::string>().c_str(), value.c_str(), FALSE))
            {
                inAS.insert(*itr);
            }
            else
            {
                continue;
            }
        }
        else
        {
            continue;
        }

        // go through each attribute, if it matches one of the otherAttributeNames, add it to the inAS
        for (auto attribute : attributeSet)
        {
            for (auto attributeName : otherAttributeNames)
            {
                if (SymMatchString(attribute.getName().c_str(), attributeName.c_str(), FALSE))
                {
                    inAS.insert(attribute);
                }
            }
        }

        retAS.push_back(inAS);
    }

    return retAS;
}

std::vector<cdi::attr::AttributeSet> getAttributeSetsWithout(std::string key, std::string value)
{
    std::vector<cdi::attr::AttributeSet> matchingAttributeSets;

    std::vector<cdi::attr::AttributeSet> devicesAttributeSet = getAllDevicesAttributeSet();
    for (auto &deviceAttrSet : devicesAttributeSet)
    {
        auto itr = deviceAttrSet.find(ATTRWITHNAME(key));
        if (itr != deviceAttrSet.end())
        {
            if (!SymMatchString(itr->getValue<std::string>().c_str(), value.c_str(), FALSE))
            {
                matchingAttributeSets.push_back(deviceAttrSet);
            }
        }
        else
        {
            matchingAttributeSets.push_back(deviceAttrSet);
        }
    }
    return matchingAttributeSets;
}
