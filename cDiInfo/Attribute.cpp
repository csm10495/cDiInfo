/*
* cDiInfo - An application to get information via the Windows SetupDi... APIs
* Charles Machalow - MIT License - (C) 2016
* Attribute.cpp - Implementation file for the generic attribute type
*/

// STL Includes
#include <string>

// Local Includes
#include "Attribute.h"
#include "Strings.h"

// #defines
#define UNNAMED_ATTRIBUTE "Unnamed Attribute"
#define NO_DESCRIPTION    "No Description"
#define NO_PARSING        "No Parsing"

namespace cdi
{
    namespace attr
    {
        Attribute::Attribute(BYTE* bytes, size_t length, std::string name, std::string description, std::string parsing)
        {
            BytesRepresentation = std::make_unique<BYTE[]>(length);
            BytesRepresentationLength = length;
            Name = name;
            Description = description;
            Parsing = parsing;
            memcpy_s(BytesRepresentation.get(), length, bytes, length);
        }

        Attribute::Attribute(std::string name, std::string description, std::string parsing) : Attribute((BYTE*)parsing.c_str(), parsing.size(), name, description, parsing)
        {

        }

        Attribute::Attribute(std::string name, std::string description, std::wstring parsing) : Attribute((BYTE*)parsing.c_str(), parsing.size() * (ULONG)sizeof(wchar_t), name, description, cdi::strings::wStringToString(parsing))
        {

        }

        Attribute::Attribute(const Attribute &other)
        {
            deepCopy(*this, other);
        }

        Attribute::Attribute(const Attribute &other, std::string addStr)
        {
            deepCopy(*this, other);
            this->Parsing = other.Parsing + addStr;
        }

        Attribute::Attribute()
        {
            this->BytesRepresentation = NULL;
            this->BytesRepresentationLength = 0;
            this->Description = NO_DESCRIPTION;
            this->Name = UNNAMED_ATTRIBUTE;
            this->Parsing = NO_PARSING;
        }

        Attribute &Attribute::operator=(Attribute &other)
        {
            deepCopy(*this, other);
            return *this;
        }

        Attribute::~Attribute()
        {
            // unique_ptr takes care of this
        }

        template <> char Attribute::getValue<char>()
        {
            return *(char*)BytesRepresentation.get();
        }

        template <> char* Attribute::getValue<char*>()
        {
            return (char*)BytesRepresentation.get();
        }

        template <> int Attribute::getValue<int>()
        {
            return *(int*)BytesRepresentation.get();
        }

        template <> int* Attribute::getValue<int*>()
        {
            return (int*)BytesRepresentation.get();
        }

        template <> long Attribute::getValue<long>()
        {
            return *(long*)BytesRepresentation.get();
        }

        template <> long* Attribute::getValue<long*>()
        {
            return (long*)BytesRepresentation.get();
        }

        template <> unsigned int Attribute::getValue<unsigned int>()
        {
            return *(unsigned int*)BytesRepresentation.get();
        }

        template <> unsigned int* Attribute::getValue<unsigned int*>()
        {
            return (unsigned int*)BytesRepresentation.get();
        }

        template <> unsigned long Attribute::getValue<unsigned long>()
        {
            return *(unsigned long*)BytesRepresentation.get();
        }

        template <> unsigned long* Attribute::getValue<unsigned long*>()
        {
            return (unsigned long*)BytesRepresentation.get();
        }

        template <> unsigned long long Attribute::getValue<unsigned long long>()
        {
            return *(unsigned long long*)BytesRepresentation.get();
        }

        template <> unsigned long long* Attribute::getValue<unsigned long long*>()
        {
            return (unsigned long long*)BytesRepresentation.get();
        }

        template <> long long Attribute::getValue<long long>()
        {
            return *(long long*)BytesRepresentation.get();
        }

        template <> long long* Attribute::getValue<long long*>()
        {
            return (long long*)BytesRepresentation.get();
        }

        template <> BYTE Attribute::getValue<BYTE>()
        {
            return *(BYTE*)BytesRepresentation.get();
        }

        template <> BYTE* Attribute::getValue<BYTE*>()
        {
            return (BYTE*)BytesRepresentation.get();
        }

        template <> UINT16 Attribute::getValue<UINT16>()
        {
            return *(UINT16*)BytesRepresentation.get();
        }

        template <> UINT16* Attribute::getValue<UINT16*>()
        {
            return (UINT16*)BytesRepresentation.get();
        }

        template <> INT16 Attribute::getValue<INT16>()
        {
            return *(INT16*)BytesRepresentation.get();
        }

        template <> std::string Attribute::getValue<std::string>()
        {
            return Parsing;
        }

        template <> char Attribute::getValue<char>() const
        {
            return *(char*)BytesRepresentation.get();
        }

        template <> char* Attribute::getValue<char*>() const
        {
            return (char*)BytesRepresentation.get();
        }

