/*
 * header_ip.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#include "header_ip.h"

#include <stdio.h>
#include <string.h>


Header::Ip::Ip()
{
    Clear();
}

Header::Ip::Ip(const Ip &ip)
{
    Set(ip);
}

Header::Ip::~Ip()
{
    Clear();
}

void Header::Ip::Clear()
{
    __version = versionIpv4;
    __headerLength = rawMinSize;
    __dscp = 0;
    __ecn = 0;
    __totalLength = rawMinSize;
    __identification = 0;

    __isDontFragment = true;
    __isMoreFragments = false;

    __fragmentOffset = 0;
    __ttl = defaultTtl;

    __ipProto = 0;

    __checksum = 0;

    __srcIp.Clear();
    __dstIp.Clear();
}

void Header::Ip::Set(const Ip &ip)
{
    Clear();

    __version = ip.__version;
    __headerLength = ip.__headerLength;
    __dscp = ip.__dscp;
    __ecn = ip.__ecn;
    __totalLength = ip.__totalLength;
    __identification = ip.__identification;

    __isDontFragment = ip.__isDontFragment;
    __isMoreFragments = ip.__isMoreFragments;

    __fragmentOffset = ip.__fragmentOffset;
    __ttl = ip.__ttl;

    __ipProto = ip.__ipProto;

    __checksum = ip.__checksum;

    __srcIp = ip.__srcIp;
    __dstIp = ip.__dstIp;
}

bool Header::Ip::GetRaw(unsigned char *buffer, unsigned int size, unsigned int *offset) const
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL header detected" << std::endl;
        return false;
    }

    if (size < rawMinSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Header buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    unsigned char raw[size];

    raw[versionMask] = (__version << versionMaskOffset) & versionMask;
    raw[headerLengthMask] = __headerLength & headerLengthMask;
    raw[dscpOffset] = (__dscp << dscpMaskOffset) & dscpMask;
    raw[ecnOffset] = __ecn & ecnMask;

    bool rc = Object::ConvertToRaw(&raw[totalLengthOffset], __totalLength);
    if (!rc) {
        std::cerr << __PRETTY_FUNCTION__ << ": Error writing total length to the buffer" << std::endl;
        return false;
    }

    rc = Object::ConvertToRaw(&raw[identificationOffset], __identification);
    if (!rc) {
        std::cerr << __PRETTY_FUNCTION__ << ": Error writing identification to the buffer" << std::endl;
        return false;
    }

    rc = Object::ConvertToRaw(&raw[fragmentFieldOffset], __fragmentOffset);
    if (!rc) {
        std::cerr << __PRETTY_FUNCTION__ << ": Error writing fragment field to the buffer" << std::endl;
        return false;
    }

    raw[flagsOffset] &= ~flagsMask;
    if (__isDontFragment) {
        raw[flagsOffset] |= flagDfMask;
    }
    if (__isMoreFragments) {
        raw[flagsOffset] |= flagMfMask;
    }

    raw[ttlOffset] = __ttl;
    raw[ipProtoOffset] = __ipProto.Get();

    rc = Object::ConvertToRaw(&raw[checksumOffset], __checksum);
    if (!rc) {
        std::cerr << __PRETTY_FUNCTION__ << ": Error writing checksum to the buffer" << std::endl;
        return false;
    }

    rc = __srcIp.GetRaw(&raw[srcIpOffset], size - srcIpOffset);
    if (!rc) {
        std::cerr << __PRETTY_FUNCTION__ << ": Error writing source IP to the buffer" << std::endl;
        return false;
    }

    rc = __dstIp.GetRaw(&raw[dstIpOffset], size - dstIpOffset);
    if (!rc) {
        std::cerr << __PRETTY_FUNCTION__ << ": Error writing destination IP to the buffer" << std::endl;
        return false;
    }

    memcpy(buffer, raw, size);

    if (offset != NULL) {
        *offset = *offset + dstIpOffset + IpAddress::rawSize;
    }

    return true;
}

bool Header::Ip::SetRaw(const unsigned char *buffer, unsigned int size, unsigned int *offset)
{
    Clear();

    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL header detected" << std::endl;
        return false;
    }

    if (size < rawMinSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Header buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    __version = (buffer[versionOffset] & versionMask) >> versionMaskOffset;
    __headerLength = buffer[headerLengthOffset] & headerLengthMask;
    __dscp = (buffer[dscpOffset] & dscpMask) >> dscpMaskOffset;
    __ecn = buffer[ecnOffset] & ecnMask;

    __totalLength = *reinterpret_cast<const unsigned short *>(&buffer[totalLengthOffset]);
    __identification = *reinterpret_cast<const unsigned short *>(&buffer[identificationOffset]);

    if (buffer[flagsOffset] & flagDfMask) {
        __isDontFragment = true;
    } else {
        __isDontFragment = false;
    }

    if (buffer[flagsOffset] & flagMfMask) {
        __isMoreFragments = true;
    } else {
        __isMoreFragments = false;
    }

    __fragmentOffset = (*reinterpret_cast<const unsigned short *>(&buffer[fragmentFieldOffset])) & fragmentFieldMask;
    __ttl = buffer[ttlOffset];
    __ipProto.Set(buffer[ipProtoOffset]);

    __checksum = *reinterpret_cast<const unsigned short *>(&buffer[checksumOffset]);

    bool rc = __srcIp.SetRaw(&buffer[srcIpOffset], size - srcIpOffset);
    if (!rc) {
        return false;
    }

    rc = __dstIp.SetRaw(&buffer[dstIpOffset], size - dstIpOffset);
    if (!rc) {
        return false;
    }

    if (offset != NULL) {
        *offset = *offset + dstIpOffset + IpAddress::rawSize;
    }

    return true;
}

unsigned int Header::Ip::GetSize() const
{
    unsigned int size = rawMinSize;

    return size;
}

bool Header::Ip::SetHeaderLength(unsigned int length)
{
    __headerLength = length & headerLengthMask;

    return true;
}

unsigned int Header::Ip::GetHeaderLength() const
{
    return __headerLength;
}

bool Header::Ip::SetTotalLength(unsigned int length)
{
    unsigned short max = ~0;

    if (length < rawMinSize || length > max) {
        std::cerr << __PRETTY_FUNCTION__ << ": Invalid total length" << std::endl;
        return false;
    }

    __totalLength = static_cast<unsigned short>(length);

    return true;
}

unsigned int Header::Ip::GetTotalLength() const
{
    return __totalLength;
}

bool Header::Ip::SetFlagDontFragment(bool flag)
{
    if (__fragmentOffset != 0 && flag) {
        std::cerr << __PRETTY_FUNCTION__ << ": Can't set Don't Fragment flag with non-zero fragment offset" << std::endl;
        return false;
    }

    __isDontFragment = flag;

    return true;
}

bool Header::Ip::GetFlagDontFragment() const
{
    return __isDontFragment;
}

bool Header::Ip::SetFlagMoreFragments(bool flag)
{
    __isMoreFragments = flag;

    return true;
}

bool Header::Ip::GetFlagMoreFragments() const
{
    return __isMoreFragments;
}

bool Header::Ip::SetFragmentOffset(unsigned int offset)
{
    if (__isDontFragment && offset != 0) {
        std::cerr << __PRETTY_FUNCTION__ << ": Can't set non-zero fragment offset with Don't Fragment flag" << std::endl;
        return false;
    }

    __fragmentOffset = offset;

    return true;
}

unsigned int Header::Ip::GetFragmentOffset() const
{
    return __fragmentOffset;
}

bool Header::Ip::SetTtl(unsigned int ttl)
{
    unsigned char max = ~0;

    if (ttl > max) {
        std::cerr << __PRETTY_FUNCTION__ << ": TTL value is too big" << std::endl;
        return false;
    }

    __ttl = ttl;

    return true;
}

unsigned int Header::Ip::GetTtl() const
{
    return __ttl;
}

bool Header::Ip::SetIpProtocol(const IpProtocol &protocol)
{
    __ipProto = protocol;

    return true;
}

IpProtocol Header::Ip::GetIpProtocol() const
{
    return __ipProto;
}

bool Header::Ip::SetChecksum(unsigned short checksum)
{
    unsigned short correct = CalculateChecksum();

    if (checksum != correct) {
        std::cerr << __PRETTY_FUNCTION__ << ": Warning! Checksum doesn't match ("
                  << checksum << ", correct: " << correct << ")" << std::endl;
    }

    __checksum = checksum;

    return true;
}

unsigned short Header::Ip::GetChecksum() const
{
    return __checksum;
}

unsigned short Header::Ip::CalculateChecksum(const unsigned char *buffer, unsigned int size)
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
        octet = *reinterpret_cast<const unsigned short *>(&buffer[offset]);
        sum += octet;
        offset += sizeof(unsigned short);
    }

    unsigned short checksum = (sum & 0xFFFF0000) + (sum & 0x0000FFFF);

    return ~checksum;
}

unsigned short Header::Ip::CalculateChecksum() const
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

    return CalculateChecksum(buffer, sizeof(buffer));
}

bool Header::Ip::IsChecksumValid() const
{
    unsigned char buffer[rawMinSize];

    bool rc = GetRaw(buffer, sizeof(buffer), NULL);
    if (!rc) {
        return false;
    }

    unsigned short checksum = CalculateChecksum(buffer, sizeof(buffer));

    return (checksum == 0);
}

bool Header::Ip::SetSrcIp(const IpAddress &ip)
{
    return __srcIp.Set(ip);
}

IpAddress Header::Ip::GetSrcIp() const
{
    return __srcIp;
}

bool Header::Ip::SetDstIp(const IpAddress &ip)
{
    return __dstIp.Set(ip);
}

IpAddress Header::Ip::GetDstIp() const
{
    return __dstIp;
}

bool Header::Ip::operator==(const Ip &right) const
{
    if (__version != right.__version) {
        return false;
    }

    if (__headerLength != right.__headerLength) {
        return false;
    }

    if (__dscp != right.__dscp) {
        return false;
    }

    if (__ecn != right.__ecn) {
        return false;
    }

    if (__totalLength != right.__totalLength) {
        return false;
    }

    if (__identification != right.__identification) {
        return false;
    }

    if (__isDontFragment != right.__isDontFragment) {
        return false;
    }

    if (__isMoreFragments != right.__isMoreFragments) {
        return false;
    }

    if (__fragmentOffset != right.__fragmentOffset) {
        return false;
    }

    if (__ttl != right.__ttl) {
        return false;
    }

    if (__ipProto != right.__ipProto) {
        return false;
    }

    if (__srcIp != right.__srcIp) {
        return false;
    }

    if (__dstIp != right.__dstIp) {
        return false;
    }

    return true;
}

bool Header::Ip::operator!=(const Ip &right) const
{
    return !operator==(right);
}
