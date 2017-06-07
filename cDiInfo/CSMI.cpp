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
#include "ata.h"

#pragma pack(push,1)
// Some ATA stuff that should possibly be elsewhere...
typedef struct FIS_48_BIT
{
    UINT8 FISType; //Needs to be 0x27
    UINT8 Reserved0 : 7;
    UINT8 IsCommandRegister : 1;

    union
    {
        UINT8 Command;
        union
        {
            UINT8 Status;
            struct
            {
                UINT8 ERR : 1;
                UINT8 IDX : 1;
                UINT8 CORR : 1;
                UINT8 DRQ : 1;
                UINT8 DSC : 1;
                UINT8 DWF : 1;
                UINT8 DRDY : 1;
                UINT8 BSY : 1;
            };
        };
    };

    UINT8 Feature;

    UINT8 LBALow;
    UINT8 LBAMid;
    UINT8 LBAHigh;
    UINT8 Device;

    UINT8 LBAExtLow;
    UINT8 LBAExtMid;
    UINT8 LBAExtHigh;
    UINT8 FeatureExt;

    UINT8 Count;
    UINT8 CountExt;
    UINT8 Reserved1;
    UINT8 Control;

    UINT32 ReservedEnd;

}FIS_48_BIT, *PFIS_48_BIT;
#pragma pack(pop)

#define DEFAULT_DEVICE_REGISTER   0xA0
#define DEFAULT_SECTOR_SIZE       512
#define FIS_HOST_TO_DEVICE_48_BIT 0x27

namespace cdi
{
    namespace detection
    {
        namespace csmi
        {
            cdi::attr::AttributeSetVector getCsmiDevices(std::string devicePath, cdi::attr::AttributeSet &adapterDeviceAttributeSet)
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
                    DWORD bytesReturned = 0;

                    std::map<UCHAR, CSMI_SAS_PHY_ENTITY> idToPhyEntries = getIdToPhyEntries(handle);
                    size_t s = idToPhyEntries.size();

