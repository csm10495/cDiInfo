/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Registry.cpp - Windows Registry interfacing implementation file
*/

// Local includes
#include "Registry.h"

namespace cdi
{
    namespace registry
    {
        bool getStringFromRegistry(HKEY hKey, std::string subKey, std::string value, std::string & output)
        {
            DWORD size;
            DWORD type;
            LONG retVal = RegGetValue(hKey, subKey.c_str(), value.c_str(), RRF_RT_ANY, NULL, NULL, &size);

            // For some reason this doesn't always work unless I add a bit (maybe for a null terminator?)
            size = size + 1;

            if (retVal == ERROR_SUCCESS)
            {
                char * allocatedString = new char[size];
                retVal = RegGetValue(hKey, subKey.c_str(), value.c_str(), RRF_RT_ANY, &type, allocatedString, &size);

                output = cdi::strings::rTrim(std::string(allocatedString, size - 1));
                delete[] allocatedString;

                if (retVal == ERROR_SUCCESS)
                {
                    return true;
                }
            }

            return false;
        }

        bool getUIntFromRegistry(HKEY hkey, std::string subKey, std::string value, UINT64 &output)
        {
            std::string outString = "";
            if (getStringFromRegistry(hkey, subKey, value, outString))
            {
                // Now we have a string, cast it back to UINT64
                memcpy(&output, outString.data(), outString.size());
                return true;
            }
            return false;
        }

    }
}

