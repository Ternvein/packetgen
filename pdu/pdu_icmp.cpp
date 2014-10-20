/*
 * pdu_icmp.cpp
 *
 *  Created on: Sep 24, 2014
 *      Author: elestar
 */

#include "pdu_icmp.h"


Pdu::Icmp::Icmp()
{
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
    __ethernet.Clear();
    __ip.Clear();

    Ethertype ether(Ethertype::IP);
    IpProtocol proto(IpProtocol::ICMP);
    __ethernet.SetEthertype(ether);
    __ip.SetIpProtocol(proto);

    __type = 0;
    __code = 0;
    __checksum = 0;
    __rest = 0;
}

void Pdu::Icmp::Set(const Icmp &icmp)
{
    __ethernet = icmp.__ethernet;
    __ip = icmp.__ip;

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

    unsigned int minSize = __ethernet.GetSize() + __ip.GetSize() + rawMinSize;
    if (size < minSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Buffer size " << size
                  << " less than minimal PDU size " << minSize << std::endl;
        return false;
    }

    unsigned int rawOffset = 0;
    bool rc = __ethernet.GetRaw(buffer, size, &rawOffset);
    if (!rc) {
        return false;
    }

    rc = __ip.GetRaw(&buffer[rawOffset], size - rawOffset, &rawOffset);
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

    if (offset != NULL) {
        rawOffset += (restOffset + sizeof(__rest));
        *offset += rawOffset;
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

    unsigned int minSize = rawMinSize + Header::Ethernet::rawMinSize + Header::Ip::rawMinSize;
    if (size < minSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Buffer size " << size
                  << " less than minimal PDU size " << minSize << std::endl;
        return false;
    }

    unsigned int rawOffset = 0;
    bool rc = __ethernet.SetRaw(buffer, size, &rawOffset);
    if (!rc) {
        return false;
    }

    rc = __ip.SetRaw(&buffer[rawOffset], size - rawOffset, &rawOffset);
    if (!rc) {
        return false;
    }

    __type = buffer[rawOffset + typeOffset];
    __code = buffer[rawOffset + codeOffset];
    __checksum = *reinterpret_cast<const unsigned short *>(&buffer[rawOffset + checksumOffset]);
    __rest = *reinterpret_cast<const unsigned int *>(&buffer[rawOffset + restOffset]);

    if (offset != NULL) {
        rawOffset += (restOffset + sizeof(__rest));
        *offset += rawOffset;
    }

    return true;
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
        std::cerr << __PRETTY_FUNCTION__ << ": Warning! Checksum doesn't match ("
                  << checksum << ", correct: " << correct << ")" << std::endl;
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
    unsigned char buffer[rawMinSize];

    bool rc = GetRaw(buffer, sizeof(buffer), NULL);
    if (!rc) {
        return 0;
    }

    unsigned short checksum = 0;
    rc = Object::ConvertToRaw(&buffer[checksumOffset], checksum);
    if (!rc) {
        return 0;
    }

    return Header::Ip::CalculateChecksum(buffer, sizeof(buffer));
}

bool Pdu::Icmp::IsChecksumValid() const
{
    unsigned char buffer[rawMinSize];

    bool rc = GetRaw(buffer, sizeof(buffer), NULL);
    if (!rc) {
        return false;
    }

    unsigned short checksum = Header::Ip::CalculateChecksum(buffer, sizeof(buffer));

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

bool Pdu::Icmp::operator==(const Icmp &right) const
{
    if (__ethernet != right.__ethernet) {
        return false;
    }

    if (__ip != right.__ip) {
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
