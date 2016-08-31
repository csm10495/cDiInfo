/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Session.cpp - Header for a single session. Most core things should run within one session.
*/

// Local includes
#include "Session.h"

namespace cdi
{
    namespace session
    {
        Session::Session()
        {
            // Don't need to get these over and over, cache them.
            deviceIdToScsiPortMap = getDeviceIdToScsiPortMap();
            msDosDeviceNameToDriveLetterMap = getMsDosDeviceNameToDriveLetterMap();
        }

        DeviceIdToScsiPortMap &Session::getDeviceIdToScsiPortMap()
        {
            return deviceIdToScsiPortMap;
        }

        std::map<std::string, std::string> &Session::getMsDosDeviceNameToDriveLetterMap()
        {
            return msDosDeviceNameToDriveLetterMap;
        }
    }
}