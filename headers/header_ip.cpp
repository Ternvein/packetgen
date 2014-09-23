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

bool Header::Ip::GetRaw(unsigned char *buffer, unsigned int size) const
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

    return true;
}

bool Header::Ip::SetRaw(const unsigned char *buffer, unsigned int size)
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

    return true;
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
