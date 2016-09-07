/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Attribute.h - Header file for the generic attribute type
*/
#pragma once

// WinApi Includes
#include <Windows.h>

// STL Includes
#include <memory>
#include <set>
#include <vector>

namespace cdi
{
    namespace attr
    {
        // A generic attribute with a customizeable toString mechanism
        class Attribute
        {
        public:
            // Constructor, takes a BYTE array/length. Also takes name, description, and final parsing
            Attribute(BYTE* bytes, size_t length, std::string name, std::string description, std::string parsing);

            // Constructor for strings. The idea being that bytes/length can be determined from the string parsing
            Attribute(std::string name, std::string description, std::string parsing);

            // Constructor for wide strings. The idea being that bytes/length can be determined from the string parsing
            Attribute(std::string name, std::string description, std::wstring parsing);

            // Empty constructor
            Attribute();

            // Copy constructor
            Attribute(const Attribute &other);

            // Copy constructor, with the ability to add to parsing
            Attribute(const Attribute &other, std::string addStr);

            // Equals operator
            Attribute &operator=(Attribute &other);

            // Destructor, deletes the internal BytesRepresentation
            ~Attribute();

            // Predefine template
            template <class T>
            T getValue();

            // Predefine const template
            template <class T>
            T getValue() const;

            // Gets the name
            std::string getName();

            // Gets the name (const)
            std::string getName() const;

            // Gets the description
            std::string getDescription() const;

            // Gets the length of the internal byte representation
            size_t getLength() const;

        private:

            // The raw bytes representation
            std::unique_ptr<BYTE[]> BytesRepresentation;

            // The length of the byte array
            size_t BytesRepresentationLength;

            // The name of the field
            std::string Name;

            // The description of the field
            std::string Description;

            // The parsing of the field
            std::string Parsing;

            // Used to make an internal deep copy of everything
            static void deepCopy(Attribute &to, const Attribute &from);
        };

        struct AttributeCompare
        {
            bool operator()(const Attribute &lhs, const Attribute &rhs);
            bool operator()(const Attribute &lhs, const Attribute &rhs) const;
        };

        typedef std::set<Attribute, AttributeCompare> AttributeSet;
        typedef std::vector<AttributeSet> AttributeSetVector;

        // Attribute to Xml
        std::string toXml(const Attribute &attr);

        // AttributeSet to Xml
        std::string toXml(const AttributeSet &attrSet);

        // AttributeSetVector to Xml
        std::string toXml(const AttributeSetVector &attrSetVector);
    }
}
