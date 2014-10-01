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

    __ethernet.SetEthertype(Ethertype::IP);
    __ip.SetIpProtocol(IpProtocol::ICMP);

    __type = TYPE_ECHOREPLY;
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

bool Pdu::Icmp::GetRaw(unsigned char *buffer, unsigned int size) const
{
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

    unsigned int offset = 0;
    bool rc = __ethernet.GetRaw(buffer, size, &offset);
    if (!rc) {
        return false;
    }

    rc = __ip.GetRaw(&buffer[offset], size - offset, &offset);
    if (!rc) {
        return false;
    }

    //TODO: Add GetSize for headers
    if ((size - offset) < rawMinSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Buffer size " << (size - offset)
                  << " less than minimal PDU size " << rawMinSize << std::endl;
        return false;
    }

    offset += typeOffset;
    rc = Object::ConvertToRaw(&buffer[offset], __type);
    if (!rc) {
        return false;
    }

    offset += codeOffset;
    rc = Object::ConvertToRaw(&buffer[offset], __code);
    if (!rc) {
        return false;
    }

    offset += checksumOffset;
    rc = Object::ConvertToRaw(&buffer[offset], __checksum);
    if (!rc) {
        return false;
    }

    offset += restOffset;
    rc = Object::ConvertToRaw(&buffer[offset], __rest);
    if (!rc) {
        return false;
    }

    return true;
}

bool Pdu::Icmp::SetRaw(const unsigned char *buffer, unsigned int size)
{
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

    bool rc = __ethernet.SetRaw(buffer, size);

    return false;
}

bool Pdu::Icmp::SetType(Type value)
{
    __type = value;

    return true;
}

Pdu::Icmp::Type Pdu::Icmp::GetType() const
{
    return __type;
}

bool Pdu::Icmp::SetCode(Code value)
{
    __code = value;

    return true;
}

Pdu::Icmp::Code Pdu::Icmp::GetCode() const
{
    return __code;
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

unsigned short Pdu::Icmp::CalculateChecksum(const unsigned char *buffer, unsigned int size)
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return 0;
    }

    if ((size % 2) != 0) {
        std::cerr << __PRETTY_FUNCTION__ << ": Buffer size is invalid" << std::endl;
        return 0;
    }

    unsigned int offset = 0;
    unsigned int sum = 0;
    unsigned short octet = 0;
    while (offset < size) {
        octet = *reinterpret_cast<unsigned short *>(&buffer[offset]);
        sum += octet;
        offset += sizeof(unsigned short);
    }

    unsigned short checksum = (sum & 0xFFFF0000) + (sum & 0x0000FFFF);

    return ~checksum;
}

unsigned short Pdu::Icmp::CalculateChecksum() const
{
    unsigned char buffer[rawMinSize];

    bool rc = GetRaw(buffer, sizeof(buffer));
    if (!rc) {
        return 0;
    }

    unsigned short checksum = 0;
    rc = Object::ConvertToRaw(&buffer[checksumOffset], checksum);
    if (!rc) {
        return 0;
    }

    return CalculateChecksum(buffer, sizeof(buffer));
}

bool Pdu::Icmp::IsChecksumValid() const
{
    unsigned char buffer[rawMinSize];

    bool rc = GetRaw(buffer, sizeof(buffer));
    if (!rc) {
        return false;
    }

    unsigned short checksum = CalculateChecksum(buffer, sizeof(buffer));

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
