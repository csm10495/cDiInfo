/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Source.cpp - Main source implementation file
*/

// Local includes
#include "Source.h"

// Prints usage information
void printUsage(int argc, char** argv)
{
    std::cout << "cDiInfo: A program that gets information using the SetupDi... WinApi calls." << std::endl;
    std::cout << "Usage: <cDiInfo> [-a | -keys | -enumerators | -classes | -disable | -enable | -restart | -get | -getAll | -getAllJust | -getAllWithout] [<key> <value> (other,keys)]" << std::endl;
    std::cout << "Key and value used if applying to a specific device" << std::endl;
    std::cout << "You gave " << argc << " arg(s)." << std::endl;
}

// Main entry point
int main(int argc, char** argv)
{
    int returnCode = EXIT_SUCCESS;
    if (argc == 2 && std::string(argv[1]) == "-a")
    {
        printAllInfo();
    }
    else if (argc == 2 && std::string(argv[1]) == "-keys")
    {
        std::vector<std::string> &keys = getSampleAttributeKeys();
        printVectorOfStrings(keys, "Possible Attribute Map Keys: (Remember, not all devices have all of these)", false);
    }
    else if (argc == 2 && std::string(argv[1]) == "-enumerators")
    {
        std::vector<std::string> &enumerators = getEnumerators();
        printVectorOfStrings(enumerators, "Enumerators:", false);
    }
    else if (argc == 2 && std::string(argv[1]) == "-classes")
    {
        std::vector<std::string> &classes = getClasses();
        printVectorOfStrings(classes, "Classes:", false);
    }
    else if (argc == 4 && std::string(argv[1]) == "-disable")
    {
        DEVINST devInst = getDevInstWith(argv[2], argv[3]);
        STATUS s = disableDevice(devInst);
        std::cout << getStatusStr(s) << std::endl;
    }
    else if (argc == 4 && std::string(argv[1]) == "-enable")
    {
        DEVINST devInst = getDevInstWith(argv[2], argv[3]);
        STATUS s = enableDevice(devInst);
        std::cout << getStatusStr(s) << std::endl;
    }
    else if (argc == 4 && std::string(argv[1]) == "-restart")
    {
        DEVINST devInst = getDevInstWith(argv[2], argv[3]);
        STATUS s = disableDevice(devInst);
        if (s == STATUS::SUCCESS)
        {
            s = enableDevice(devInst);
        }
        std::cout << getStatusStr(s) << std::endl;
    }
    else if (argc == 4 && std::string(argv[1]) == "-get")
    {
        AttributeMap &deviceAttrMap = getAttributeMapWith(argv[2], argv[3]);
        printAttributeMap(deviceAttrMap);
    }
    else if (argc == 4 && std::string(argv[1]) == "-getAll")
    {
        std::vector<AttributeMap> &attributeMaps = getAttributeMapsWith(argv[2], argv[3]);
        for (auto i : attributeMaps)
        {
            printAttributeMap(i);
        }
    }
    else if (argc >= 4 && std::string(argv[1]) == "-getAllJust")
    {
        // This is comma delimited
        std::string otherAttributes = "";
        if (argc > 4)
        {
            otherAttributes = argv[4];
            printf("Attributes of %s that match %s and keys that match: %s\n", argv[2], argv[3], otherAttributes.c_str());
        }
        else
        {
            printf("Attributes of %s that match %s\n", argv[2], argv[3]);
        }

        std::vector<AttributeMap> &matchingAttributeMaps = getAttributesWith(argv[2], argv[3], otherAttributes);
        for (auto i : matchingAttributeMaps)
        {
            printAttributeMap(i);
        }
    }
    else if (argc == 4 && std::string(argv[1]) == "-getAllWithout")
    {
        std::vector<AttributeMap> &attributeMaps = getAttributeMapsWithout(argv[2], argv[3]);
        for (auto i : attributeMaps)
        {
            printAttributeMap(i);
        }
    }
    else
    {
        printUsage(argc, argv);
    }

    return returnCode;
}