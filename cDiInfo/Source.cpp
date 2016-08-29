/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Source.cpp - Main source implementation file
*/

// Local Includes
#include "Core.h"
#include "Enumerations.h"
#include "Features.h"
#include "Registry.h"
#include "Source.h"
#include "Strings.h"

// Prints usage information
void printUsage(int argc, char** argv)
{
    std::cout << "cDiInfo: A program that gets information using the SetupDi... WinApi calls." << std::endl;
    std::cout << "Usage: <cDiInfo> <-xml> [-a | -keys | -enumerators | -classes | -disable | -enable | -restart | -get | -getAll | -getAllJust | -getAllWithout] [<key> <value> (other,keys)]" << std::endl;
    std::cout << "Key and value used if applying to a specific device" << std::endl;
    std::cout << "If -xml is given before another command, the data will be returned as XML if available. Otherwise a human-readable format is given." << std::endl;
    std::cout << "You gave " << argc << " arg(s)." << std::endl;
}

// Main entry point
int main(int argc, char** argv)
{
    int returnCode = EXIT_SUCCESS;
    bool useXml = false;

    int arg1 = 1; //first arg (after xml if given)
    int arg2 = 2;
    int arg3 = 3;
    int arg4 = 4; //fourth arg (after xml if given)
    int arg5 = 5; //fifth arg (after xml if given)

    if (argc > 1)
    {
        if (std::string(argv[arg1]) == "-xml")
        {
            useXml = true;
            arg1++;
            arg2++;
            arg3++;
            arg4++;
            arg5++;
        }
    }

    if (argc >= 2 && std::string(argv[arg1]) == "-a")
    {
        cdi::features::printAllInfo(useXml);
    }
    else if (argc >= 2 && std::string(argv[arg1]) == "-keys")
    {
        std::vector<std::string> &keys = cdi::features::getSampleAttributeKeys();
        cdi::features::printVectorOfStrings(keys, "Possible Attribute Map Keys: (Remember, not all devices have all of these)", false);
    }
    else if (argc >= 2 && std::string(argv[arg1]) == "-enumerators")
    {
        std::vector<std::string> &enumerators = cdi::features::getEnumerators();
        cdi::features::printVectorOfStrings(enumerators, "Enumerators:", false);
    }
    else if (argc >= 2 && std::string(argv[arg1]) == "-classes")
    {
        std::vector<std::string> &classes = cdi::features::getClasses();
        cdi::features::printVectorOfStrings(classes, "Classes:", false);
    }
    else if (argc >= 4 && std::string(argv[arg1]) == "-disable")
    {
        DEVINST devInst = cdi::features::getDevInstWith(argv[arg2], argv[arg3]);
        STATUS s = cdi::features::disableDevice(devInst);
        std::cout << cdi::enums::getStatusStr(s) << std::endl;
        if (s != STATUS::SUCCESS)
        {
            returnCode = EXIT_FAILURE;
        }
    }
    else if (argc >= 4 && std::string(argv[arg1]) == "-enable")
    {
        DEVINST devInst = cdi::features::getDevInstWith(argv[arg2], argv[arg3]);
        STATUS s = cdi::features::enableDevice(devInst);
        std::cout << cdi::enums::getStatusStr(s) << std::endl;
        if (s != STATUS::SUCCESS)
        {
            returnCode = EXIT_FAILURE;
        }
    }
    else if (argc >= 4 && std::string(argv[arg1]) == "-restart")
    {
        DEVINST devInst = cdi::features::getDevInstWith(argv[arg2], argv[arg3]);
        STATUS s = cdi::features::disableDevice(devInst);
        if (s == STATUS::SUCCESS)
        {
            s = cdi::features::enableDevice(devInst);
        }
        std::cout << cdi::enums::getStatusStr(s) << std::endl;
        if (s != STATUS::SUCCESS)
        {
            returnCode = EXIT_FAILURE;
        }
    }
    else if (argc >= 4 && std::string(argv[arg1]) == "-get")
    {
        cdi::attr::AttributeSet &deviceAttrSet = cdi::features::getAttributeSetWith(argv[arg2], argv[arg3]);
        cdi::features::printAttributeSet(deviceAttrSet, useXml);
    }
    else if (argc >= 4 && std::string(argv[arg1]) == "-getAll")
    {
        cdi::attr::AttributeSetVector &attributeSets = cdi::features::getAttributeSetsWith(argv[arg2], argv[arg3]);
        cdi::features::printAttributeSetVector(attributeSets, useXml);
    }
    else if (argc >= 4 && std::string(argv[arg1]) == "-getAllJust")
    {
        // This is comma delimited
        std::string otherAttributes = "";
        if ((argc > 6 && useXml) || argc > 4)
        {
            otherAttributes = std::string(argv[arg4]);
            fprintf(stderr, "Getting attributes of %s that match %s and keys that match: %s\n", argv[arg2], argv[arg3], otherAttributes.c_str());
        }
        else
        {
            fprintf(stderr, "Getting attributes of %s that match %s\n", argv[arg2], argv[arg3]);
        }
        cdi::attr::AttributeSetVector &matchingAttributeSets = cdi::features::getAttributesWith(argv[arg2], argv[arg3], otherAttributes);
        cdi::features::printAttributeSetVector(matchingAttributeSets, useXml);
    }
    else if (argc >= 4 && std::string(argv[arg1]) == "-getAllWithout")
    {
        cdi::attr::AttributeSetVector &attributeSets = cdi::features::getAttributeSetsWithout(argv[arg2], argv[arg3]);
        cdi::features::printAttributeSetVector(attributeSets, useXml);
    }
    else
    {
        printUsage(argc, argv);
    }

    return returnCode;
}