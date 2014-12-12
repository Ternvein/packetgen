/*
 * pdu_icmp.cpp
 *
 *  Created on: Sep 24, 2014
 *      Author: elestar
 */

#include "pdu_icmp.h"

#include <string.h>


Pdu::Icmp::Icmp()
{
    __data = NULL;
    Clear();
}

Pdu::Icmp::Icmp(const Icmp &icmp)
{
    Set(icmp);
}

Pdu::Icmp::~Icmp()
{
    Clear();
}

void Pdu::Icmp::Clear()
{
    Header::Ethernet *ethernet = dynamic_cast<Header::Ethernet *>(this);
    Header::Ip *ip = dynamic_cast<Header::Ip *>(this);

    ethernet->Clear();
    ip->Clear();

    Ethertype ether(Ethertype::IP);
    IpProtocol proto(IpProtocol::ICMP);
    SetEthertype(ether);
    SetIpProtocol(proto);

    if (__data != NULL) {
        delete [] __data;
        __data = NULL;
    }

    __type = 0;
    __code = 0;
    __checksum = 0;
    __rest = 0;
    __dataSize = 0;
}

void Pdu::Icmp::Set(const Icmp &icmp)
{
    Header::Ethernet *ethernet = dynamic_cast<Header::Ethernet *>(this);
    Header::Ip *ip = dynamic_cast<Header::Ip *>(this);

    ethernet->Set(icmp);
    ip->Set(icmp);

    __type = icmp.__type;
    __code = icmp.__code;
    __checksum = icmp.__checksum;
    __rest = icmp.__rest;
}

bool Pdu::Icmp::GetRaw(unsigned char *buffer, unsigned int size, unsigned int *offset) const
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    const Header::Ethernet *ethernet = dynamic_cast<const Header::Ethernet *>(this);
    const Header::Ip *ip = dynamic_cast<const Header::Ip *>(this);

    unsigned int minSize = GetSize();
    if (size < minSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Buffer size " << size
                  << " less than minimal PDU size " << minSize << std::endl;
        return false;
    }

    unsigned int rawOffset = 0;
    bool rc = ethernet->GetRaw(buffer, size, &rawOffset);
    if (!rc) {
        return false;
    }

    rc = ip->GetRaw(&buffer[rawOffset], size - rawOffset, &rawOffset);
    if (!rc) {
        return false;
    }

    rc = Object::ConvertToRaw(&buffer[rawOffset + typeOffset], __type);
    if (!rc) {
        return false;
    }

    rc = Object::ConvertToRaw(&buffer[rawOffset + codeOffset], __code);
    if (!rc) {
        return false;
    }

    rc = Object::ConvertToRaw(&buffer[rawOffset + checksumOffset], __checksum);
    if (!rc) {
        return false;
    }

    rc = Object::ConvertToRaw(&buffer[rawOffset + restOffset], __rest);
    if (!rc) {
        return false;
    }
    rawOffset += (restOffset + sizeof(__rest));

    memcpy(&buffer[rawOffset], __data, __dataSize);

    if (offset != NULL) {
        *offset += (rawOffset + __dataSize);
    }

    return true;
}

bool Pdu::Icmp::SetRaw(const unsigned char *buffer, unsigned int size, unsigned int *offset)
{
    Clear();

    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    Header::Ethernet *ethernet = dynamic_cast<Header::Ethernet *>(this);
    Header::Ip *ip = dynamic_cast<Header::Ip *>(this);

    unsigned int minSize = GetMinSize();
    if (size < minSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Buffer size " << size
                  << " less than minimal PDU size " << minSize << std::endl;
        return false;
    }

    unsigned int rawOffset = 0;
    bool rc = ethernet->SetRaw(buffer, size, &rawOffset);
    if (!rc) {
        return false;
    }

    rc = ip->SetRaw(&buffer[rawOffset], size - rawOffset, &rawOffset);
    if (!rc) {
        return false;
    }

    __type = buffer[rawOffset + typeOffset];
    __code = buffer[rawOffset + codeOffset];
    __checksum = *reinterpret_cast<const unsigned short *>(&buffer[rawOffset + checksumOffset]);
    __rest = *reinterpret_cast<const unsigned int *>(&buffer[rawOffset + restOffset]);
    rawOffset += (restOffset + sizeof(__rest));
    if (rawOffset < size) {
        SetData(&buffer[rawOffset], size - rawOffset);
    }

    if (offset != NULL) {
        rawOffset = size;
        *offset += rawOffset;
    }

    return true;
}

