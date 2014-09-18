/*
 * macaddress.cpp
 *
 *  Created on: Sep 15, 2014
 *      Author: elestar
 */

#include "macaddress.h"

#include <stdio.h>
#include <string.h>
#include <iostream>


MacAddress::MacAddress()
{
    Clear();
}

MacAddress::MacAddress(const MacAddress &mac)
{
    Set(mac);
}

MacAddress::MacAddress(const unsigned char *mac, unsigned int size)
{
	SetRaw(mac, size);
}

MacAddress::MacAddress(const char *mac, unsigned int size)
{
	Parse(mac, size);
}

MacAddress::~MacAddress()
{
    Clear();
}

void MacAddress::Clear()
{
    memset(__raw, 0, sizeof(__raw));
    __isMulticast = false;
    __isBroadcast = false;
}

bool MacAddress::GetRaw(unsigned char *mac, unsigned int size) const
{
    if (mac == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL MAC detected" << std::endl;
        return false;
    }

    if (size < rawSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": MAC buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    memcpy(mac, __raw, sizeof(__raw));

    return true;
}

bool MacAddress::SetRaw(const unsigned char *mac, unsigned int size)
{
	Clear();

    if (mac == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL MAC detected" << std::endl;
        return false;
    }

    if (size < rawSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": MAC buffer size " << size << " is too short" << std::endl;
        return false;
    }

    memcpy(__raw, mac, sizeof(__raw));

    CheckMulticast();
    CheckBroadcast();

    return true;
}

bool MacAddress::Set(const MacAddress &mac)
{
    Clear();

    memcpy(__raw, mac.__raw, sizeof(__raw));
    __isMulticast = mac.__isMulticast;
    __isBroadcast = mac.__isBroadcast;

    return true;
}

bool MacAddress::Parse(const char *mac, unsigned int size)
{
    Clear();

    if (mac == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL MAC detected" << std::endl;
        return false;
    }

    if (size < stringSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": MAC buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    unsigned char raw[rawSize];
    unsigned int offset = 0;
    unsigned int octet = 0;
    int rc;

    while (mac[offset] != '\0') {
        unsigned int byte;
        rc = sscanf(&mac[offset], "%x", &byte);
        if (rc != 1) {
            std::cerr << __PRETTY_FUNCTION__ << ": Error reading byte "
                      << offset << " from buffer" << std::endl;
            return false;
        }

        raw[octet] = static_cast<unsigned char>(byte);

        octet++;
        if (octet >= rawSize) {
            break;
        }

        offset += 2;
        if (mac[offset] != ':') {
            std::cerr << __PRETTY_FUNCTION__ << ": Error reading byte "
                      << offset << " from buffer" << std::endl;
            return false;
        }

        offset++;
    }

    memcpy(__raw, raw, sizeof(__raw));

    CheckMulticast();
    CheckBroadcast();

    return true;
}

bool MacAddress::ToString(char *mac, unsigned int size) const
{
    if (mac == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL MAC detected" << std::endl;
        return false;
    }

    if (size < stringSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": MAC buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    unsigned int offset = 0;
    for (unsigned int i = 0; i < rawSize; i++) {
        snprintf(&mac[offset], 4, "%02X:", __raw[i]);
        offset += 3;
    }

    mac[stringSize - 1] = '\0';

    return true;
}

void MacAddress::CheckMulticast()
{
    if (__raw[0] & 0x01) {
        __isMulticast = true;
        return;
    }

    __isMulticast = false;
}

void MacAddress::CheckBroadcast()
{
    for (unsigned int i = 0; i < rawSize; i++) {
        if ((__raw[i] & 0xFF) != 0xFF) {
            __isBroadcast = false;
            return;
        }
    }

    __isBroadcast = true;
}

bool MacAddress::IsMulticast() const
{
    return __isMulticast;
}

bool MacAddress::IsBroadcast() const
{
    return __isBroadcast;
}

bool MacAddress::operator==(const MacAddress &right) const
{
    for (unsigned int i = 0; i < rawSize; i++) {
        if (__raw[i] != right.__raw[i]) {
            return false;
        }
    }

    if (__isMulticast != right.__isMulticast) {
        return false;
    }

    if (__isBroadcast != right.__isBroadcast) {
        return false;
    }

    return true;
}

bool MacAddress::operator!=(const MacAddress &right) const
{
    return !operator==(right);
}

MacAddress MacAddress::operator++()
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

MacAddress MacAddress::operator++(int)
{
    MacAddress mac(*this);

    ++*this;

    return mac;
}
