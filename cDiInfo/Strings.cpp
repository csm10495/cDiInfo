/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Strings.cpp - Implementation file for string manipulation functions
*/

// Local includes
#include "Strings.h"

std::string trim(std::string &s)
{
    return lTrim(rTrim(s));
}

std::string& lTrim(std::string& s)
{
    s.erase(0, s.find_first_not_of(TRIM_CHARS));

    while (s.size() > 0 && s.front() == '\0')
    {
        s.erase(0);
    }

    return s;

}

std::string &rTrim(std::string & s)
{
    s.erase(s.find_last_not_of(TRIM_CHARS) + 1);

    while (s.size() > 0 && s.back() == '\0')
    {
        s.pop_back();
    }

    return s;
}

std::string &toUpper(std::string &s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);

    return s;
}

bool startsWith(std::string needle, std::string haystack, bool ignoreCase)
{
    if (ignoreCase)
    {
        toUpper(needle);
        toUpper(haystack);
    }

    if (needle.size() <= haystack.size())
    {
        for (size_t i = 0; i < needle.size(); i++)
        {
            if (needle[i] != haystack[i])
            {
                return false;
            }
        }
        return true;
    }

    return false;
}

std::string guidToString(GUID guid)
{
    wchar_t guidCharArr[4096] = { 0 };
    StringFromGUID2(guid, guidCharArr, 4096);
    std::wstring asWStr = std::wstring(guidCharArr);
    return std::string(asWStr.begin(), asWStr.end());
}

std::string cmProbToString(unsigned long problemNumber)
{
    if (problemNumber == 1)
    {
        return "CM_PROB_NOT_CONFIGURED";
    }
    if (problemNumber == 2)
    {
        return "CM_PROB_DEVLOADER_FAILED";
    }
    if (problemNumber == 3)
    {
        return "CM_PROB_OUT_OF_MEMORY";
    }
    if (problemNumber == 4)
    {
        return "CM_PROB_ENTRY_IS_WRONG_TYPE";
    }
    if (problemNumber == 5)
    {
        return "CM_PROB_LACKED_ARBITRATOR";
    }
    if (problemNumber == 6)
    {
        return "CM_PROB_BOOT_CONFIG_CONFLICT";
    }
    if (problemNumber == 7)
    {
        return "CM_PROB_FAILED_FILTER";
    }
    if (problemNumber == 8)
    {
        return "CM_PROB_DEVLOADER_NOT_FOUND";
    }
    if (problemNumber == 9)
    {
        return "CM_PROB_INVALID_DATA";
    }
    if (problemNumber == 10)
    {
        return "CM_PROB_FAILED_START";
    }
    if (problemNumber == 11)
    {
        return "CM_PROB_LIAR";
    }
    if (problemNumber == 12)
    {
        return "CM_PROB_NORMAL_CONFLICT";
    }
    if (problemNumber == 13)
    {
        return "CM_PROB_NOT_VERIFIED";
    }
    if (problemNumber == 14)
    {
        return "CM_PROB_NEED_RESTART";
    }
    if (problemNumber == 15)
    {
        return "CM_PROB_REENUMERATION";
    }
    if (problemNumber == 16)
    {
        return "CM_PROB_PARTIAL_LOG_CONF";
    }
    if (problemNumber == 17)
    {
        return "CM_PROB_UNKNOWN_RESOURCE";
    }
    if (problemNumber == 18)
    {
        return "CM_PROB_REINSTALL";
    }
    if (problemNumber == 19)
    {
        return "CM_PROB_REGISTRY";
    }
    if (problemNumber == 20)
    {
        return "CM_PROB_VXDLDR";
    }
    if (problemNumber == 21)
    {
        return "CM_PROB_WILL_BE_REMOVED";
    }
    if (problemNumber == 22)
    {
        return "CM_PROB_DISABLED";
    }
    if (problemNumber == 23)
    {
        return "CM_PROB_DEVLOADER_NOT_READY";
    }
    if (problemNumber == 24)
    {
        return "CM_PROB_DEVICE_NOT_THERE";
    }
    if (problemNumber == 25)
    {
        return "CM_PROB_MOVED";
    }
    if (problemNumber == 26)
    {
        return "CM_PROB_TOO_EARLY";
    }
    if (problemNumber == 27)
    {
        return "CM_PROB_NO_VALID_LOG_CONF";
    }
    if (problemNumber == 28)
    {
        return "CM_PROB_FAILED_INSTALL";
    }
    if (problemNumber == 29)
    {
        return "CM_PROB_HARDWARE_DISABLED";
    }
    if (problemNumber == 30)
    {
        return "CM_PROB_CANT_SHARE_IRQ";
    }
    if (problemNumber == 31)
    {
        return "CM_PROB_FAILED_ADD";
    }
    if (problemNumber == 32)
    {
        return "CM_PROB_DISABLED_SERVICE";
    }
    if (problemNumber == 33)
    {
        return "CM_PROB_TRANSLATION_FAILED";
    }
    if (problemNumber == 34)
    {
        return "CM_PROB_NO_SOFTCONFIG";
    }
    if (problemNumber == 35)
    {
        return "CM_PROB_BIOS_TABLE";
    }
    if (problemNumber == 36)
    {
        return "CM_PROB_IRQ_TRANSLATION_FAILED";
    }
    if (problemNumber == 37)
    {
        return "CM_PROB_FAILED_DRIVER_ENTRY";
    }
    if (problemNumber == 38)
    {
        return "CM_PROB_DRIVER_FAILED_PRIOR_UNLOAD";
    }
    if (problemNumber == 39)
    {
        return "CM_PROB_DRIVER_FAILED_LOAD";
    }
    if (problemNumber == 40)
    {
        return "CM_PROB_DRIVER_SERVICE_KEY_INVALID";
    }
    if (problemNumber == 41)
    {
        return "CM_PROB_LEGACY_SERVICE_NO_DEVICES";
    }
    if (problemNumber == 42)
    {
        return "CM_PROB_DUPLICATE_DEVICE";
    }
    if (problemNumber == 43)
    {
        return "CM_PROB_FAILED_POST_START";
    }
    if (problemNumber == 44)
    {
        return "CM_PROB_HALTED";
    }
    if (problemNumber == 45)
    {
        return "CM_PROB_PHANTOM";
    }
    if (problemNumber == 46)
    {
        return "CM_PROB_SYSTEM_SHUTDOWN";
    }
    if (problemNumber == 47)
    {
        return "CM_PROB_HELD_FOR_EJECT";
    }
    if (problemNumber == 48)
    {
        return "CM_PROB_DRIVER_BLOCKED";
    }
    if (problemNumber == 49)
    {
        return "CM_PROB_REGISTRY_TOO_LARGE";
    }
    if (problemNumber == 50)
    {
        return "CM_PROB_SETPROPERTIES_FAILED";
    }
    if (problemNumber == 51)
    {
        return "CM_PROB_WAITING_ON_DEPENDENCY";
    }
    if (problemNumber == 52)
    {
        return "CM_PROB_UNSIGNED_DRIVER";
    }
    if (problemNumber == 53)
    {
        return "CM_PROB_USED_BY_DEBUGGER";
    }

    return "Unknown CM_Prob Code";
}