unsigned int Pdu::Icmp::GetMinSize() const
{
    return (Header::Ethernet::rawMinSize + Header::Ip::rawMinSize + rawMinSize);
}

unsigned int Pdu::Icmp::GetSize() const
{
    const Header::Ethernet *ethernet = dynamic_cast<const Header::Ethernet *>(this);
    const Header::Ip *ip = dynamic_cast<const Header::Ip *>(this);

    return (ethernet->GetSize() + ip->GetSize() + rawMinSize + __dataSize);
}

bool Pdu::Icmp::SetType(Type value)
{
    __type = value;

    return true;
}

Pdu::Icmp::Type Pdu::Icmp::GetType() const
{
    return static_cast<Type>(__type);
}

bool Pdu::Icmp::SetCode(Code value)
{
    __code = value;

    return true;
}

Pdu::Icmp::Code Pdu::Icmp::GetCode() const
{
    return static_cast<Code>(__code);
}

bool Pdu::Icmp::SetChecksum(unsigned short checksum)
{
    unsigned short correct = CalculateChecksum();
    if (checksum != correct) {
        std::cerr << __PRETTY_FUNCTION__ << ": Warning! Checksum doesn't match (0x"
                  << std::hex << checksum << std::dec << ", correct: 0x"
                  << std::hex << correct << std::dec << ")" << std::endl;
    }

    __checksum = checksum;

    return true;
}

unsigned short Pdu::Icmp::GetChecksum() const
{
    return __checksum;
}

unsigned short Pdu::Icmp::CalculateChecksum() const
{
    unsigned int size = GetSize();
    unsigned char buffer[size];

    bool rc = GetRaw(buffer, size, NULL);
    if (!rc) {
        return 0;
    }

    const Header::Ethernet *ethernet = dynamic_cast<const Header::Ethernet *>(this);
    const Header::Ip *ip = dynamic_cast<const Header::Ip *>(this);
    unsigned int offset = ethernet->GetSize() + ip->GetSize() + checksumOffset;

    unsigned short checksum = 0;
    rc = Object::ConvertToRaw(&buffer[offset], checksum);
    if (!rc) {
        return 0;
    }

    return Header::Ip::CalculateChecksum(buffer, size);
}

bool Pdu::Icmp::IsChecksumValid() const
{
    unsigned char buffer[rawMinSize];

    bool rc = GetRaw(buffer, sizeof(buffer), NULL);
    if (!rc) {
        return false;
    }

    unsigned short checksum = CalculateChecksum();

    return (checksum == 0);
}

bool Pdu::Icmp::SetRest(unsigned int rest)
{
    __rest = rest;

    return true;
}

unsigned int Pdu::Icmp::GetRest() const
{
    return __rest;
}

bool Pdu::Icmp::SetData(const unsigned char *data, unsigned int dataSize)
{
    if (data == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    if (dataSize == 0) {
        std::cerr << __PRETTY_FUNCTION__ << ": Data size " << dataSize << " invalid" << std::endl;
        return false;
    }

    if (__data != NULL) {
        delete [] __data;
        __data = NULL;
    }

    __data = new unsigned char [dataSize];
    __dataSize = dataSize;
    memcpy(__data, data, dataSize);

    return true;
}

bool Pdu::Icmp::GetData(unsigned char *data, unsigned int dataSize) const
{
    if (data == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    if (dataSize != __dataSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Data size " << dataSize << " invalid" << std::endl;
        return false;
    }

    if (__data == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": Data is empty" << std::endl;
        return false;
    }

    memcpy(data, __data, dataSize);

    return true;
}

void Pdu::Icmp::ClearData()
{
    if (__data != NULL) {
        delete [] __data;
        __data = NULL;
    }

    __dataSize = 0;
}

unsigned int Pdu::Icmp::GetDataSize() const
{
    return __dataSize;
}

bool Pdu::Icmp::operator==(const Icmp &right) const
{
    const Header::Ethernet *ethernet = dynamic_cast<const Header::Ethernet *>(this);
    const Header::Ip *ip = dynamic_cast<const Header::Ip *>(this);

    if (*ethernet != right) {
        return false;
    }

    if (*ip != right) {
        return false;
    }

    if (__type != right.__type) {
        return false;
    }

    if (__code != right.__code) {
        return false;
    }

    if (__checksum != right.__checksum) {
        return false;
    }

    if (__rest != right.__rest) {
        return false;
    }

    return true;
}

bool Pdu::Icmp::operator!=(const Icmp &right) const
{
    return !operator==(right);
}
