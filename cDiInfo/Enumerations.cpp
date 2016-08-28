/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Enumerations.cpp - Implementations for Locally-Defined Enum functions
*/

// Local includes
#include "Enumerations.h"

namespace cdi
{
    namespace enums
    {
        std::string getStatusStr(STATUS s)
        {
            if (s == SUCCESS)
                return getVarName(SUCCESS);
            else if (s == CM_FAILED)
                return getVarName(CM_FAILED);
            else if (s == FAILED_TO_GET_MATCHING_DEVINST)
                return getVarName(FAILED_TO_GET_MATCHING_DEVINST);

            return "Unknown Status";
        }
    }
}