std::string nodeStatusToString(unsigned long status)
{
    std::string retString = "";

    if (status & 1)
    {
        retString += "DN_ROOT_ENUMERATED, ";
    }
    if (status & 2)
    {
        retString += "DN_DRIVER_LOADED, ";
    }
    if (status & 4)
    {
        retString += "DN_ENUM_LOADED, ";
    }
    if (status & 8)
    {
        retString += "DN_STARTED, ";
    }
    if (status & 16)
    {
        retString += "DN_MANUAL, ";
    }
    if (status & 32)
    {
        retString += "DN_NEED_TO_ENUM, ";
    }
    if (status & 64)
    {
        retString += "DN_DRIVER_BLOCKED, ";
    }
    if (status & 64)
    {
        retString += "DN_NOT_FIRST_TIME, ";
    }
    if (status & 128)
    {
        retString += "DN_HARDWARE_ENUM, ";
    }
    if (status & 256)
    {
        retString += "DN_LIAR, ";
    }
    if (status & 256)
    {
        retString += "DN_NEED_RESTART, ";
    }
    if (status & 512)
    {
        retString += "DN_HAS_MARK, ";
    }
    if (status & 512)
    {
        retString += "DN_CHILD_WITH_INVALID_ID, ";
    }
    if (status & 1024)
    {
        retString += "DN_HAS_PROBLEM, ";
    }
    if (status & 2048)
    {
        retString += "DN_FILTERED, ";
    }
    if (status & 4096)
    {
        retString += "DN_MOVED, ";
    }
    if (status & 4096)
    {
        retString += "DN_LEGACY_DRIVER, ";
    }
    if (status & 8192)
    {
        retString += "DN_DISABLEABLE, ";
    }
    if (status & 16384)
    {
        retString += "DN_REMOVABLE, ";
    }
    if (status & 32768)
    {
        retString += "DN_PRIVATE_PROBLEM, ";
    }
    if (status & 65536)
    {
        retString += "DN_MF_PARENT, ";
    }
    if (status & 131072)
    {
        retString += "DN_MF_CHILD, ";
    }
    if (status & 262144)
    {
        retString += "DN_WILL_BE_REMOVED, ";
    }
    if (status & 524288)
    {
        retString += "DN_NOT_FIRST_TIMEE, ";
    }
    if (status & 1048576)
    {
        retString += "DN_STOP_FREE_RES, ";
    }
    if (status & 2097152)
    {
        retString += "DN_REBAL_CANDIDATE, ";
    }
    if (status & 4194304)
    {
        retString += "DN_BAD_PARTIAL, ";
    }
    if (status & 8388608)
    {
        retString += "DN_NT_ENUMERATOR, ";
    }
    if (status & 16777216)
    {
        retString += "DN_NT_DRIVER, ";
    }
    if (status & 33554432)
    {
        retString += "DN_NEEDS_LOCKING, ";
    }
    if (status & 33554432)
    {
        retString += "DN_DEVICE_DISCONNECTED, ";
    }
    if (status & 67108864)
    {
        retString += "DN_ARM_WAKEUP, ";
    }
    if (status & 134217728)
    {
        retString += "DN_APM_ENUMERATOR, ";
    }
    if (status & 268435456)
    {
        retString += "DN_APM_DRIVER, ";
    }
    if (status & 536870912)
    {
        retString += "DN_SILENT_INSTALL, ";
    }
    if (status & 1073741824)
    {
        retString += "DN_NO_SHOW_IN_DM, ";
    }
    if (status & 2147483648)
    {
        retString += "DN_BOOT_LOG_PROB, ";
    }
    return retString.erase(retString.size() - 2, 2);
}

std::string cmRetValueToString(unsigned long value)
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

std::string windowsErrorToString(uint32_t errorCode)
{
    char* lpMsgBuf;
    DWORD bufLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    if (bufLen)
    {
        std::string result((char*)lpMsgBuf, bufLen);
        LocalFree(lpMsgBuf);
        return result;
    }
    else
    {
        return "";
    }
}

std::string wStringToString(std::wstring wStr)
{
    return std::string(wStr.begin(), wStr.end());
}

std::wstring stringToWString(std::string str)
{
    return std::wstring(str.begin(), str.end());
}

