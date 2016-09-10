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
                    BYTE buffer[8192] = { 0 };
                    BYTE buffer2[8192] = { 0 };

                    // Get the RAID Configuration... which should include basic information on each 
                    PCSMI_SAS_RAID_CONFIG_BUFFER raidConfigBuffer = (PCSMI_SAS_RAID_CONFIG_BUFFER)buffer;
                    raidConfigBuffer->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                    raidConfigBuffer->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                    raidConfigBuffer->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_RAID_CONFIG;
                    raidConfigBuffer->IoctlHeader.Length = sizeof(buffer) - sizeof(IOCTL_HEADER);
                    memcpy_s(&raidConfigBuffer->IoctlHeader.Signature, sizeof(raidConfigBuffer->IoctlHeader.Signature), CSMI_RAID_SIGNATURE, sizeof(CSMI_RAID_SIGNATURE));

                    DWORD bytesReturned = 0;
                    if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer, sizeof(buffer), &buffer, sizeof(buffer), &bytesReturned, NULL) && bytesReturned > 0 && raidConfigBuffer->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
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

                            std::string pseudoDeviceId = "CSMI\\RAID_DISK\\" + cdi::strings::rTrim(std::string((char*)drive->bModel, sizeof(drive->bModel))) + \
                                cdi::strings::rTrim(std::string((char*)drive->bFirmware, sizeof(drive->bFirmware))) + cdi::strings::rTrim(std::string((char*)drive->bSerialNumber, sizeof(drive->bSerialNumber))) + "&" + std::to_string(i);

                            pseudoDeviceId = cdi::strings::recursiveReplace(pseudoDeviceId, "  ", " ");
                            pseudoDeviceId = cdi::strings::recursiveReplace(pseudoDeviceId, " ", "&");

                            raidDrive.insert(cdi::attr::Attribute("PseudoDeviceId", "A pseudo-unique identifier for the device. This is used if the device wasn't detected with a Device (Instance) Id.", pseudoDeviceId));

                            PCSMI_SAS_GET_SCSI_ADDRESS_BUFFER scsiAddressBuffer = (PCSMI_SAS_GET_SCSI_ADDRESS_BUFFER)buffer2;
                            scsiAddressBuffer->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_SCSI_ADDRESS;
                            scsiAddressBuffer->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                            scsiAddressBuffer->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                            scsiAddressBuffer->IoctlHeader.Length = sizeof(buffer2) - sizeof(IOCTL_HEADER);
                            memcpy_s(&scsiAddressBuffer->IoctlHeader.Signature, sizeof(scsiAddressBuffer->IoctlHeader.Signature), CSMI_SAS_SIGNATURE, sizeof(CSMI_SAS_SIGNATURE));
                            memcpy_s(&scsiAddressBuffer->bSASAddress, sizeof(scsiAddressBuffer->bSASAddress), &drive->bSASAddress, sizeof(drive->bSASAddress));
                            memcpy_s(&scsiAddressBuffer->bSASLun, sizeof(scsiAddressBuffer->bSASLun), &drive->bSASLun, sizeof(drive->bSASLun));
                            scsiAddressBuffer->bHostIndex = 0;
                            scsiAddressBuffer->bPathId = 0;
                            scsiAddressBuffer->bTargetId = 0;
                            scsiAddressBuffer->bLun = 0;
                            if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer2, sizeof(buffer2), &buffer2, sizeof(buffer2), &bytesReturned, NULL) && bytesReturned > 0)
                            {
                                raidDrive.insert(cdi::attr::Attribute((BYTE*)&scsiAddressBuffer->bPathId, sizeof(UCHAR), "ScsiPathId", "SCSI Path Id. Often points to a specific device behind the host bus (SCSI) adapter.", std::to_string(scsiAddressBuffer->bPathId)));

                                raidDrive.insert(cdi::attr::Attribute((BYTE*)&scsiAddressBuffer->bLun, sizeof(UCHAR), "ScsiLun", "SCSI Logical Unit Number. Often points to a specific device handler behind a specific path id and target id behind the host bus (SCSI) adapter.", std::to_string(scsiAddressBuffer->bLun)));

                                raidDrive.insert(cdi::attr::Attribute((BYTE*)&scsiAddressBuffer->bTargetId, sizeof(UCHAR), "ScsiTargetId", "SCSI Target Id. Often points to a specific device handler behind a specific path id behind the host bus (SCSI) adapter.", std::to_string(scsiAddressBuffer->bTargetId)));
                            }

                            retVec.push_back(raidDrive);
                            drive++;
                        }
                    }

                    CloseHandle(handle);
                }

                return retVec;
            }

            void addControllerCSMIAttributes(HANDLE &handle, cdi::attr::AttributeSet &attributeSet)
            {
                BYTE buffer[8192] = { 0 };

                PCSMI_SAS_DRIVER_INFO_BUFFER driverInfo = (PCSMI_SAS_DRIVER_INFO_BUFFER)buffer;
                driverInfo->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                driverInfo->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                driverInfo->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_DRIVER_INFO;
                driverInfo->IoctlHeader.Length = sizeof(buffer) - sizeof(IOCTL_HEADER);
                memcpy_s(&driverInfo->IoctlHeader.Signature, sizeof(driverInfo->IoctlHeader.Signature), CSMI_ALL_SIGNATURE, sizeof(CSMI_ALL_SIGNATURE));

                DWORD bytesReturned = 0;
                if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer, sizeof(buffer), &buffer, sizeof(buffer), &bytesReturned, NULL) && bytesReturned > 0 && driverInfo->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                {
                    attributeSet.insert(cdi::attr::Attribute("CSMIDriverName", "The name of the driver managing the underlying CSMI calls for the RAID controller.", (char*)driverInfo->Information.szName));
                    attributeSet.insert(cdi::attr::Attribute("CSMIDriverDescription", "A description for the driver managing the underlying CSMI calls for the RAID controller.", (char*)driverInfo->Information.szDescription));
                    std::string version = std::to_string(driverInfo->Information.usMajorRevision) + "." + std::to_string(driverInfo->Information.usMinorRevision) + "." + std::to_string(driverInfo->Information.usBuildRevision) + "." + std::to_string(driverInfo->Information.usReleaseRevision);
                    attributeSet.insert(cdi::attr::Attribute("CSMIDriverVersion", "The version for the driver managing the underlying CSMI calls for the RAID controller.", version));
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&driverInfo->Information.usCSMIMajorRevision, sizeof(driverInfo->Information.usCSMIMajorRevision), "CSMIMajorRevision", "The major revision of the CSMI specification this driver adheres to.", std::to_string(driverInfo->Information.usCSMIMajorRevision)));
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&driverInfo->Information.usCSMIMinorRevision, sizeof(driverInfo->Information.usCSMIMinorRevision), "CSMIMinorRevision", "The minor revision of the CSMI specification this driver adheres to.", std::to_string(driverInfo->Information.usCSMIMinorRevision)));
                }

                PCSMI_SAS_CNTLR_CONFIG_BUFFER controllerConfig = (PCSMI_SAS_CNTLR_CONFIG_BUFFER)buffer;
                controllerConfig->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                controllerConfig->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                controllerConfig->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_CNTLR_CONFIG;
                controllerConfig->IoctlHeader.Length = sizeof(buffer) - sizeof(IOCTL_HEADER);
                memcpy_s(&controllerConfig->IoctlHeader.Signature, sizeof(controllerConfig->IoctlHeader.Signature), CSMI_ALL_SIGNATURE, sizeof(CSMI_ALL_SIGNATURE));

                if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer, sizeof(buffer), &buffer, sizeof(buffer), &bytesReturned, NULL) && bytesReturned > 0 && driverInfo->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                {
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerConfig->Configuration.uBoardID, sizeof(controllerConfig->Configuration.uBoardID), "SubsystemId", "32-bit subsystem ID from the controller’s PCI configuration space.Bits 0 – 15 contain the subsystem vendor ID and bits 16 – 31 contain the subsystem ID as defined by the PCI specification.", std::to_string(controllerConfig->Configuration.uBoardID)));

                    if (controllerConfig->Configuration.usSlotNumber != SLOT_NUMBER_UNKNOWN)
                    {
                        attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerConfig->Configuration.usSlotNumber, sizeof(controllerConfig->Configuration.usSlotNumber), "PhysicalSlotNumber", "The physical slot number for the controller.", std::to_string(controllerConfig->Configuration.usSlotNumber)));
                    }

                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerConfig->Configuration.bIoBusType, sizeof(controllerConfig->Configuration.bIoBusType), "IOBusType", "The physical slot number for the controller.", cdi::strings::csmiIoBusTypeToString(controllerConfig->Configuration.bIoBusType)));

                    // This isn't perfect, but if the first 8 characters are 0... its probably an invalid serial.
                    if (*(UINT64*)controllerConfig->Configuration.szSerialNumber != 0)
                    {
                        attributeSet.insert(cdi::attr::Attribute("ControllerSerialNumber", "The serial number for the controller.", (char*)controllerConfig->Configuration.szSerialNumber));
                    }

                    std::string version = std::to_string(controllerConfig->Configuration.usMajorRevision) + "." + std::to_string(controllerConfig->Configuration.usMinorRevision) + "." + std::to_string(controllerConfig->Configuration.usBuildRevision) + "." + std::to_string(controllerConfig->Configuration.usReleaseRevision);
                    attributeSet.insert(cdi::attr::Attribute("ControllerProductRevision", "A unique descriptor for the host software on the controller. Sometimes known as Firmware.", version));
                
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerConfig->Configuration.uControllerFlags, sizeof(controllerConfig->Configuration.uControllerFlags), "ControllerFlags", "Flags designating the behavior of the controller.", cdi::strings::csmiControllerFlagsToString(controllerConfig->Configuration.bIoBusType)));
                }

                // todo: Still have at least CC_CSMI_SAS_GET_CNTLR_STATUS 
            }
        }
    }
}