/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Enumerations.h - Header for Locally-Defined Enums
*/
#pragma once

// STL includes
#include <string>

// Gets the name of the passed in variable
#define getVarName(var) #var

// Used to denote the completion of a command
typedef enum STATUS {
    SUCCESS = 0,
    CM_FAILED,
    FAILED_TO_GET_MATCHING_DEVINST,
} STATUS;

// A basic representation to designate the type used for registry properties
typedef enum TYPE {
    __STRING_,
    __WSTRING_,
    _INT_,
    _GUID_,
} TYPE;


namespace cdi
{
    namespace enums
    {
        // Returns the string for the given status
        std::string getStatusStr(STATUS s);
    }
}


