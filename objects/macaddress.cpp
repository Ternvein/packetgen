/*
 * macaddress.cpp
 *
 *  Created on: Sep 15, 2014
 *      Author: elestar
 */

#include "macaddress.h"
#include <stdio.h>
#include <string.h>


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
        return false;
    }

    if (size < rawSize) {
        return false;
    }

    memcpy(mac, __raw, sizeof(__raw));

    return true;
}

bool MacAddress::SetRaw(const unsigned char *mac, unsigned int size)
{
	Clear();

    if (mac == NULL) {
        return false;
    }

    if (size < rawSize) {
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
    unsigned char raw[rawSize];

    Clear();

    if (mac == NULL) {
        return false;
    }

    if (size < stringSize) {
        return false;
    }

    unsigned int rc;
    unsigned int offset = 0;
    unsigned int octet = 0;
    for (unsigned int i = 0; i < rawSize; i++) {
        rc = sscanf(&mac[offset], "%x", &raw[i]);
        if (rc != 1) {
            return false;
        }

        offset += 2;
        if (mac[offset] != ':') {
            return false;
        }

        offset++;
    }

    while (mac[offset] != '\0') {
        rc = sscanf(&mac[offset], "%x", &raw[octet]);
        if (rc != 1) {
            return false;
        }

        octet++;
        if (octet >= rawSize) {
            break;
        }

        offset += 2;
        if (mac[offset] != ':') {
            return false;
        }

        offset++;
    }

    CheckMulticast();
    CheckBroadcast();

    return true;
}

bool MacAddress::ToString(char *mac, unsigned int size) const
{
    if (mac == NULL) {
        return false;
    }

    if (size < stringSize) {
        return false;
    }

    unsigned int offset = 0;
    for (unsigned int i = 0; i < rawSize; i++) {
        snprintf(&mac[offset], 3, "%02X:", __raw[i]);
        offset += 3;
    }

    mac[stringSize] = '\0';

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
