/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Attribute.h - Header file for the generic attribute type
*/
#pragma once

// WinApi Includes
#include <Windows.h>

namespace cdi
{
    // A generic attribute with a customizeable toString mechanism
    class Attribute
    {
    public:
        // Constructor, takes a BYTE array/length. Also takes name, description, and final parsing
        Attribute(BYTE* bytes, UINT64 length, std::string name, std::string description, std::string parsing);

        // Cheap constructor without name/description/parsing
        Attribute(BYTE* bytes, UINT64 length);

        // Copy constructor
        Attribute(const Attribute &other);

        // Destructor, deletes the internal BytesRepresentation
        ~Attribute();

        // Predefine template
        template <class T>
        T getValue();

        // Gets the name
        std::string getName();

        // Gets the description
        std::string getDescription();

        // Gets the length of the internal byte representation
        UINT64 getLength();

    private:

        // The raw bytes representation
        BYTE* BytesRepresentation = nullptr;

        // The length of the byte array
        UINT64 BytesRepresentationLength;

        // The name of the field
        std::string Name;

        // The description of the field
        std::string Description;

        // The parsing of the field
        std::string Parsing;

        // Throws an error if BytesRepresentation is nullptr
        void checkForNullptr();
    };
}
