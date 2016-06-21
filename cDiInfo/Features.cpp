/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Features.cpp - Features implementation file
*/

// Local includes
#include "Core.h"
#include "Features.h"

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
        std::cerr << "CM_Disable_DevNode(...) returned " << cmRet << " (" << cmRetValueToString(cmRet) << ")" << std::endl;
        return CM_FAILED;
    }
}

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

std::string cmRetValueToString(ULONG value)
{
    if (value == 0)
    {
        return "CR_SUCCESS";
    }
    else if (value == 1)
    {
        return "CR_DEFAULT";
    }
    else if (value == 2)
    {
        return "CR_OUT_OF_MEMORY";
    }
    else if (value == 3)
    {
        return "CR_INVALID_POINTER";
    }
    else if (value == 4)
    {
        return "CR_INVALID_FLAG";
    }
    else if (value == 5)
    {
        return "CR_INVALID_DEVINST";
    }
    else if (value == 6)
    {
        return "CR_INVALID_RES_DES";
    }
    else if (value == 7)
    {
        return "CR_INVALID_LOG_CONF";
    }
    else if (value == 8)
    {
        return "CR_INVALID_ARBITRATOR";
    }
    else if (value == 9)
    {
        return "CR_INVALID_NODELIST";
    }
    else if (value == 10)
    {
        return "CR_DEVNODE_HAS_REQS";
    }
    else if (value == 11)
    {
        return "CR_INVALID_RESOURCEID";
    }
    else if (value == 12)
    {
        return "CR_DLVXD_NOT_FOUND";
    }
    else if (value == 13)
    {
        return "CR_NO_SUCH_DEVINST";
    }
    else if (value == 13)
    {
        return "CR_NO_SUCH_DEVNODE";
    }
    else if (value == 14)
    {
        return "CR_NO_MORE_LOG_CONF";
    }
    else if (value == 15)
    {
        return "CR_NO_MORE_RES_DES";
    }
    else if (value == 16)
    {
        return "CR_ALREADY_SUCH_DEVNODE";
    }
    else if (value == 17)
    {
        return "CR_INVALID_RANGE_LIST";
    }
    else if (value == 18)
    {
        return "CR_INVALID_RANGE";
    }
    else if (value == 19)
    {
        return "CR_FAILURE";
    }
    else if (value == 20)
    {
        return "CR_NO_SUCH_LOGICAL_DEV";
    }
    else if (value == 21)
    {
        return "CR_CREATE_BLOCKED";
    }
    else if (value == 22)
    {
        return "CR_NOT_SYSTEM_VM";
    }
    else if (value == 23)
    {
        return "CR_REMOVE_VETOED";
    }
    else if (value == 24)
    {
        return "CR_APM_VETOED";
    }
    else if (value == 25)
    {
        return "CR_INVALID_LOAD_TYPE";
    }
    else if (value == 26)
    {
        return "CR_BUFFER_SMALL";
    }
    else if (value == 27)
    {
        return "CR_NO_ARBITRATOR";
    }
    else if (value == 28)
    {
        return "CR_NO_REGISTRY_HANDLE";
    }
    else if (value == 29)
    {
        return "CR_REGISTRY_ERROR";
    }
    else if (value == 30)
    {
        return "CR_INVALID_DEVICE_ID";
    }
    else if (value == 31)
    {
        return "CR_INVALID_DATA";
    }
    else if (value == 32)
    {
        return "CR_INVALID_API";
    }
    else if (value == 33)
    {
        return "CR_DEVLOADER_NOT_READY";
    }
    else if (value == 34)
    {
        return "CR_NEED_RESTART";
    }
    else if (value == 35)
    {
        return "CR_NO_MORE_HW_PROFILES";
    }
    else if (value == 36)
    {
        return "CR_DEVICE_NOT_THERE";
    }
    else if (value == 37)
    {
        return "CR_NO_SUCH_VALUE";
    }
    else if (value == 38)
    {
        return "CR_WRONG_TYPE";
    }
    else if (value == 39)
    {
        return "CR_INVALID_PRIORITY";
    }
    else if (value == 40)
    {
        return "CR_NOT_DISABLEABLE";
    }
    else if (value == 41)
    {
        return "CR_FREE_RESOURCES";
    }
    else if (value == 42)
    {
        return "CR_QUERY_VETOED";
    }
    else if (value == 43)
    {
        return "CR_CANT_SHARE_IRQ";
    }
    else if (value == 44)
    {
        return "CR_NO_DEPENDENT";
    }
    else if (value == 45)
    {
        return "CR_SAME_RESOURCES";
    }
    else if (value == 46)
    {
        return "CR_NO_SUCH_REGISTRY_KEY";
    }
    else if (value == 47)
    {
        return "CR_INVALID_MACHINENAME";
    }
    else if (value == 48)
    {
        return "CR_REMOTE_COMM_FAILURE";
    }
    else if (value == 49)
    {
        return "CR_MACHINE_UNAVAILABLE";
    }
    else if (value == 50)
    {
        return "CR_NO_CM_SERVICES";
    }
    else if (value == 51)
    {
        return "CR_ACCESS_DENIED";
    }
    else if (value == 52)
    {
        return "CR_CALL_NOT_IMPLEMENTED";
    }
    else if (value == 53)
    {
        return "CR_INVALID_PROPERTY";
    }
    else if (value == 54)
    {
        return "CR_DEVICE_INTERFACE_ACTIVE";
    }
    else if (value == 55)
    {
        return "CR_NO_SUCH_DEVICE_INTERFACE";
    }
    else if (value == 56)
    {
        return "CR_INVALID_REFERENCE_STRING";
    }
    else if (value == 57)
    {
        return "CR_INVALID_CONFLICT_LIST";
    }
    else if (value == 58)
    {
        return "CR_INVALID_INDEX";
    }
    else if (value == 59)
    {
        return "CR_INVALID_STRUCTURE_SIZE";
    }
    else if (value == 60)
    {
        return "NUM_CR_RESULTS";
    }

    return "Unkown CR Return Code";
}
