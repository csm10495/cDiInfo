/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Strings.cpp - Implementation file for string manipulation functions
*/

// Local includes
#include "Strings.h"

std::string &rTrim(std::string & s)
{
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
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
