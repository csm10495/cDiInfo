/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* CSMI.h - Implementation file for CSMI info-getting
*/

#pragma once

// Local includes
#include "Include/csmisas.h"

// STL includes
#include <map>
#include <string>

// WinApi includes
#include <Windows.h>

// For SMART parsing
#define BAD_SMART_LOW   0xF4
#define BAD_SMART_HIGH  0x2C

namespace cdi
{
    // Forward declaration(s)
    namespace { namespace attr { class AttributeSetVector; } };

    namespace detection
    {
        namespace csmi
        {
            // Returns an AttributeSetVector of devices found behind CSMI RAID
            cdi::attr::AttributeSetVector getCSMIDevices(std::string devicePath);

            // Adds attributes via CSMI to a CSMI RAID controller
            void addControllerCSMIAttributes(HANDLE &handle, cdi::attr::AttributeSet &attributeSet);

            // Gets a map of identifier to CSMI_SAS_PHY_ENTITY from the given handle 
            std::map<UCHAR, CSMI_SAS_PHY_ENTITY> getIdToPhyEntries(HANDLE &handle);
        }
    }
}