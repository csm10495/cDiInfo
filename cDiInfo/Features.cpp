/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Features.cpp - Features implementation file
*/

// Local includes
#include "Enumerations.h"
#include "Features.h"

namespace cdi
{
    namespace features
    {
        const std::vector<std::string> sampleAttributeKeys = {
            "DeviceId",
            "DeviceDesc",
            "FriendlyName",
            "HardwareId",
            "Driver",
            "Manufacturer",
            "BusNumber",
            "ChildrenDeviceIds",
            "ParentDeviceId",
            "SiblingDeviceIds",
            "DriverDescription",
            "DriverMfgName",
            "ScsiAdapterPath",
            "ScsiPortNumber",
            "ScsiPathId",
            "ScsiLun",
            "ScsiTargetId",
            "DevicePath",
            "PhysicalDrivePath",
            "MaximumTransferLength",
            "DeviceHotplug",
            "DiskLength",
            "VendorId",
            "ProductRevision",
            "SerialNumber",
        };

        STATUS disableDevice(DEVINST devInst)
        {
            if (devInst == NULL)
            {
                return FAILED_TO_GET_MATCHING_DEVINST;
            }

            DWORD cmRet = CM_Disable_DevNode(devInst, CM_DISABLE_UI_NOT_OK | CM_DISABLE_ABSOLUTE);

            if (cmRet == CR_SUCCESS)
            {
                return SUCCESS;
            }
            else
            {
                std::cerr << "CM_Disable_DevNode(...) returned " << cmRet << " (" << cdi::strings::cmRetValueToString(cmRet) << ")" << std::endl;
                return CM_FAILED;
            }
        }

        STATUS enableDevice(DEVINST devInst)
        {
            if (devInst == NULL)
            {
                return FAILED_TO_GET_MATCHING_DEVINST;
            }

            DWORD cmRet = CM_Enable_DevNode(devInst, 0);

            if (cmRet == CR_SUCCESS)
            {
                return SUCCESS;
            }
            else
            {
                std::cerr << "CM_Enable_DevNode(...) returned " << cmRet << " (" << cdi::strings::cmRetValueToString(cmRet) << ")" << std::endl;
                return CM_FAILED;
            }
        }

        void printAllInfo(bool useXml)
        {
            cdi::attr::AttributeSetVector allDevsAttrSet = getAllDevicesAttributeSet();
            printAttributeSetVector(allDevsAttrSet, useXml);
        }

        void printVectorOfStrings(std::vector<std::string>& vec, std::string title, bool printStars)
        {
            printf("%s\n", title.c_str());
            auto regex = std::regex("\n");
            for (auto i : vec)
            {
                i = std::regex_replace(i, regex, "\n  ");
                printf("%s\n", cdi::strings::rTrim(i).c_str());
                if (printStars)
                {
                    printf("\n*******************\n");
                }
            }
        }

        std::vector<std::string> getSampleAttributeKeys()
        {
            return sampleAttributeKeys;
        }

        std::vector<std::string> getEnumerators()
        {
            std::vector<std::string> enumerators;

            for (ULONG i = 0; ;i++)
            {
                char buf[MAX_DEVICE_ID_LEN] = { '\0' };
                ULONG bufLen = MAX_DEVICE_ID_LEN;
                if (CM_Enumerate_Enumerators(i, buf, &bufLen, 0) != CR_SUCCESS)
                {
                    break;
                }
                enumerators.push_back(std::string(buf, bufLen));
            }

            return enumerators;
        }

        std::vector<std::string> getClasses()
        {
            std::vector<std::string> classes;

            for (ULONG i = 0; ;i++)
            {
                GUID guid;
                if (CM_Enumerate_Classes(i, &guid, 0) != CR_SUCCESS)
                {
                    break;
                }
                classes.push_back(cdi::strings::guidToString(guid));
            }

            return classes;
        }

        cdi::attr::AttributeSetVector getAttributeSetsWith(std::string key, std::string value)
        {
            cdi::attr::AttributeSetVector matchingAttributeSets;

            cdi::attr::AttributeSetVector devicesAttributeSet = getAllDevicesAttributeSet();
            for (auto &deviceAttrSet : devicesAttributeSet)
            {
                auto itr = deviceAttrSet.find(ATTRWITHNAME(key));
                if (itr != deviceAttrSet.end())
                {
                    if (cdi::strings::wildcardMatch(itr->getValue<std::string>(), value, FALSE))
                    {
                        matchingAttributeSets.push_back(deviceAttrSet);
                    }
                }
            }
            return matchingAttributeSets;
        }