        template <> int* Attribute::getValue<int*>() const
        {
            return (int*)BytesRepresentation.get();
        }

        template <> long Attribute::getValue<long>() const
        {
            return *(long*)BytesRepresentation.get();
        }

        template <> long* Attribute::getValue<long*>() const
        {
            return (long*)BytesRepresentation.get();
        }

        template <> unsigned int Attribute::getValue<unsigned int>() const
        {
            return *(unsigned int*)BytesRepresentation.get();
        }

        template <> unsigned int* Attribute::getValue<unsigned int*>() const
        {
            return (unsigned int*)BytesRepresentation.get();
        }

        template <> unsigned long Attribute::getValue<unsigned long>() const
        {
            return *(unsigned long*)BytesRepresentation.get();
        }

        template <> unsigned long* Attribute::getValue<unsigned long*>() const
        {
            return (unsigned long*)BytesRepresentation.get();
        }

        template <> unsigned long long Attribute::getValue<unsigned long long>() const
        {
            return *(unsigned long long*)BytesRepresentation.get();
        }

        template <> unsigned long long* Attribute::getValue<unsigned long long*>() const
        {
            return (unsigned long long*)BytesRepresentation.get();
        }

        template <> long long Attribute::getValue<long long>() const
        {
            return *(long long*)BytesRepresentation.get();
        }

        template <> long long* Attribute::getValue<long long*>() const
        {
            return (long long*)BytesRepresentation.get();
        }

        template <> BYTE Attribute::getValue<BYTE>() const
        {
            return *(BYTE*)BytesRepresentation.get();
        }

        template <> BYTE* Attribute::getValue<BYTE*>() const
        {
            return (BYTE*)BytesRepresentation.get();
        }

        template <> UINT16 Attribute::getValue<UINT16>() const
        {
            return *(UINT16*)BytesRepresentation.get();
        }

        template <> UINT16* Attribute::getValue<UINT16*>() const
        {
            return (UINT16*)BytesRepresentation.get();
        }

        template <> INT16 Attribute::getValue<INT16>() const
        {
            return *(INT16*)BytesRepresentation.get();
        }

        template <> std::string Attribute::getValue<std::string>() const
        {
            return Parsing;
        }

        std::string Attribute::getName()
        {
            return Name;
        }

        std::string Attribute::getName() const
        {
            return Name;
        }

        std::string Attribute::getDescription() const
        {
            return Description;
        }

        size_t Attribute::getLength() const
        {
            return BytesRepresentationLength;
        }

        void Attribute::deepCopy(Attribute &to, const Attribute &from)
        {
            to.BytesRepresentation = std::make_unique<BYTE[]>(from.BytesRepresentationLength);
            to.BytesRepresentationLength = from.BytesRepresentationLength;
            to.Description = from.Description;
            to.Name = from.Name;
            to.Parsing = from.Parsing;

            memcpy_s(to.BytesRepresentation.get(), to.BytesRepresentationLength, from.BytesRepresentation.get(), from.BytesRepresentationLength);
        }

        bool AttributeCompare::operator() (const Attribute &lhs, const Attribute &rhs)
        {
            return const_cast<Attribute*>(&lhs)->getName() < const_cast<Attribute*>(&rhs)->getName();
        }

        bool AttributeCompare::operator() (const Attribute &lhs, const Attribute &rhs) const
        {
            return const_cast<Attribute*>(&lhs)->getName() < const_cast<Attribute*>(&rhs)->getName();
        }

        std::string toXml(const Attribute &attr)
        {
            std::string xml = "<Attribute>\n";

            xml += "<name>" + cdi::strings::stringToXmlSafeStr(attr.getName()) + "</name>\n";
            xml += "<description>" + cdi::strings::stringToXmlSafeStr(attr.getDescription()) + "</description>\n";
            xml += "<parsing>" + cdi::strings::stringToXmlSafeStr(attr.getValue<std::string>()) + "</parsing>\n";
            xml += "<bytes length=\"" + cdi::strings::stringToXmlSafeStr(std::to_string(attr.getLength())) + "\">";

            //bytes to string
            for (UINT64 i = 0; i < attr.getLength(); i++)
            {
                xml += cdi::strings::numToHexString(attr.getValue<BYTE*>()[i], 2).substr(2); // Remove 0x
            }

            xml += "</bytes>\n";

            return xml + "</Attribute>";
        }

        std::string toXml(const AttributeSet &attrSet)
        {
            std::string xml = "<AttributeSet>\n";

            for (const Attribute &attr : attrSet)
            {
                xml += toXml(attr);
            }

            return xml + "</AttributeSet>";
        }

        std::string toXml(const AttributeSetVector &attrSetVector)
        {
            std::string xml = "<AttributeSetVector>\n";

            for (const AttributeSet &attrSet : attrSetVector)
            {
                xml += toXml(attrSet);
            }

            return xml + "</AttributeSetVector>";
        }
    }
}