std::string propertyKeyToString(DEVPROPKEY propertyKey)
{
    if (IsEqualDevPropKey(DEVPKEY_Device_DeviceDesc, propertyKey))
    {
        return "Device_DeviceDesc";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_HardwareIds, propertyKey))
    {
        return "Device_HardwareId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_CompatibleIds, propertyKey))
    {
        return "Device_CompatibleIds";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Service, propertyKey))
    {
        return "Device_Service";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Class, propertyKey))
    {
        return "Device_Class";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ClassGuid, propertyKey))
    {
        return "Device_ClassGuid";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Driver, propertyKey))
    {
        return "Device_Driver";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ConfigFlags, propertyKey))
    {
        return "Device_ConfigFlags";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Manufacturer, propertyKey))
    {
        return "Device_Manufacturer";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_FriendlyName, propertyKey))
    {
        return "Device_FriendlyName";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_LocationInfo, propertyKey))
    {
        return "Device_LocationInformation";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_PDOName, propertyKey))
    {
        return "Device_PhysicalDeviceObjectName";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Capabilities, propertyKey))
    {
        return "Device_Capabilities";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_UINumber, propertyKey))
    {
        return "Device_UiNumber";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_UpperFilters, propertyKey))
    {
        return "Device_UpperFilters";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_LowerFilters, propertyKey))
    {
        return "Device_LowerFilters";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_BusTypeGuid, propertyKey))
    {
        return "Device_BusTypeGuid";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_LegacyBusType, propertyKey))
    {
        return "Device_LegacyBusType";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_BusNumber, propertyKey))
    {
        return "Device_BusNumber";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_EnumeratorName, propertyKey))
    {
        return "Device_EnumeratorName";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Security, propertyKey))
    {
        return "Device_Security";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_SecuritySDS, propertyKey))
    {
        return "Device_SecuritySDS";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DevType, propertyKey))
    {
        return "Device_DevType";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Exclusive, propertyKey))
    {
        return "Device_Exclusive";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Characteristics, propertyKey))
    {
        return "Device_Characteristics";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Address, propertyKey))
    {
        return "Device_Address";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_UINumberDescFormat, propertyKey))
    {
        return "Device_UiNumberDescFormat";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_PowerData, propertyKey))
    {
        return "Device_PowerData";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_RemovalPolicy, propertyKey))
    {
        return "Device_RemovalPolicy";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_RemovalPolicyDefault, propertyKey))
    {
        return "Device_RemovalPolicyHwDefault";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_RemovalPolicyOverride, propertyKey))
    {
        return "Device_RemovalPolicyOverride";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_InstallState, propertyKey))
    {
        return "Device_InstallState";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_LocationPaths, propertyKey))
    {
        return "Device_LocationPaths";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_BaseContainerId, propertyKey))
    {
        return "Device_BaseContainerId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_InstanceId, propertyKey))
    {
        return "Device_DeviceId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DevNodeStatus, propertyKey))
    {
        return "Device_DevNodeStatus";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ProblemCode, propertyKey))
    {
        return "Device_ProblemCode";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_EjectionRelations, propertyKey))
    {
        return "Device_EjectionRelations";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_RemovalRelations, propertyKey))
    {
        return "Device_RemovalRelations";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_PowerRelations, propertyKey))
    {
        return "Device_PowerRelations";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_BusRelations, propertyKey))
    {
        return "Device_BusRelations";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Parent, propertyKey))
    {
        return "Device_ParentDeviceId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Children, propertyKey))
    {
        return "Device_ChildrenDeviceIds";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Siblings, propertyKey))
    {
        return "Device_Siblings";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_TransportRelations, propertyKey))
    {
        return "Device_TransportRelations";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ProblemStatus, propertyKey))
    {
        return "Device_ProblemStatus";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Reported, propertyKey))
    {
        return "Device_Reported";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Legacy, propertyKey))
    {
        return "Device_Legacy";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ContainerId, propertyKey))
    {
        return "Device_ContainerId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_InLocalMachineContainer, propertyKey))
    {
        return "Device_InLocalMachineContainer";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Model, propertyKey))
    {
        return "Device_Model";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ModelId, propertyKey))
    {
        return "Device_ModelId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_FriendlyNameAttributes, propertyKey))
    {
        return "Device_FriendlyNameAttributes";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ManufacturerAttributes, propertyKey))
    {
        return "Device_ManufacturerAttributes";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_PresenceNotForDevice, propertyKey))
    {
        return "Device_PresenceNotForDevice";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_SignalStrength, propertyKey))
    {
        return "Device_SignalStrength";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_IsAssociateableByUserAction, propertyKey))
    {
        return "Device_IsAssociateableByUserAction";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ShowInUninstallUI, propertyKey))
    {
        return "Device_ShowInUninstallUI";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Numa_Proximity_Domain, propertyKey))
    {
        return "Device_Numa_Proximity_Domain";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DHP_Rebalance_Policy, propertyKey))
    {
        return "Device_DHP_Rebalance_Policy";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Numa_Node, propertyKey))
    {
        return "Device_Numa_Node";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_BusReportedDeviceDesc, propertyKey))
    {
        return "Device_BusReportedDeviceDesc";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_IsPresent, propertyKey))
    {
        return "Device_IsPresent";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_HasProblem, propertyKey))
    {
        return "Device_HasProblem";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ConfigurationId, propertyKey))
    {
        return "Device_ConfigurationId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ReportedDeviceIdsHash, propertyKey))
    {
        return "Device_ReportedDeviceIdsHash";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_PhysicalDeviceLocation, propertyKey))
    {
        return "Device_PhysicalDeviceLocation";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_BiosDeviceName, propertyKey))
    {
        return "Device_BiosDeviceName";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverProblemDesc, propertyKey))
    {
        return "Device_DriverProblemDesc";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DebuggerSafe, propertyKey))
    {
        return "Device_DebuggerSafe";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_PostInstallInProgress, propertyKey))
    {
        return "Device_PostInstallInProgress";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_Stack, propertyKey))
    {
        return "Device_Stack";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ExtendedConfigurationIds, propertyKey))
    {
        return "Device_ExtendedConfigurationIds";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_IsRebootRequired, propertyKey))
    {
        return "Device_IsRebootRequired";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_FirmwareDate, propertyKey))
    {
        return "Device_FirmwareDate";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_FirmwareVersion, propertyKey))
    {
        return "Device_FirmwareVersion";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_FirmwareRevision, propertyKey))
    {
        return "Device_FirmwareRevision";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DependencyProviders, propertyKey))
    {
        return "Device_DependencyProviders";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DependencyDependents, propertyKey))
    {
        return "Device_DependencyDependents";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_SoftRestartSupported, propertyKey))
    {
        return "Device_SoftRestartSupported";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_SessionId, propertyKey))
    {
        return "Device_SessionId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_InstallDate, propertyKey))
    {
        return "Device_InstallDate";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_FirstInstallDate, propertyKey))
    {
        return "Device_FirstInstallDate";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_LastArrivalDate, propertyKey))
    {
        return "Device_LastArrivalDate";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_LastRemovalDate, propertyKey))
    {
        return "Device_LastRemovalDate";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverDate, propertyKey))
    {
        return "Device_DriverDate";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverVersion, propertyKey))
    {
        return "Device_DriverVersion";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverDesc, propertyKey))
    {
        return "Device_DriverDescription";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverInfPath, propertyKey))
    {
        return "Device_DriverInfPath";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverInfSection, propertyKey))
    {
        return "Device_DriverInfSection";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverInfSectionExt, propertyKey))
    {
        return "Device_DriverInfSectionExt";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_MatchingDeviceId, propertyKey))
    {
        return "Device_MatchingDeviceId";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverProvider, propertyKey))
    {
        return "Device_DriverProvider";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverPropPageProvider, propertyKey))
    {
        return "Device_DriverPropPageProvider";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverCoInstallers, propertyKey))
    {
        return "Device_DriverCoInstallers";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ResourcePickerTags, propertyKey))
    {
        return "Device_ResourcePickerTags";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_ResourcePickerExceptions, propertyKey))
    {
        return "Device_ResourcePickerExceptions";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverRank, propertyKey))
    {
        return "Device_DriverRank";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_DriverLogoLevel, propertyKey))
    {
        return "Device_DriverLogoLevel";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_NoConnectSound, propertyKey))
    {
        return "Device_NoConnectSound";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_GenericDriverInstalled, propertyKey))
    {
        return "Device_GenericDriverInstalled";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_AdditionalSoftwareRequested, propertyKey))
    {
        return "Device_AdditionalSoftwareRequested";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_SafeRemovalRequired, propertyKey))
    {
        return "Device_SafeRemovalRequired";
    }
    if (IsEqualDevPropKey(DEVPKEY_Device_SafeRemovalRequiredOverride, propertyKey))
    {
        return "Device_SafeRemovalRequiredOverride";
    }
    if (IsEqualDevPropKey(DEVPKEY_DrvPkg_Model, propertyKey))
    {
        return "DrvPkg_Model";
    }
    if (IsEqualDevPropKey(DEVPKEY_DrvPkg_VendorWebSite, propertyKey))
    {
        return "DrvPkg_VendorWebSite";
    }
    if (IsEqualDevPropKey(DEVPKEY_DrvPkg_DetailedDescription, propertyKey))
    {
        return "DrvPkg_DetailedDescription";
    }
    if (IsEqualDevPropKey(DEVPKEY_DrvPkg_DocumentationLink, propertyKey))
    {
        return "DrvPkg_DocumentationLink";
    }
    if (IsEqualDevPropKey(DEVPKEY_DrvPkg_Icon, propertyKey))
    {
        return "DrvPkg_Icon";
    }
    if (IsEqualDevPropKey(DEVPKEY_DrvPkg_BrandingIcon, propertyKey))
    {
        return "DrvPkg_BrandingIcon";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_UpperFilters, propertyKey))
    {
        return "DeviceClass_UpperFilters";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_LowerFilters, propertyKey))
    {
        return "DeviceClass_LowerFilters";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_Security, propertyKey))
    {
        return "DeviceClass_Security";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_SecuritySDS, propertyKey))
    {
        return "DeviceClass_SecuritySDS";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_DevType, propertyKey))
    {
        return "DeviceClass_DevType";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_Exclusive, propertyKey))
    {
        return "DeviceClass_Exclusive";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_Characteristics, propertyKey))
    {
        return "DeviceClass_Characteristics";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_Name, propertyKey))
    {
        return "DeviceClass_Name";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_ClassName, propertyKey))
    {
        return "DeviceClass_ClassName";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_Icon, propertyKey))
    {
        return "DeviceClass_Icon";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_ClassInstaller, propertyKey))
    {
        return "DeviceClass_ClassInstaller";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_PropPageProvider, propertyKey))
    {
        return "DeviceClass_PropPageProvider";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_NoInstallClass, propertyKey))
    {
        return "DeviceClass_NoInstallClass";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_NoDisplayClass, propertyKey))
    {
        return "DeviceClass_NoDisplayClass";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_SilentInstall, propertyKey))
    {
        return "DeviceClass_SilentInstall";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_NoUseClass, propertyKey))
    {
        return "DeviceClass_NoUseClass";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_DefaultService, propertyKey))
    {
        return "DeviceClass_DefaultService";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_IconPath, propertyKey))
    {
        return "DeviceClass_IconPath";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_DHPRebalanceOptOut, propertyKey))
    {
        return "DeviceClass_DHPRebalanceOptOut";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceClass_ClassCoInstallers, propertyKey))
    {
        return "DeviceClass_ClassCoInstallers";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceInterface_FriendlyName, propertyKey))
    {
        return "DeviceInterface_FriendlyName";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceInterface_Enabled, propertyKey))
    {
        return "DeviceInterface_Enabled";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceInterface_ClassGuid, propertyKey))
    {
        return "DeviceInterface_ClassGuid";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceInterface_ReferenceString, propertyKey))
    {
        return "DeviceInterface_ReferenceString";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceInterface_Restricted, propertyKey))
    {
        return "DeviceInterface_Restricted";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceInterfaceClass_DefaultInterface, propertyKey))
    {
        return "DeviceInterfaceClass_DefaultInterface";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceInterfaceClass_Name, propertyKey))
    {
        return "DeviceInterfaceClass_Name";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Address, propertyKey))
    {
        return "DeviceContainer_Address";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_DiscoveryMethod, propertyKey))
    {
        return "DeviceContainer_DiscoveryMethod";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsEncrypted, propertyKey))
    {
        return "DeviceContainer_IsEncrypted";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsAuthenticated, propertyKey))
    {
        return "DeviceContainer_IsAuthenticated";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsConnected, propertyKey))
    {
        return "DeviceContainer_IsConnected";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsPaired, propertyKey))
    {
        return "DeviceContainer_IsPaired";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Icon, propertyKey))
    {
        return "DeviceContainer_Icon";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Version, propertyKey))
    {
        return "DeviceContainer_Version";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Last_Seen, propertyKey))
    {
        return "DeviceContainer_Last_Seen";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Last_Connected, propertyKey))
    {
        return "DeviceContainer_Last_Connected";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsShowInDisconnectedState, propertyKey))
    {
        return "DeviceContainer_IsShowInDisconnectedState";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsLocalMachine, propertyKey))
    {
        return "DeviceContainer_IsLocalMachine";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_MetadataPath, propertyKey))
    {
        return "DeviceContainer_MetadataPath";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsMetadataSearchInProgress, propertyKey))
    {
        return "DeviceContainer_IsMetadataSearchInProgress";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_MetadataChecksum, propertyKey))
    {
        return "DeviceContainer_MetadataChecksum";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsNotInterestingForDisplay, propertyKey))
    {
        return "DeviceContainer_IsNotInterestingForDisplay";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_LaunchDeviceStageOnDeviceConnect, propertyKey))
    {
        return "DeviceContainer_LaunchDeviceStageOnDeviceConnect";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_LaunchDeviceStageFromExplorer, propertyKey))
    {
        return "DeviceContainer_LaunchDeviceStageFromExplorer";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_BaselineExperienceId, propertyKey))
    {
        return "DeviceContainer_BaselineExperienceId";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsDeviceUniquelyIdentifiable, propertyKey))
    {
        return "DeviceContainer_IsDeviceUniquelyIdentifiable";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_AssociationArray, propertyKey))
    {
        return "DeviceContainer_AssociationArray";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_DeviceDescription1, propertyKey))
    {
        return "DeviceContainer_DeviceDescription1";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_DeviceDescription2, propertyKey))
    {
        return "DeviceContainer_DeviceDescription2";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_HasProblem, propertyKey))
    {
        return "DeviceContainer_HasProblem";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsSharedDevice, propertyKey))
    {
        return "DeviceContainer_IsSharedDevice";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsNetworkDevice, propertyKey))
    {
        return "DeviceContainer_IsNetworkDevice";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsDefaultDevice, propertyKey))
    {
        return "DeviceContainer_IsDefaultDevice";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_MetadataCabinet, propertyKey))
    {
        return "DeviceContainer_MetadataCabinet";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_RequiresPairingElevation, propertyKey))
    {
        return "DeviceContainer_RequiresPairingElevation";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_ExperienceId, propertyKey))
    {
        return "DeviceContainer_ExperienceId";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Category, propertyKey))
    {
        return "DeviceContainer_Category";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Category_Desc_Singular, propertyKey))
    {
        return "DeviceContainer_Category_Desc_Singular";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Category_Desc_Plural, propertyKey))
    {
        return "DeviceContainer_Category_Desc_Plural";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Category_Icon, propertyKey))
    {
        return "DeviceContainer_Category_Icon";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_CategoryGroup_Desc, propertyKey))
    {
        return "DeviceContainer_CategoryGroup_Desc";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_CategoryGroup_Icon, propertyKey))
    {
        return "DeviceContainer_CategoryGroup_Icon";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_PrimaryCategory, propertyKey))
    {
        return "DeviceContainer_PrimaryCategory";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_UnpairUninstall, propertyKey))
    {
        return "DeviceContainer_UnpairUninstall";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_RequiresUninstallElevation, propertyKey))
    {
        return "DeviceContainer_RequiresUninstallElevation";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_DeviceFunctionSubRank, propertyKey))
    {
        return "DeviceContainer_DeviceFunctionSubRank";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_AlwaysShowDeviceAsConnected, propertyKey))
    {
        return "DeviceContainer_AlwaysShowDeviceAsConnected";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_ConfigFlags, propertyKey))
    {
        return "DeviceContainer_ConfigFlags";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_PrivilegedPackageFamilyNames, propertyKey))
    {
        return "DeviceContainer_PrivilegedPackageFamilyNames";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_CustomPrivilegedPackageFamilyNames, propertyKey))
    {
        return "DeviceContainer_CustomPrivilegedPackageFamilyNames";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_IsRebootRequired, propertyKey))
    {
        return "DeviceContainer_IsRebootRequired";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_FriendlyName, propertyKey))
    {
        return "DeviceContainer_FriendlyName";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_Manufacturer, propertyKey))
    {
        return "DeviceContainer_Manufacturer";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_ModelName, propertyKey))
    {
        return "DeviceContainer_ModelName";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_ModelNumber, propertyKey))
    {
        return "DeviceContainer_ModelNumber";
    }
    if (IsEqualDevPropKey(DEVPKEY_DeviceContainer_InstallInProgress, propertyKey))
    {
        return "DeviceContainer_InstallInProgress";
    }
    if (IsEqualDevPropKey(DEVPKEY_DevQuery_ObjectType, propertyKey))
    {
        return "DevQuery_ObjectType";
    }

    return (guidToString(propertyKey.fmtid) + "[" + std::to_string(propertyKey.pid) + "]");
}

