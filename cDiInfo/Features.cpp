/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Features.cpp - Features implementation file
*/

// Local includes
#include "Core.h"
#include "Features.h"

// Tries to disable a device with a key/value
STATUS disableDevice(std::string key, std::string value)
{
    DEVINST devInst = getDevInstWith(key, value);

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
        std::cerr << "CM_Disable_DevNode(...) returned " << cmRet << std::endl;
        return CM_FAILED;
    }
}

// Tries to enable a device with a key/value
STATUS enableDevice(std::string key, std::string value)
{
    DEVINST devInst = getDevInstWith(key, value);

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
        std::cerr << "CM_Enable_DevNode(...) returned " << cmRet << std::endl;
        return CM_FAILED;
    }
}

// Prints everything we can get about every device
void printAllInfo()
{
    std::vector<AttributeMap> allDevsAttrMap = getAllDevicesAttributeMap();
    for (auto i : allDevsAttrMap)
    {
        printAttributeMap(i);
    }
}