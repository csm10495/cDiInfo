/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* CSMI.cpp - Implementation file for CSMI info-getting
*/

// Local includes
#include "Attribute.h"
#include "CSMI.h"
#include "Strings.h"

// WinApi includes
#include "Ntddscsi.h"

namespace cdi
{
    namespace detection
    {
        namespace csmi
        {
            // todo: pass in AttributeSet to add things about the overall raid array
            cdi::attr::AttributeSetVector getCSMIDevices(std::string devicePath)
            {
                cdi::attr::AttributeSetVector retVec;

                HANDLE handle = CreateFile(devicePath.c_str(),
                    GENERIC_WRITE | GENERIC_READ,
                    (FILE_SHARE_READ | FILE_SHARE_WRITE),
                    NULL,
                    OPEN_EXISTING,
                    NULL,
                    NULL);

                if (handle != INVALID_HANDLE_VALUE)
                {
                    // Get the RAID Configuration... which should include basic information on each device
                    BYTE buffer[8192] = { 0 };
                    PCSMI_SAS_RAID_CONFIG_BUFFER raidConfigBuffer = (PCSMI_SAS_RAID_CONFIG_BUFFER)buffer;
                    raidConfigBuffer->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                    raidConfigBuffer->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                    raidConfigBuffer->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_RAID_CONFIG;
                    raidConfigBuffer->IoctlHeader.Length = sizeof(buffer) - sizeof(IOCTL_HEADER);
                    memcpy_s(&raidConfigBuffer->IoctlHeader.Signature, sizeof(raidConfigBuffer->IoctlHeader.Signature), CSMI_RAID_SIGNATURE, sizeof(CSMI_RAID_SIGNATURE));

                    DWORD bytesReturned = 0;
                    if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer, sizeof(buffer), &buffer, sizeof(buffer), &bytesReturned, NULL) && raidConfigBuffer->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                    {
                        // todo: there is info in raidConfigBuffer that should go back up to 
                        PCSMI_SAS_RAID_DRIVES drive = raidConfigBuffer->Configuration.Drives;
                        for (size_t i = 0; i < raidConfigBuffer->Configuration.bDriveCount; i++)
                        {
                            cdi::attr::AttributeSet raidDrive;
                            raidDrive.insert(cdi::attr::Attribute("Model", "A unique descriptor for this particular product from this manufacturer.", std::string((char*)drive->bModel, sizeof(drive->bModel))));
                            raidDrive.insert(cdi::attr::Attribute("SerialNumber", "A unique descriptor for this particular device from the manufacturer.", std::string((char*)drive->bSerialNumber, sizeof(drive->bSerialNumber))));
                            raidDrive.insert(cdi::attr::Attribute("ProductRevision", "A unique descriptor for the host software on this device. Sometimes known as Firmware.", std::string((char*)drive->bFirmware, sizeof(drive->bFirmware))));
                            raidDrive.insert(cdi::attr::Attribute("AdapterDevicePath", "The device path for the device's adapter.", devicePath));
                            raidDrive.insert(cdi::attr::Attribute((BYTE*)&drive->bDriveStatus, sizeof(drive->bDriveStatus), "DriveStatus", "A status for the drive based on the current RAID.", cdi::strings::csmiDriveStatusToString(drive->bDriveStatus)));
                            raidDrive.insert(cdi::attr::Attribute((BYTE*)&drive->bDriveUsage, sizeof(drive->bDriveUsage), "DriveUsage", "A descriptor of how the drive is being used in terms of RAID.", cdi::strings::csmiDriveUsageToString(drive->bDriveUsage)));
                            raidDrive.insert(cdi::attr::Attribute((BYTE*)&drive->bDriveType, sizeof(drive->bDriveType), "DriveType", "A descriptor of the type of drive this is.", cdi::strings::csmiDriveTypeToString(drive->bDriveType)));

                            // The Device Id is made up here... todo: get rid of spaces
                            std::string psuedoDeviceId = "CSMI\\RAID_DISK\\" + cdi::strings::rTrim(std::string((char*)drive->bModel, sizeof(drive->bModel))) + \
                                cdi::strings::rTrim(std::string((char*)drive->bFirmware, sizeof(drive->bFirmware))) + cdi::strings::rTrim(std::string((char*)drive->bSerialNumber, sizeof(drive->bSerialNumber))) + "&" + std::to_string(i);

                            raidDrive.insert(cdi::attr::Attribute("PseudoDeviceId", "A pseudo-unique identifier for the device. This is used if the device wasn't detected with a Device (Instance) Id.", psuedoDeviceId));

                            // todo: use CSMI_SAS_GET_SCSI_ADDRESS_BUFFER to get port,path,target,lun

                            retVec.push_back(raidDrive);
                            drive++;
                        }
                    }

                    CloseHandle(handle);
                }

                return retVec;
            }
        }
    }
}