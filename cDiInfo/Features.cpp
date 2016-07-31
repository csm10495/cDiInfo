/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Features.cpp - Features implementation file
*/

// Local includes
#include "Features.h"

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
    std::cout << title << std::endl;
    for (auto &i : vec)
    {
        std::cout << "  " << i << std::endl;
    }
}

std::vector<std::string> getSampleAttributeKeys()
{
    std::vector<std::string> retKeys;

    HDEVINFO devs = { 0 };
    SP_DEVINFO_DATA devInfo = { 0 };

    AttributeMap sampleAttributeMap = getDeviceAttributeMap(devs, devInfo, std::map<int, std::string>());

    for (auto &i : sampleAttributeMap)
    {
        if (i.first != DEVINFO_DATA_STRING)
        {
            retKeys.push_back(i.first);
        }
    }

    return retKeys;
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
