/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* CSMI.h - Implementation file for CSMI info-getting
*/

#pragma once

// Local includes
#include "Include/csmisas.h"

// STL includes
#include <string>

// WinApi includes
#include <Windows.h>

namespace cdi
{
    // Forward declaration(s)
    namespace { namespace attr { class AttributeSetVector; } };

    namespace detection
    {
        namespace csmi
        {
            cdi::attr::AttributeSetVector getCSMIDevices(std::string devicePath);
        }
    }
}