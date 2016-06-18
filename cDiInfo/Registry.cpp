/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Registry.cpp - Windows Registry interfacing implementation file
*/

// Local includes
#include "Registry.h"

bool getStringFromRegistry(HKEY hKey, std::string subKey, std::string value, std::string & output)
{
    DWORD size;
    DWORD type;
    LONG retVal = RegGetValue(hKey, subKey.c_str(), value.c_str(), RRF_RT_ANY, NULL, NULL, &size);

    if (retVal == ERROR_SUCCESS)
    {
        char * allocatedString = new char[size];
        retVal = RegGetValue(hKey, subKey.c_str(), value.c_str(), RRF_RT_ANY, &type, allocatedString, &size);

        output = rTrim(std::string(allocatedString, size - 1));
        delete[] allocatedString;

        if (retVal == ERROR_SUCCESS)
        {
            return true;
        }
    }

    return false;
}
