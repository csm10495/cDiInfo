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