        cdi::attr::AttributeSetVector getAttributesWith(std::string key, std::string value, std::string alt)
        {
            cdi::attr::AttributeSetVector devicesAttributeSet = getAllDevicesAttributeSet();
            cdi::attr::AttributeSetVector retAS;

            std::vector<std::string> otherAttributeNames = cdi::strings::split(alt, ',');
            // trim each string
            std::for_each(otherAttributeNames.begin(), otherAttributeNames.end(), cdi::strings::trim);

            for (cdi::attr::AttributeSet &attributeSet : devicesAttributeSet)
            {
                cdi::attr::AttributeSet inAS;

                // make sure this attribute Set has the key... if not continue
                auto itr = attributeSet.find(ATTRWITHNAME(key));
                if (itr != attributeSet.end())
                {
                    if (cdi::strings::wildcardMatch(itr->getValue<std::string>(), value, FALSE))
                    {
                        inAS.insert(*itr);
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }

                // go through each attribute, if it matches one of the otherAttributeNames, add it to the inAS
                for (auto attribute : attributeSet)
                {
                    for (auto attributeName : otherAttributeNames)
                    {
                        if (cdi::strings::wildcardMatch(attribute.getName(), attributeName, FALSE))
                        {
                            inAS.insert(attribute);
                        }
                    }
                }

                retAS.push_back(inAS);
            }

            return retAS;
        }

        cdi::attr::AttributeSetVector getAttributeSetsWithout(std::string key, std::string value)
        {
            cdi::attr::AttributeSetVector matchingAttributeSets;

            cdi::attr::AttributeSetVector devicesAttributeSet = getAllDevicesAttributeSet();
            for (auto &deviceAttrSet : devicesAttributeSet)
            {
                auto itr = deviceAttrSet.find(ATTRWITHNAME(key));
                if (itr != deviceAttrSet.end())
                {
                    if (!cdi::strings::wildcardMatch(itr->getValue<std::string>(), value, FALSE))
                    {
                        matchingAttributeSets.push_back(deviceAttrSet);
                    }
                }
                else
                {
                    matchingAttributeSets.push_back(deviceAttrSet);
                }
            }
            return matchingAttributeSets;
        }

        DEVINST getDevInstWith(std::string key, std::string value)
        {
            cdi::attr::AttributeSet deviceAttrSet = getAttributeSetWith(key, value);
            if (!deviceAttrSet.empty())
            {
                PSP_DEVINFO_DATA pDevInfoData = (PSP_DEVINFO_DATA)deviceAttrSet.find(ATTRWITHNAME(DEVINFO_DATA_STRING))->getValue<BYTE*>();
                return pDevInfoData->DevInst;
            }
            return NULL;
        }

        cdi::attr::AttributeSet getAttributeSetWith(std::string key, std::string value)
        {
            cdi::attr::AttributeSetVector devicesAttributeSet = getAllDevicesAttributeSet();
            for (auto &deviceAttrSet : devicesAttributeSet)
            {
                auto itr = deviceAttrSet.find(ATTRWITHNAME(key));
                if (itr != deviceAttrSet.end())
                {
                    if (cdi::strings::wildcardMatch(itr->getValue<std::string>(), value, FALSE))
                    {
                        return deviceAttrSet;
                    }
                }
            }
            return cdi::attr::AttributeSet();
        }

        void printAttributeSetVector(cdi::attr::AttributeSetVector &attrSetVector, bool useXml)
        {
            if (useXml)
            {
                fprintf(stdout, "%s\n", cdi::attr::toXml(attrSetVector).c_str());
            }
            else
            {
                for (auto i : attrSetVector)
                {
                    printAttributeSet(i, useXml);
                }
            }
        }

        void printAttributeSet(cdi::attr::AttributeSet &attrSet, bool useXml)
        {
            if (useXml)
            {
                fprintf(stdout, "%s\n", cdi::attr::toXml(attrSet).c_str());
            }
            else
            {
                std::regex newlineRegex = std::regex("\n");
                for (auto i : attrSet)
                {
                    // Hide 'private' attributes
                    if (!cdi::strings::startsWith("__", i.getName(), true))
                    {
                        std::string value = i.getValue<std::string>();

                        std::string spaces = "                           ";
                        while (spaces.size() < i.getName().size() + 2)
                        {
                            spaces += " ";
                        }

                        value = std::regex_replace(value, newlineRegex, "\n" + spaces);

                        printf("%-25s: %s\n", i.getName().c_str(), value.c_str());
                    }
                }
                puts("*******************************************************");
            }
        }
    }
}


