/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Session.h - Header for a single session. Most core things should run within one session.
*/
#pragma once

// Local includes
#include "Core.h"

namespace cdi
{
    namespace session
    {
        class Session
        {
        public:
            // Constructor
            Session();

            // Get the cached device id to scsi port map
            DeviceIdToScsiPortMap &getDeviceIdToScsiPortMap();

            // Get the cached ms dos device name to drive letter map
            std::map<std::string, std::string> &getMsDosDeviceNameToDriveLetterMap();
        private:
            // The device id to scsi port map
            DeviceIdToScsiPortMap deviceIdToScsiPortMap;

            // the cached ms dos device name to drive letter map
            std::map<std::string, std::string> msDosDeviceNameToDriveLetterMap;
        };
    }
}