                    //Get number of RAID sets
                    PCSMI_SAS_RAID_INFO_BUFFER raidInfo = (PCSMI_SAS_RAID_INFO_BUFFER)buffer;
                    raidInfo->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                    raidInfo->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                    raidInfo->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_RAID_INFO;
                    raidInfo->IoctlHeader.Length = sizeof(buffer) - sizeof(IOCTL_HEADER);
                    memcpy_s(&raidInfo->IoctlHeader.Signature, sizeof(raidInfo->IoctlHeader.Signature), CSMI_RAID_SIGNATURE, sizeof(CSMI_RAID_SIGNATURE));
                    // get number of raid sets before attempting to get information
                    if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer, sizeof(buffer), &buffer, sizeof(buffer), &bytesReturned, NULL) && bytesReturned > 0 && raidInfo->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                    {
                        // at this point there is definitely some CSMI going on here...
                        adapterDeviceAttributeSet.insert(cdi::attr::Attribute((BYTE*)&s, sizeof(idToPhyEntries.size()), "CsmiPhyEntries", "Devices discovered as Phy Entries behind this CSMI RAID controller.", std::to_string(idToPhyEntries.size())));
                        adapterDeviceAttributeSet.insert(cdi::attr::Attribute((BYTE*)&raidInfo->Information.uNumRaidSets, sizeof(raidInfo->Information.uNumRaidSets), "NumberOfRaidSets", "Current number of RAID arrays behind this CSMI RAID controller.", std::to_string(raidInfo->Information.uNumRaidSets)));
                        adapterDeviceAttributeSet.insert(cdi::attr::Attribute((BYTE*)&raidInfo->Information.uMaxDrivesPerSet, sizeof(raidInfo->Information.uMaxDrivesPerSet), "MaxDrivesPerRaidSet", "Maximum number of drives per RAID array behind this CSMI RAID controller.", std::to_string(raidInfo->Information.uMaxDrivesPerSet)));

                        // Highest possible number of raid arrays on per controller
                        for (UINT8 raidSetNum = 0; raidSetNum < raidInfo->Information.uNumRaidSets; raidSetNum++)
                        {
                            // Get the RAID Configuration... which should include basic information on each 
                            PCSMI_SAS_RAID_CONFIG_BUFFER raidConfigBuffer = (PCSMI_SAS_RAID_CONFIG_BUFFER)buffer;
                            raidConfigBuffer->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                            raidConfigBuffer->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                            raidConfigBuffer->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_RAID_CONFIG;
                            raidConfigBuffer->IoctlHeader.Length = sizeof(buffer) - sizeof(IOCTL_HEADER);
                            memcpy_s(&raidConfigBuffer->IoctlHeader.Signature, sizeof(raidConfigBuffer->IoctlHeader.Signature), CSMI_RAID_SIGNATURE, sizeof(CSMI_RAID_SIGNATURE));
                            raidConfigBuffer->Configuration.uRaidSetIndex = raidSetNum;
                            if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer, sizeof(buffer), &buffer, sizeof(buffer), &bytesReturned, NULL) && bytesReturned > 0)
                            {
                                PCSMI_SAS_RAID_DRIVES drive = raidConfigBuffer->Configuration.Drives;
                                for (size_t i = 0; i < raidConfigBuffer->Configuration.bDriveCount; i++)
                                {
                                    cdi::attr::AttributeSet raidDrive;
                                    raidDrive.insert(cdi::attr::Attribute("ProductId", "A unique descriptor for this particular product from this manufacturer.", std::string((char*)drive->bModel, sizeof(drive->bModel))));
                                    raidDrive.insert(cdi::attr::Attribute("SerialNumber", "A unique descriptor for this particular device from the manufacturer.", std::string((char*)drive->bSerialNumber, sizeof(drive->bSerialNumber))));
                                    raidDrive.insert(cdi::attr::Attribute("ProductRevision", "A unique descriptor for the host software on this device. Sometimes known as Firmware.", std::string((char*)drive->bFirmware, sizeof(drive->bFirmware))));
                                    raidDrive.insert(cdi::attr::Attribute("AdapterDevicePath", "The device path for the device's adapter.", devicePath));
                                    raidDrive.insert(cdi::attr::Attribute((BYTE*)&drive->bDriveStatus, sizeof(drive->bDriveStatus), "DriveStatus", "A status for the drive based on the current RAID.", cdi::strings::csmiDriveStatusToString(drive->bDriveStatus)));
                                    raidDrive.insert(cdi::attr::Attribute((BYTE*)&drive->bDriveUsage, sizeof(drive->bDriveUsage), "DriveUsage", "A descriptor of how the drive is being used in terms of RAID.", cdi::strings::csmiDriveUsageToString(drive->bDriveUsage)));
                                    raidDrive.insert(cdi::attr::Attribute((BYTE*)&drive->bDriveType, sizeof(drive->bDriveType), "DriveType", "A descriptor of the type of drive this is.", cdi::strings::csmiDriveTypeToString(drive->bDriveType)));

                                    std::string pseudoDeviceId = "CSMI\\RAID_DISK\\" + cdi::strings::rTrim(std::string((char*)drive->bModel, sizeof(drive->bModel))) + \
                                        cdi::strings::rTrim(std::string((char*)drive->bFirmware, sizeof(drive->bFirmware))) + cdi::strings::rTrim(std::string((char*)drive->bSerialNumber, sizeof(drive->bSerialNumber))) + \
                                        "&" + std::to_string(raidSetNum) + "&" + std::to_string(i);

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

                                    // idToPhyEntries should really have this, though check just in case so not to error.
                                    if (idToPhyEntries.find(drive->bSASAddress[2]) != idToPhyEntries.end())
                                    {
                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&idToPhyEntries[drive->bSASAddress[2]].Attached.bPhyIdentifier, sizeof(idToPhyEntries[drive->bSASAddress[2]].Attached.bPhyIdentifier), "PhyIdentifier", "Specifies the Phy used to make requests.", std::to_string(idToPhyEntries[drive->bSASAddress[2]].Attached.bPhyIdentifier)));
                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&idToPhyEntries[drive->bSASAddress[2]].bPortIdentifier, sizeof(idToPhyEntries[drive->bSASAddress[2]].bPortIdentifier), "PortIdentifier", "Specifies the Port used to issue requests.", std::to_string(idToPhyEntries[drive->bSASAddress[2]].bPortIdentifier)));