std::string propertyBufferToString(BYTE* propertyBuffer, ULONG propertyBufferSize, DEVPROPTYPE propertyType, DEVPROPKEY propertyKey)
{
    if (propertyType == DEVPROP_TYPE_EMPTY || propertyType == DEVPROP_TYPE_NULL)
    {
        return "";
    }
    else if (propertyType == DEVPROP_TYPE_SBYTE)
    {
        return std::to_string((INT8)propertyBuffer[0]);
    }
    else if (propertyType == DEVPROP_TYPE_BYTE)
    {
        return std::to_string((UINT8)propertyBuffer[0]);
    }
    else if (propertyType == DEVPROP_TYPE_INT16)
    {
        INT16 i = 0;
        memcpy(&i, propertyBuffer, sizeof(INT16));
        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_UINT16)
    {
        UINT16 i = 0;
        memcpy(&i, propertyBuffer, sizeof(UINT16));
        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_INT32)
    {
        INT32 i = 0;
        memcpy(&i, propertyBuffer, sizeof(INT32));
        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_UINT32)
    {
        UINT32 i = 0;
        memcpy(&i, propertyBuffer, sizeof(UINT32));
        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_INT64)
    {
        INT64 i = 0;
        memcpy(&i, propertyBuffer, sizeof(INT64));
        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_UINT64)
    {
        UINT64 i = 0;
        memcpy(&i, propertyBuffer, sizeof(UINT64));
        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_FLOAT)
    {
        FLOAT i = 0;
        memcpy(&i, propertyBuffer, sizeof(FLOAT));
        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_DOUBLE)
    {
        DOUBLE i = 0;
        memcpy(&i, propertyBuffer, sizeof(DOUBLE));
        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_DECIMAL)
    {
        DECIMAL d;
        memcpy(&d, propertyBuffer, sizeof(DECIMAL));
        ULONGLONG ret = (ULONGLONG(d.Lo64) + ULONGLONG(d.Hi32) * ULONGLONG(pow(10, d.scale)));
        if (d.sign == 1)
        {
            return std::to_string(ret);
        }
        else
        {
            return "-" + std::to_string(ret);
        }
    }
    else if (propertyType == DEVPROP_TYPE_GUID)
    {
        GUID d;
        memcpy(&d, propertyBuffer, sizeof(GUID));
        return guidToString(d);
    }
    else if (propertyType == DEVPROP_TYPE_CURRENCY)
    {
        CURRENCY d;
        memcpy(&d, propertyBuffer, sizeof(CURRENCY));
        return std::to_string(d.int64);
    }
    else if (propertyType == DEVPROP_TYPE_DATE)
    {
        DATE d;
        memcpy(&d, propertyBuffer, sizeof(DATE));
        return std::to_string(d);
    }
    else if (propertyType == DEVPROP_TYPE_FILETIME)
    {
        FILETIME d;
        memcpy(&d, propertyBuffer, sizeof(FILETIME));
        SYSTEMTIME systemTime;
        if (FileTimeToSystemTime(&d, &systemTime))
        {
            return (std::to_string(systemTime.wMonth) + "/" + std::to_string(systemTime.wDay) + "/" + std::to_string(systemTime.wYear) + \
                " - " + std::to_string(systemTime.wHour) + "-" + std::to_string(systemTime.wMinute) + "-" + std::to_string(systemTime.wSecond) + "." + \
                std::to_string(systemTime.wMilliseconds));
        }
        else
        {
            return "Unable to convert FILETIME";
        }
    }
    else if (propertyType == DEVPROP_TYPE_BOOLEAN)
    {
        if (propertyBuffer[0] == 0)
        {
            return "False";
        }
        else
        {
            return "True";
        }
    }
    else if (propertyType == DEVPROP_TYPE_STRING)
    {
        return rTrim(wStringToString(std::wstring((wchar_t*)propertyBuffer, propertyBufferSize / sizeof(wchar_t))));
    }
    else if (propertyType == DEVPROP_TYPE_STRING_LIST)
    {
        std::string retStr = rTrim(wStringToString(std::wstring((wchar_t*)propertyBuffer, propertyBufferSize / sizeof(wchar_t))));
        retStr = delimitedStringToNewlineString(retStr);
        return retStr;
    }
    else if (propertyType == DEVPROP_TYPE_SECURITY_DESCRIPTOR)
    {
        SECURITY_DESCRIPTOR securityDescriptor;
        memcpy(&securityDescriptor, propertyBuffer, sizeof(SECURITY_DESCRIPTOR));
        ULONG retStrLen = 1000;
        std::string retStr(" ", retStrLen);
        if (ConvertSecurityDescriptorToStringSecurityDescriptor(&securityDescriptor, SDDL_REVISION_1, OWNER_SECURITY_INFORMATION, (char**)retStr.data(), &retStrLen))
        {
            return std::string(retStr.data(), retStrLen);
        }
        else
        {
            return "Unable to convert SECURITY_DESCRIPTOR";
        }
    }
    else if (propertyType == DEVPROP_TYPE_SECURITY_DESCRIPTOR_STRING)
    {
        return rTrim(wStringToString(std::wstring((wchar_t*)propertyBuffer, propertyBufferSize / sizeof(wchar_t))));
    }
    else if (propertyType == DEVPROP_TYPE_DEVPROPKEY)
    {
        DEVPROPKEY d;
        memcpy(&d, propertyBuffer, sizeof(DEVPROPKEY));
        return propertyKeyToString(d);
    }
    else if (propertyType == DEVPROP_TYPE_DEVPROPTYPE)
    {
        DEVPROPTYPE d;
        memcpy(&d, propertyBuffer, sizeof(DEVPROPTYPE));
        return std::to_string(d);
    }
    else if (propertyKey == DEVPKEY_Device_PowerData && propertyType == DEVPROP_TYPE_BINARY)
    {
        PCM_POWER_DATA cmPowerData = (PCM_POWER_DATA)propertyBuffer;
        std::string retStr = "Most Recent Power State: " + powerStateToString(cmPowerData->PD_MostRecentPowerState) + "\n";
        retStr += "Supported Capabilities: ";
        if (cmPowerData->PD_Capabilities & PDCAP_D0_SUPPORTED)
        {
            retStr += "(D0) ";
        }
        if (cmPowerData->PD_Capabilities & PDCAP_D1_SUPPORTED)
        {
            retStr += "(D1) ";
        }
        if (cmPowerData->PD_Capabilities & PDCAP_D2_SUPPORTED)
        {
            retStr += "(D2) ";
        }
        if (cmPowerData->PD_Capabilities & PDCAP_D3_SUPPORTED)
        {
            retStr += "(D3) ";
        }
        if (cmPowerData->PD_Capabilities & PDCAP_WAKE_FROM_D0_SUPPORTED)
        {
            retStr += "(Wake from D0) ";
        }
        if (cmPowerData->PD_Capabilities & PDCAP_WAKE_FROM_D1_SUPPORTED)
        {
            retStr += "(Wake from D1) ";
        }
        if (cmPowerData->PD_Capabilities & PDCAP_WAKE_FROM_D2_SUPPORTED)
        {
            retStr += "(Wake from D2) ";
        }
        if (cmPowerData->PD_Capabilities & PDCAP_WAKE_FROM_D3_SUPPORTED)
        {
            retStr += "(Wake from D3) ";
        }
        if (cmPowerData->PD_Capabilities & PDCAP_WARM_EJECT_SUPPORTED)
        {
            retStr += "(Warm eject) ";
        }
        retStr += "\n";
        retStr += "Latency When Returning To D0 From D1: " + std::to_string(cmPowerData->PD_D1Latency) + " microseconds \n";
        retStr += "Latency When Returning To D0 From D2: " + std::to_string(cmPowerData->PD_D2Latency) + " microseconds \n";
        retStr += "Latency When Returning To D0 From D3: " + std::to_string(cmPowerData->PD_D3Latency) + " microseconds \n";

        for (ULONG i = 0; i < POWER_SYSTEM_MAXIMUM; i++)
        {
            retStr += systemPowerStateToString(i) + ": " + powerStateToString(cmPowerData->PD_PowerStateMapping[i]) + "\n";
        }

        retStr += "Lowest State Where The Device Can Wake The System: " + systemPowerStateToString(cmPowerData->PD_DeepestSystemWake);

        return retStr;
    }
    else if (propertyType == DEVPROP_TYPE_BINARY)
    {
        return byteArrayToString(propertyBuffer, propertyBufferSize);
    }
    else if (propertyType == DEVPROP_TYPE_ERROR)
    {
        UINT32 i = 0;
        memcpy(&i, propertyBuffer, sizeof(UINT32));

        return windowsErrorToString(i);
    }
    else if (propertyType == DEVPROP_TYPE_NTSTATUS)
    {
        UINT32 i = 0;
        memcpy(&i, propertyBuffer, sizeof(UINT32));

        return std::to_string(i);
    }
    else if (propertyType == DEVPROP_TYPE_STRING_INDIRECT)
    {
        return rTrim(wStringToString(std::wstring((wchar_t*)propertyBuffer, propertyBufferSize / sizeof(wchar_t))));
    }

    return rTrim(wStringToString(std::wstring((wchar_t*)propertyBuffer, propertyBufferSize / sizeof(wchar_t))));

}

