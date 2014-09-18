/*
 * ipaddress.cpp
 *
 *  Created on: Sep 15, 2014
 *      Author: elestar
 */

#include "ipaddress.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>


IpAddress::IpAddress()
{
    Clear();
}

IpAddress::IpAddress(const IpAddress &ip)
{
    Set(ip);
}

IpAddress::IpAddress(const unsigned char *ip, unsigned int size)
{
    SetRaw(ip, size);
}

IpAddress::IpAddress(const char *ip, unsigned int size)
{
    Parse(ip, size);
}

IpAddress::~IpAddress()
{
    Clear();
}

void IpAddress::Clear()
{
    memset(__raw, 0, sizeof(__raw));
    __isMulticast = false;
    __isBroadcast = false;
}

bool IpAddress::GetRaw(unsigned char *ip, unsigned int size) const
{
    if (ip == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP detected" << std::endl;
        return false;
    }

    if (size < rawSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    memcpy(ip, __raw, sizeof(__raw));

    return true;
}

bool IpAddress::SetRaw(const unsigned char *ip, unsigned int size)
{
    Clear();

    if (ip == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP detected" << std::endl;
        return false;
    }

    if (size < rawSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    memcpy(__raw, ip, sizeof(__raw));

    return true;
}

bool IpAddress::Set(const IpAddress &ip)
{
    Clear();

    memcpy(__raw, ip.__raw, sizeof(__raw));
    __isMulticast = ip.__isMulticast;
    __isBroadcast = ip.__isBroadcast;

    return true;
}

unsigned int IpAddress::GetStringSize() const
{
    return stringMaxSize;
}

bool IpAddress::Parse(const char *ip, unsigned int size)
{
    Clear();

    if (ip == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP detected" << std::endl;
        return false;
    }

    if (size < stringMinSize || size > stringMaxSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    unsigned char raw[rawSize];
    int rc = inet_pton(AF_INET, ip, raw);
    if (rc > 0) {
        memcpy(__raw, raw, sizeof(__raw));

        CheckMulticast();
        CheckBroadcast();

        return true;
    }

    std::cerr << __PRETTY_FUNCTION__ << ": Error converting string into IP" << std::endl;

    return false;
}

bool IpAddress::ToString(char *ip, unsigned int size) const
{
    if (ip == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP detected" << std::endl;
        return false;
    }

    if (size < stringMaxSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    inet_ntop(AF_INET, __raw, ip, size);

    return true;
}

void IpAddress::CheckMulticast()
{
    unsigned int ip = *reinterpret_cast<unsigned int *>(__raw);

    if ((ip & multicastIpMask) == multicastIp) {
        __isMulticast = true;
        return;
    }

    __isMulticast = false;
}

void IpAddress::CheckBroadcast()
{
    unsigned int ip = *reinterpret_cast<unsigned int *>(__raw);

    if (ip == broadcastIp) {
        __isBroadcast = true;
        return;
    }

    __isBroadcast = false;
}

bool IpAddress::IsMulticast() const
{
    return __isMulticast;
}

bool IpAddress::IsBroadcast() const
{
    return __isBroadcast;
}

bool IpAddress::operator==(const IpAddress &right) const
{
    int rc = memcmp(__raw, right.__raw, sizeof(__raw));
    if (rc != 0) {
        return false;
    }

    if (__isMulticast != right.__isMulticast) {
        return false;
    }

    if (__isBroadcast != right.__isBroadcast) {
        return false;
    }

    return true;
}

bool IpAddress::operator!=(const IpAddress &right) const
{
    return !operator==(right);
}

IpAddress IpAddress::operator++()
{
    unsigned int i = sizeof(__raw) - 1;

    for (; i >= 0; i--) {
        __raw[i]++;
        if (__raw[i] != 0) {
            break;
        }
    }

    return *this;
}

IpAddress IpAddress::operator++(int)
{
    IpAddress ip(*this);

    ++*this;

    return ip;
}