                                        // Send ATA Command
                                        PCSMI_SAS_STP_PASSTHRU_BUFFER passthruBuffer = (PCSMI_SAS_STP_PASSTHRU_BUFFER)buffer2;
                                        memset(&buffer2, 0, sizeof(buffer2));
                                        passthruBuffer->IoctlHeader.ControlCode = CC_CSMI_SAS_STP_PASSTHRU;
                                        passthruBuffer->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                                        passthruBuffer->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                                        passthruBuffer->IoctlHeader.Length = sizeof(buffer2) - sizeof(IOCTL_HEADER);
                                        memcpy_s(&passthruBuffer->IoctlHeader.Signature, sizeof(passthruBuffer->IoctlHeader.Signature), CSMI_SAS_SIGNATURE, sizeof(CSMI_SAS_SIGNATURE));

                                        // Not sure why the 2's but it's the only way it works.
                                        passthruBuffer->Parameters.bDestinationSASAddress[2] = drive->bSASAddress[2];
                                        passthruBuffer->Parameters.bPhyIdentifier = idToPhyEntries[drive->bSASAddress[2]].Attached.bPhyIdentifier;
                                        passthruBuffer->Parameters.bPortIdentifier = idToPhyEntries[drive->bSASAddress[2]].bPortIdentifier;
                                        passthruBuffer->Parameters.bConnectionRate = CSMI_SAS_LINK_RATE_NEGOTIATED;
                                        passthruBuffer->Parameters.uFlags = CSMI_SAS_STP_PIO | CSMI_SAS_STP_READ;
                                        passthruBuffer->Parameters.uDataLength = sizeof(buffer2) - sizeof(CSMI_SAS_STP_PASSTHRU_BUFFER);

                                        PFIS_48_BIT fis = (PFIS_48_BIT)passthruBuffer->Parameters.bCommandFIS;
                                        fis->FISType = FIS_HOST_TO_DEVICE_48_BIT;
                                        fis->IsCommandRegister = true;
                                        fis->Command = SMART_CMD;
                                        fis->Feature = READ_THRESHOLDS;
                                        fis->LBAMid = SMART_CYL_LOW;
                                        fis->LBAHigh = SMART_CYL_HI;
                                        fis->Device = DEFAULT_DEVICE_REGISTER;
                                        PFIS_48_BIT status = (PFIS_48_BIT)passthruBuffer->Status.bStatusFIS;