std::string byteArrayToString(BYTE* buffer, ULONG bufferSize)
{
    if (bufferSize == 0)
    {
        return ZERO_LENGTH_BUFFER;
    }

    std::string retStr;
    std::string characterLine = "";
    for (size_t i = 0; i < bufferSize; i++)
    {
        char buf[4];
        snprintf(buf, sizeof(buf), "%02X ", buffer[i]);
        if (i != 0 && i % 8 == 0)
        {
            retStr += " " + characterLine + "\n";
            characterLine = "";
        }
        if (buffer[i] >= 32 && buffer[i] <= 126)
        {
            characterLine += (char)buffer[i];
        }
        else
        {
            characterLine += ".";
        }
        retStr += buf;
    }
    while (retStr.size() < 24)
    {
        retStr += " ";
    }

    return retStr + " " + characterLine;
}

std::string resourceTypeToString(RESOURCEID resourceType)
{
    if (resourceType == 0)
    {
        return "ResType_All";
    }
    if (resourceType == 1)
    {
        return "ResType_Mem";
    }
    if (resourceType == 2)
    {
        return "ResType_IO";
    }
    if (resourceType == 3)
    {
        return "ResType_DMA";
    }
    if (resourceType == 4)
    {
        return "ResType_IRQ";
    }
    if (resourceType == 5)
    {
        return "ResType_DoNotUse";
    }
    if (resourceType == 6)
    {
        return "ResType_BusNumber";
    }
    if (resourceType == 7)
    {
        return "ResType_MemLarge";
    }
    if (resourceType == 0x00008000)
    {
        return "ResType_Ignored_Bit";
    }
    if (resourceType == 0x0000FFFF)
    {
        return "ResType_ClassSpecific";
    }
    if (resourceType == 0x00008001)
    {
        return "ResType_DevicePrivate";
    }
    if (resourceType == 0x00008002)
    {
        return "ResType_PcCardConfig";
    }
    if (resourceType == 0x00008003)
    {
        return "ResType_MfCardConfig";
    }
    if (resourceType == 0x00008004)
    {
        return "ResType_Connection";
    }

    return "ResType_Unknown";
}

