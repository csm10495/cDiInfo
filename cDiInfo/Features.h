/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Features.h - Features header file
*/
#pragma once

// Local includes
#include "Core.h"

// STL includes
#include <vector>

namespace cdi
{
    namespace features
    {
        // Tries to disable a devInst
        STATUS disableDevice(DEVINST devInst);

        // Tries to enable a devInst
        STATUS enableDevice(DEVINST devInst);

        // Prints everything we can get about every device
        void printAllInfo(bool useXml);

        // Prints a vector of strings with a title
        void printVectorOfStrings(std::vector<std::string> &vec, std::string title, bool printStars);

        // Gets a vector of attribute keys (note that this may not be all possible ones)
        std::vector<std::string> getSampleAttributeKeys();

        // Get a vector of enumerators
        std::vector<std::string> getEnumerators();

        // Get a vector of classes
        std::vector<std::string> getClasses();

        // Gets all AttributeSets that have a matching key and value
        cdi::attr::AttributeSetVector getAttributeSetsWith(std::string key, std::string value);

        // Gets all AttributeSets that have a matching key and value
        cdi::attr::AttributeSetVector getAttributesWith(std::string key, std::string value, std::string alt);

        // Gets all AttributeSets that don't have a matching key and value
        cdi::attr::AttributeSetVector getAttributeSetsWithout(std::string key, std::string value);

        // Gets a DEVINST that has a matching key and value in it's AttributeSet
        DEVINST getDevInstWith(std::string key, std::string value);

        // Gets a AttributeSet that has a matching key and value in it's AttributeSet
        cdi::attr::AttributeSet getAttributeSetWith(std::string key, std::string value);

        // Prints a given AttributeSetVector
        void printAttributeSetVector(cdi::attr::AttributeSetVector &attrSetVector, bool useXml);

        // Prints a given AttributeSet
        void printAttributeSet(cdi::attr::AttributeSet &attrSet, bool useXml);
    }
}