                                        // Read thresholds first, if that passes read attributes
                                        if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer2, sizeof(buffer2), &buffer2, sizeof(buffer2), &bytesReturned, NULL) && bytesReturned > 0 && passthruBuffer->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                                        {
                                            // Check if the ATA command passed completely
                                            if (!status->ERR)
                                            {
                                                BYTE thresholds[READ_THRESHOLD_BUFFER_SIZE] = { 0 };
                                                memcpy_s(&thresholds, sizeof(thresholds), passthruBuffer->bDataBuffer, READ_THRESHOLD_BUFFER_SIZE);
                                                fis->Feature = READ_ATTRIBUTES;

                                                // Read attributes
                                                if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer2, sizeof(buffer2), &buffer2, sizeof(buffer2), &bytesReturned, NULL) && bytesReturned > 0 && passthruBuffer->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                                                {
                                                    if (!status->ERR)
                                                    {
                                                        std::string SMARTData = cdi::strings::smartToString((BYTE*)&passthruBuffer->bDataBuffer, READ_THRESHOLD_BUFFER_SIZE, thresholds);
                                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&passthruBuffer->bDataBuffer, READ_THRESHOLD_BUFFER_SIZE, "SMARTData", "Self-Monitoring and Reporting Technology (SMART) data. Used to diagnose the state and potential for failure of a device.", SMARTData));
                                                    }
                                                }
                                            }
                                        }

                                        // Read SMART Return Status
                                        fis->Feature = RETURN_SMART_STATUS;
                                        if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer2, sizeof(buffer2), &buffer2, sizeof(buffer2), &bytesReturned, NULL) && bytesReturned > 0 && passthruBuffer->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                                        {
                                            if (!status->ERR)
                                            {
                                                std::string SMARTReturnStatus = "Healthy";
                                                if (status->LBAMid == BAD_SMART_LOW && status->LBAHigh == BAD_SMART_HIGH)
                                                {
                                                    SMARTReturnStatus = "Threshold Exceeded Condition";
                                                }
                                                raidDrive.insert(cdi::attr::Attribute((BYTE*)&status, sizeof(status), "SMARTReturnStatus", "Status determined via SMART metrics.", SMARTReturnStatus));
                                            }
                                        }

                                        // Identify Device... to play with some info
                                        fis->Command = 0xEC;
                                        fis->Feature = 0;
                                        fis->LBAMid = 0;
                                        fis->LBAHigh = 0;

                                        if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer2, sizeof(buffer2), &buffer2, sizeof(buffer2), &bytesReturned, NULL) && bytesReturned > 0 && passthruBuffer->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                                        {
                                            if (!status->ERR)
                                            {
                                                // todo: possibly move this to a different function so that in the future non CSMI can use it as well. (Decoding of Identify Device Data)
                                                PIDENTIFY_DEVICE_DATA identifyDevice = (PIDENTIFY_DEVICE_DATA)passthruBuffer->bDataBuffer;

                                                // Byte per (logical) sector
                                                DWORD wordsPerLogicalSector = *(DWORD*)&identifyDevice->WordsPerLogicalSector;
                                                DWORD blockSize = DEFAULT_SECTOR_SIZE;
                                                if ((wordsPerLogicalSector >> 12) & 1) // shall be valid when bit 12 is set to 1
                                                {
                                                    blockSize = wordsPerLogicalSector * sizeof(WORD);
                                                    raidDrive.insert(cdi::attr::Attribute((BYTE*)&wordsPerLogicalSector, sizeof(DWORD), "BytesPerSector", "The number of bytes in a sector on the specified volume. This value was translated from Words in the ATA Identify Device response block. ", std::to_string(blockSize)));
                                                }
                                                else
                                                {
                                                    raidDrive.insert(cdi::attr::Attribute((BYTE*)&wordsPerLogicalSector, sizeof(DWORD), "BytesPerSector", "The number of bytes in a sector on the specified volume. This value was translated from Words in the ATA Identify Device response block. ", std::to_string(DEFAULT_SECTOR_SIZE)));
                                                }

                                                // Get size of the drive...
                                                UINT64 diskLength = (*(UINT64*)&identifyDevice->Max48BitLBA * blockSize);
                                                raidDrive.insert(cdi::attr::Attribute((BYTE*)&diskLength, sizeof(diskLength), "DiskLength", "The size of the disk in bytes.", cdi::strings::diskLengthToString(diskLength)));
                                                raidDrive.insert(cdi::attr::Attribute((BYTE*)&identifyDevice->Max48BitLBA, sizeof(UINT64), "NumberOfBlocks", "The number of blocks on the storage disk.", std::to_string(*(UINT64*)&identifyDevice->Max48BitLBA)));
                                            }
                                        }

                                        // Some stuff from the PHY Entity
                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&idToPhyEntries[drive->bSASAddress[2]].bMaximumLinkRate, sizeof(idToPhyEntries[drive->bSASAddress[2]].bMaximumLinkRate), "MaximumLinkRate", "Maximum rate for the link connection.", cdi::strings::linkRateToString(idToPhyEntries[drive->bSASAddress[2]].bMaximumLinkRate)));
                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&idToPhyEntries[drive->bSASAddress[2]].bMinimumLinkRate, sizeof(idToPhyEntries[drive->bSASAddress[2]].bMinimumLinkRate), "MinimumLinkRate", "Minimum rate for the link connection.", cdi::strings::linkRateToString(idToPhyEntries[drive->bSASAddress[2]].bMinimumLinkRate)));
                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&idToPhyEntries[drive->bSASAddress[2]].bNegotiatedLinkRate, sizeof(idToPhyEntries[drive->bSASAddress[2]].bNegotiatedLinkRate), "NegotiatedLinkRate", "Negotiated (and current) rate for the link connection.", cdi::strings::linkRateToString(idToPhyEntries[drive->bSASAddress[2]].bNegotiatedLinkRate)));
                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&idToPhyEntries[drive->bSASAddress[2]].bPhyChangeCount, sizeof(idToPhyEntries[drive->bSASAddress[2]].bPhyChangeCount), "PhyChangeCount", "The current count of BROADCAST(CHANGE) primitives received on this phy.", std::to_string(idToPhyEntries[drive->bSASAddress[2]].bPhyChangeCount)));
                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&idToPhyEntries[drive->bSASAddress[2]].Attached.bDeviceType, sizeof(idToPhyEntries[drive->bSASAddress[2]].Attached.bDeviceType), "CsmiDeviceType", "Device type as reported by CSMI.", cdi::strings::csmiDeviceTypeToString(idToPhyEntries[drive->bSASAddress[2]].Attached.bDeviceType)));
                                        raidDrive.insert(cdi::attr::Attribute((BYTE*)&idToPhyEntries[drive->bSASAddress[2]].Attached.bTargetPortProtocol, sizeof(idToPhyEntries[drive->bSASAddress[2]].Attached.bTargetPortProtocol), "TargetPortProtocol", "Protocol for the target port.", cdi::strings::csmiTargetProtocolToString(idToPhyEntries[drive->bSASAddress[2]].Attached.bTargetPortProtocol)));

                                    }
                                    retVec.push_back(raidDrive);
                                    drive++;
                                }
                            }
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
                    attributeSet.insert(cdi::attr::Attribute("CsmiDriverName", "The name of the driver managing the underlying CSMI calls for the RAID controller.", (char*)driverInfo->Information.szName));
                    attributeSet.insert(cdi::attr::Attribute("CsmiDriverDescription", "A description for the driver managing the underlying CSMI calls for the RAID controller.", (char*)driverInfo->Information.szDescription));
                    std::string version = std::to_string(driverInfo->Information.usMajorRevision) + "." + std::to_string(driverInfo->Information.usMinorRevision) + "." + std::to_string(driverInfo->Information.usBuildRevision) + "." + std::to_string(driverInfo->Information.usReleaseRevision);
                    attributeSet.insert(cdi::attr::Attribute("CsmiDriverVersion", "The version for the driver managing the underlying CSMI calls for the RAID controller.", version));
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&driverInfo->Information.usCSMIMajorRevision, sizeof(driverInfo->Information.usCSMIMajorRevision), "CsmiMajorRevision", "The major revision of the CSMI specification this driver adheres to.", std::to_string(driverInfo->Information.usCSMIMajorRevision)));
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&driverInfo->Information.usCSMIMinorRevision, sizeof(driverInfo->Information.usCSMIMinorRevision), "CsmiMinorRevision", "The minor revision of the CSMI specification this driver adheres to.", std::to_string(driverInfo->Information.usCSMIMinorRevision)));
                }

                PCSMI_SAS_CNTLR_CONFIG_BUFFER controllerConfig = (PCSMI_SAS_CNTLR_CONFIG_BUFFER)buffer;
                controllerConfig->IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                controllerConfig->IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                controllerConfig->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_CNTLR_CONFIG;
                controllerConfig->IoctlHeader.Length = sizeof(buffer) - sizeof(IOCTL_HEADER);
                memcpy_s(&controllerConfig->IoctlHeader.Signature, sizeof(controllerConfig->IoctlHeader.Signature), CSMI_ALL_SIGNATURE, sizeof(CSMI_ALL_SIGNATURE));

                if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer, sizeof(buffer), &buffer, sizeof(buffer), &bytesReturned, NULL) && bytesReturned > 0 && driverInfo->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                {
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerConfig->Configuration.uBoardID, sizeof(controllerConfig->Configuration.uBoardID), "SubsystemId", "32-bit subsystem ID from the controller's PCI configuration space. Bits 0 - 15 contain the subsystem vendor ID and bits 16 - 31 contain the subsystem ID as defined by the PCI specification.", std::to_string(controllerConfig->Configuration.uBoardID)));

                    if (controllerConfig->Configuration.usSlotNumber != SLOT_NUMBER_UNKNOWN)
                    {
                        attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerConfig->Configuration.usSlotNumber, sizeof(controllerConfig->Configuration.usSlotNumber), "PhysicalSlotNumber", "The physical slot number for the controller.", std::to_string(controllerConfig->Configuration.usSlotNumber)));
                    }

                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerConfig->Configuration.bIoBusType, sizeof(controllerConfig->Configuration.bIoBusType), "IoBusType", "The physical slot number for the controller.", cdi::strings::csmiIoBusTypeToString(controllerConfig->Configuration.bIoBusType)));

                    // This isn't perfect, but if the first 8 characters are 0... its probably an invalid serial.
                    if (*(UINT64*)controllerConfig->Configuration.szSerialNumber != 0)
                    {
                        attributeSet.insert(cdi::attr::Attribute("ControllerSerialNumber", "The serial number for the controller.", (char*)controllerConfig->Configuration.szSerialNumber));
                    }

                    std::string version = std::to_string(controllerConfig->Configuration.usMajorRevision) + "." + std::to_string(controllerConfig->Configuration.usMinorRevision) + "." + std::to_string(controllerConfig->Configuration.usBuildRevision) + "." + std::to_string(controllerConfig->Configuration.usReleaseRevision);
                    attributeSet.insert(cdi::attr::Attribute("ControllerProductRevision", "A unique descriptor for the host software on the controller. Sometimes known as Firmware.", version));
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerConfig->Configuration.uControllerFlags, sizeof(controllerConfig->Configuration.uControllerFlags), "ControllerFlags", "Flags designating the behavior of the controller.", cdi::strings::csmiControllerFlagsToString(controllerConfig->Configuration.bIoBusType)));
                }

                controllerConfig->IoctlHeader.ControlCode = CC_CSMI_SAS_GET_CNTLR_STATUS;
                if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &buffer, sizeof(buffer), &buffer, sizeof(buffer), &bytesReturned, NULL) && bytesReturned > 0 && driverInfo->IoctlHeader.ReturnCode == CSMI_SAS_STATUS_SUCCESS)
                {
                    PCSMI_SAS_CNTLR_STATUS_BUFFER controllerStatus = (PCSMI_SAS_CNTLR_STATUS_BUFFER)buffer;
                    attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerStatus->Status.uStatus, sizeof(controllerStatus->Status.uStatus), "ControllerStatus", "The status of the controller.", cdi::strings::csmiControllerStatusToString(controllerStatus->Status.uStatus)));
                    if (controllerStatus->Status.uStatus != CSMI_SAS_CNTLR_STATUS_GOOD)
                    {
                        attributeSet.insert(cdi::attr::Attribute((BYTE*)&controllerStatus->Status.uOfflineReason, sizeof(controllerStatus->Status.uOfflineReason), "ControllerOfflineReason", "The reason for the controller to be offline.", cdi::strings::csmiOfflineReasonToString(controllerStatus->Status.uOfflineReason)));
                    }
                }
            }

            std::map<UCHAR, CSMI_SAS_PHY_ENTITY> getIdToPhyEntries(HANDLE &handle)
            {
                std::map<UCHAR, CSMI_SAS_PHY_ENTITY> idToPhyEntries;

                CSMI_SAS_PHY_INFO_BUFFER phyInfo = { 0 };
                phyInfo.IoctlHeader.ControlCode = CC_CSMI_SAS_GET_PHY_INFO;
                phyInfo.IoctlHeader.HeaderLength = sizeof(IOCTL_HEADER);
                phyInfo.IoctlHeader.Timeout = CSMI_SAS_TIMEOUT;
                phyInfo.IoctlHeader.Length = sizeof(phyInfo) - sizeof(IOCTL_HEADER);
                memcpy_s(&phyInfo.IoctlHeader.Signature, sizeof(phyInfo.IoctlHeader.Signature), CSMI_SAS_SIGNATURE, sizeof(CSMI_SAS_SIGNATURE));
                DWORD bytesReturned = 0;
                if (DeviceIoControl(handle, IOCTL_SCSI_MINIPORT, &phyInfo, sizeof(phyInfo), &phyInfo, sizeof(phyInfo), &bytesReturned, NULL) && bytesReturned > 0)
                {
                    for (size_t i = 0; i < phyInfo.Information.bNumberOfPhys; i++)
                    {
                        CSMI_SAS_PHY_ENTITY entity;
                        memcpy_s(&entity, sizeof(entity), &phyInfo.Information.Phy[i], sizeof(phyInfo.Information.Phy[i]));

                        // Can't explain this... but bSASAddress[2] seems to correspond with the SAS address from RAID above
                        idToPhyEntries[entity.Attached.bSASAddress[2]] = entity;
                    }
                }

                return idToPhyEntries;
            }
        }
    }
}