std::string resourceToString(BYTE* buffer, ULONG bufferSize, RESOURCEID resourceType)
{
    if (resourceType == ResType_BusNumber)
    {
        PBUSNUMBER_DES pBusNumberDes = (PBUSNUMBER_DES)buffer;
        std::string retStr = "Bus Range: " + numToHexString(pBusNumberDes->BUSD_Alloc_Base, 8) + " - " + numToHexString(pBusNumberDes->BUSD_Alloc_End, 8);
        return retStr;
    }
    else if (resourceType == ResType_ClassSpecific)
    {
        PCS_DES pCSDes = (PCS_DES)buffer;
        std::string retStr = "GUID: " + guidToString(pCSDes->CSD_ClassGuid) + "\n";
        retStr += "Signature: " + std::string((char*)pCSDes->CSD_Signature, pCSDes->CSD_SignatureLength) + "\n";
        retStr += "LegacyData: " + std::string((char*)(pCSDes->CSD_Signature + pCSDes->CSD_LegacyDataOffset), pCSDes->CSD_LegacyDataSize) + "\n";
        return retStr;
    }
    else if (resourceType == ResType_DMA)
    {
        PDMA_DES pDmaDes = (PDMA_DES)buffer;
        std::string retStr = "Allocated DMA Channel:" + numToHexString(pDmaDes->DD_Alloc_Chan, 8) + "\n";
        retStr += "Flags: ";
        if (pDmaDes->DD_Flags & fDD_BYTE)
        {
            retStr += "(8 bit DMA channel) ";
        }
        if (pDmaDes->DD_Flags & fDD_WORD)
        {
            retStr += "(16 bit DMA channel) ";
        }
        if (pDmaDes->DD_Flags & fDD_DWORD)
        {
            retStr += "(32 bit DMA channel) ";
        }
        if (pDmaDes->DD_Flags & fDD_BYTE_AND_WORD)
        {
            retStr += "(8 and 16 bit DMA channel) ";
        }
        if (pDmaDes->DD_Flags & fDD_NoBusMaster)
        {
            retStr += "(No bus mastering) ";
        }
        if (pDmaDes->DD_Flags & fDD_BusMaster)
        {
            retStr += "(Bus mastering) ";
        }
        if (pDmaDes->DD_Flags & fDD_TypeStandard)
        {
            retStr += "(Standard DMA) ";
        }
        if (pDmaDes->DD_Flags & fDD_TypeA)
        {
            retStr += "(Type A DMA) ";
        }
        if (pDmaDes->DD_Flags & fDD_TypeB)
        {
            retStr += "(Type B DMA) ";
        }
        if (pDmaDes->DD_Flags & fDD_TypeF)
        {
            retStr += "(Type F DMA) ";
        }
        return retStr;
    }
    else if (resourceType == ResType_IO)
    {
        PIO_DES pIoDes = (PIO_DES)buffer;
        std::string retStr = "IO Port Address Range: " + numToHexString(pIoDes->IOD_Alloc_Base, 16) + " - " + numToHexString(pIoDes->IOD_Alloc_End, 16) + "\n";
        retStr += "Flags: ";
        if (pIoDes->IOD_DesFlags & fIOD_IO)
        {
            retStr += "(IO address space) ";
        }
        if (pIoDes->IOD_DesFlags & fIOD_Memory)
        {
            retStr += "(Memory addres space) ";
        }
        if (pIoDes->IOD_DesFlags & fIOD_10_BIT_DECODE)
        {
            retStr += "(Decodes 10 bits the port address) ";
        }
        if (pIoDes->IOD_DesFlags & fIOD_12_BIT_DECODE)
        {
            retStr += "(Decodes 12 bits the port address) ";
        }
        if (pIoDes->IOD_DesFlags & fIOD_16_BIT_DECODE)
        {
            retStr += "(Decodes 16 bits the port address) ";
        }
        if (pIoDes->IOD_DesFlags & fIOD_POSITIVE_DECODE)
        {
            retStr += "(Positive diode) ";
        }
        return retStr;
    }
    else if (resourceType == ResType_IRQ)
    {
        PIRQ_DES pIrqDes = (PIRQ_DES)buffer;
        std::string retStr = "IRQ Line Number: " + numToHexString(pIrqDes->IRQD_Alloc_Num, 8) + "\n";
        retStr += "IRQ Affinity: " + numToHexString(pIrqDes->IRQD_Affinity, 16) + "\n";
        retStr += "Flags: ";
        if (pIrqDes->IRQD_Flags & fIRQD_Exclusive)
        {
            retStr += "(IRQ line cannot be shared) ";
        }
        if (pIrqDes->IRQD_Flags & fIRQD_Share)
        {
            retStr += "(IRQ line can be shared) ";
        }
        if (pIrqDes->IRQD_Flags & fIRQD_Level)
        {
            retStr += "(IRQ line is level-triggered) ";
        }
        if (pIrqDes->IRQD_Flags & fIRQD_Edge)
        {
            retStr += "(IRQ line is edge-triggered) ";
        }
        return retStr;
    }
    else if (resourceType == ResType_Mem)
    {
        PMEM_DES pMemDes = (PMEM_DES)buffer;
        std::string retStr = "Contiguous Physical Memory Address Range: " + numToHexString(pMemDes->MD_Alloc_Base, 16) + " - " + numToHexString(pMemDes->MD_Alloc_End, 16) + "\n";
        retStr += "Flags: ";
        if (pMemDes->MD_Flags & fMD_ROM)
        {
            retStr += "(Memory is read-only) ";
        }
        if (pMemDes->MD_Flags & fMD_RAM)
        {
            retStr += "(Memory is not read-only) ";
        }
        if (pMemDes->MD_Flags & fMD_ReadDisallowed)
        {
            retStr += "(Memory is write-only) ";
        }
        if (pMemDes->MD_Flags & fMD_ReadAllowed)
        {
            retStr += "(Memory is not write-only) ";
        }
        if (pMemDes->MD_Flags & fMD_24)
        {
            retStr += "(24 bit addressing) ";
        }
        if (pMemDes->MD_Flags & fMD_24)
        {
            retStr += "(32 bit addressing) ";
        }
        if (pMemDes->MD_Flags & fMD_PrefetchAllowed)
        {
            retStr += "(Memory range can be prefetched) ";
        }
        if (pMemDes->MD_Flags & fMD_PrefetchDisallowed)
        {
            retStr += "(Memory range cannot be prefetched) ";
        }
        if (pMemDes->MD_Flags & fMD_Cacheable)
        {
            retStr += "(Memory range can be cached) ";
        }
        if (pMemDes->MD_Flags & fMD_NonCacheable)
        {
            retStr += "(Memory range cannot be cached) ";
        }
        if (pMemDes->MD_Flags & fMD_CombinedWriteAllowed)
        {
            retStr += "(Combined write caching is allowed) ";
        }
        if (pMemDes->MD_Flags & fMD_CombinedWriteDisallowed)
        {
            retStr += "(Combined write caching is not allowed) ";
        }
        return retStr;
    }
    return byteArrayToString(buffer, bufferSize);
}

