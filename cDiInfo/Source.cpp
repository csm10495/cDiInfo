/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Source.cpp - Main source implementation file
*/

// Local includes
#include "Core.h"
#include "Enumerations.h"
#include "Features.h"
#include "Source.h"
#include "Strings.h"

// Prints usage information
void printUsage(int argc, char** argv)
{
    std::cout << "cDiInfo: A program that gets information using the SetupDi... WinApi calls." << std::endl;
    std::cout << "Usage: <cDiInfo> [-a | -disable | -enable | -get] [<key> <value>]" << std::endl;
    std::cout << "                                                   These used if applying to a specific device" << std::endl;
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
    else if (argc == 4 && std::string(argv[1]) == "-disable")
    {
        STATUS s = disableDevice(argv[2], argv[3]);
        std::cout << getStatusStr(s) << std::endl;
    }
    else if (argc == 4 && std::string(argv[1]) == "-enable")
    {
        STATUS s = enableDevice(argv[2], argv[3]);
        std::cout << getStatusStr(s) << std::endl;
    }
    else if (argc == 4 && std::string(argv[1]) == "-get")
    {
        AttributeMap deviceAttrMap = getAttributeMapWith(argv[2], argv[3]);
        printAttributeMap(deviceAttrMap);
    }
    else
    {
        printUsage(argc, argv);
    }

    return returnCode;
}