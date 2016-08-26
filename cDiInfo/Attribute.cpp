/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Attribute.cpp - Implementation file for the generic attribute type
*/

// WinApi Includes
#include <Windows.h>

// STL Includes
#include <exception>

// Local Includes
#include "Attribute.h"

// #defines
#define UNNAMED_ATTRIBUTE "Unnamed Attribute"
#define NO_DESCRIPTION    "No Description"
#define NO_PARSING        "No Parsing"

namespace cdi
{
    Attribute::Attribute(BYTE* bytes, UINT64 length, std::string name, std::string description, std::string parsing)
    {
        if (BytesRepresentation != nullptr)
        {
            throw std::exception("Unable to create Attribute because the BytesRepresentation is not pre-set to nullptr");
        }

        BytesRepresentation = new BYTE[length];
        BytesRepresentationLength = length;
        Name = name;
        Description = description;
        Parsing = parsing;
        memcpy_s(BytesRepresentation, length, bytes, length);
    }

    Attribute::Attribute(BYTE* bytes, UINT64 length) : Attribute(bytes, length, UNNAMED_ATTRIBUTE, NO_DESCRIPTION, NO_PARSING)
    {

    }

    Attribute::Attribute(const Attribute &other)
    {
        this->BytesRepresentation = new BYTE[other.BytesRepresentationLength];
        this->BytesRepresentationLength = other.BytesRepresentationLength;
        this->Description = other.Description;
        this->Name = other.Name;
        this->Parsing = other.Parsing;

        memcpy_s(this->BytesRepresentation, this->BytesRepresentationLength, other.BytesRepresentation, other.BytesRepresentationLength);
    }

    Attribute::~Attribute()
    {
        if (BytesRepresentation != nullptr)
        {
            delete[] BytesRepresentation;
            BytesRepresentation = nullptr;
        }
    }

    template <> char Attribute::getValue<char>()
    {
        checkForNullptr();
        return *(char*)BytesRepresentation;
    }

    template <> char* Attribute::getValue<char*>()
    {
        checkForNullptr();
        return (char*)BytesRepresentation;
    }

    template <> int Attribute::getValue<int>()
    {
        checkForNullptr();
        return *(int*)BytesRepresentation;
    }

    template <> int* Attribute::getValue<int*>()
    {
        checkForNullptr();
        return (int*)BytesRepresentation;
    }

    template <> long Attribute::getValue<long>()
    {
        checkForNullptr();
        return *(long*)BytesRepresentation;
    }

    template <> long* Attribute::getValue<long*>()
    {
        checkForNullptr();
        return (long*)BytesRepresentation;
    }

    template <> unsigned int Attribute::getValue<unsigned int>()
    {
        checkForNullptr();
        return *(unsigned int*)BytesRepresentation;
    }

    template <> unsigned int* Attribute::getValue<unsigned int*>()
    {
        checkForNullptr();
        return (unsigned int*)BytesRepresentation;
    }

    template <> unsigned long Attribute::getValue<unsigned long>()
    {
        checkForNullptr();
        return *(unsigned long*)BytesRepresentation;
    }

    template <> unsigned long* Attribute::getValue<unsigned long*>()
    {
        checkForNullptr();
        return (unsigned long*)BytesRepresentation;
    }

    template <> unsigned long long Attribute::getValue<unsigned long long>()
    {
        checkForNullptr();
        return *(unsigned long long*)BytesRepresentation;
    }

    template <> unsigned long long* Attribute::getValue<unsigned long long*>()
    {
        checkForNullptr();
        return (unsigned long long*)BytesRepresentation;
    }

    template <> long long Attribute::getValue<long long>()
    {
        checkForNullptr();
        return *(long long*)BytesRepresentation;
    }

    template <> long long* Attribute::getValue<long long*>()
    {
        checkForNullptr();
        return (long long*)BytesRepresentation;
    }

    template <> BYTE Attribute::getValue<BYTE>()
    {
        checkForNullptr();
        return *(BYTE*)BytesRepresentation;
    }

    template <> BYTE* Attribute::getValue<BYTE*>()
    {
        checkForNullptr();
        return BytesRepresentation;
    }

    template <> UINT16 Attribute::getValue<UINT16>()
    {
        checkForNullptr();
        return *(UINT16*)BytesRepresentation;
    }

    template <> UINT16* Attribute::getValue<UINT16*>()
    {
        checkForNullptr();
        return (UINT16*)BytesRepresentation;
    }

    template <> INT16 Attribute::getValue<INT16>()
    {
        checkForNullptr();
        return *(INT16*)BytesRepresentation;
    }

    template <> std::string Attribute::getValue<std::string>()
    {
        return Parsing;
    }

    std::string Attribute::getName()
    {
        return Name;
    }

    std::string Attribute::getDescription()
    {
        return Description;
    }

    UINT64 Attribute::getLength()
    {
        return BytesRepresentationLength;
    }

    void Attribute::checkForNullptr()
    {
        if (BytesRepresentation == nullptr)
        {
            throw std::exception("BytesRepresentation is nullptr");
        }
    }

}