std::string numToHexString(UINT64 number, int width)
{
    // Use the C++ method on larger strings, otherwise the C method is much faster (~3x)
    if (number > 0xFFFF)
    {
        std::stringstream sstream;
        sstream << "0x" << std::setfill('0') << std::setw(width) << std::hex << std::uppercase << number;
        return sstream.str();
    }
    else
    {
        char charArray[7] = { "\0" };
        snprintf(charArray, 7, "0x%X", (unsigned int) number);
        return std::string(charArray);
    }
}

std::string powerStateToString(ULONG powerState)
{
    if (powerState == 0)
    {
        return "Unspecified device power state";
    }
    else if (powerState == 1)
    {
        return "Maximum device power state (D0)";
    }
    else if (powerState == 2)
    {
        return "Device sleeping state less than D0 and greater than D2 (D1)";
    }
    else if (powerState == 3)
    {
        return "Device sleeping state less than D1 and greater than D3 (D2)";
    }
    else if (powerState == 4)
    {
        return "Lowest powered device sleeping state (D3)";
    }
    else if (powerState == 5)
    {
        return "Power device maximum";
    }

    return "Unknown power state";
}

std::string systemPowerStateToString(ULONG tmp)
{
    switch (tmp)
    {
    case(SYSTEM_POWER_STATE::PowerSystemUnspecified):
        return "Power System Unspecified";
    case(SYSTEM_POWER_STATE::PowerSystemWorking):
        return "Power System Working";
    case(SYSTEM_POWER_STATE::PowerSystemSleeping1):
        return "Power System Sleeping1";
    case(SYSTEM_POWER_STATE::PowerSystemSleeping2):
        return "Power System Sleeping2";
    case(SYSTEM_POWER_STATE::PowerSystemSleeping3):
        return "Power System Sleeping3";
    case(SYSTEM_POWER_STATE::PowerSystemHibernate):
        return "Power System Hibernate";
    case(SYSTEM_POWER_STATE::PowerSystemShutdown):
        return "Power System Shutdown";
    case(SYSTEM_POWER_STATE::PowerSystemMaximum):
        return "Power System Maximum";
    }
    return "Unknown Power System";
}

std::string delimitedStringToNewlineString(std::string& retStr)
{
    std::replace(retStr.begin(), retStr.end(), '\0', '\n');
    retStr = std::regex_replace(retStr, std::regex("\n\n"), "\n");

    return rTrim(retStr);
}

std::string storageBusTypeToString(STORAGE_BUS_TYPE busType)
{
    if (busType == BusTypeScsi)
    {
        return "SCSI";
    }
    if (busType == BusTypeAtapi)
    {
        return "ATAPI";
    }
    if (busType == BusTypeAta)
    {
        return "ATA";
    }
    if (busType == BusType1394)
    {
        return "1394";
    }
    if (busType == BusTypeSsa)
    {
        return "SSA";
    }
    if (busType == BusTypeFibre)
    {
        return "Fibre";
    }
    if (busType == BusTypeUsb)
    {
        return "USB";
    }
    if (busType == BusTypeRAID)
    {
        return "RAID";
    }
    if (busType == BusTypeiScsi)
    {
        return "iSCSI";
    }
    if (busType == BusTypeSas)
    {
        return "SAS";
    }
    if (busType == BusTypeSata)
    {
        return "SATA";
    }
    if (busType == BusTypeSd)
    {
        return "SD";
    }
    if (busType == BusTypeMmc)
    {
        return "MMC";
    }
    if (busType == BusTypeVirtual)
    {
        return "Virtual";
    }
    if (busType == BusTypeFileBackedVirtual)
    {
        return "File Backed Virtual";
    }
    if (busType == BusTypeSpaces)
    {
        return "Spaces";
    }
    if (busType == BusTypeNvme)
    {
        return "NVMe";
    }

    return "Unknown";
}

bool stringToGuid(std::string guidAsString, LPGUID guid)
{
    return (CLSIDFromString((LPCOLESTR)stringToWString(guidAsString).c_str(), guid) == NOERROR);
}

std::string usbConnectionStatusToString(USB_CONNECTION_STATUS tmp)
{
    switch (tmp)
    {
    case(USB_CONNECTION_STATUS::NoDeviceConnected):
        return "NoDeviceConnected";
    case(USB_CONNECTION_STATUS::DeviceConnected):
        return "DeviceConnected";
    case(USB_CONNECTION_STATUS::DeviceFailedEnumeration):
        return "DeviceFailedEnumeration";
    case(USB_CONNECTION_STATUS::DeviceGeneralFailure):
        return "DeviceGeneralFailure";
    case(USB_CONNECTION_STATUS::DeviceCausedOvercurrent):
        return "DeviceCausedOvercurrent";
    case(USB_CONNECTION_STATUS::DeviceNotEnoughPower):
        return "DeviceNotEnoughPower";
    case(USB_CONNECTION_STATUS::DeviceNotEnoughBandwidth):
        return "DeviceNotEnoughBandwidth";
    case(USB_CONNECTION_STATUS::DeviceHubNestedTooDeeply):
        return "DeviceHubNestedTooDeeply";
    case(USB_CONNECTION_STATUS::DeviceInLegacyHub):
        return "DeviceInLegacyHub";
    case(USB_CONNECTION_STATUS::DeviceEnumerating):
        return "DeviceEnumerating";
    case(USB_CONNECTION_STATUS::DeviceReset):
        return "DeviceReset";
    }
    return "No Enum Value Matches, Unknown";
}