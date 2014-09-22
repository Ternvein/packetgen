/*
 * ipprotocol.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#include "ipprotocol.h"


IpProtocol::IpProtocol()
{
    Clear();
}

IpProtocol::IpProtocol(const IpProtocol &protocol)
{
    Set(protocol);
}

IpProtocol::IpProtocol(unsigned char protocol)
{
    Set(protocol);
}

IpProtocol::~IpProtocol()
{
    Clear();
}

void IpProtocol::Clear()
{
    __raw = 0;
}

void IpProtocol::Set(const IpProtocol &protocol)
{
    Clear();

    __raw = protocol.__raw;
}

bool IpProtocol::Set(unsigned char protocol)
{
    Clear();

    __raw = protocol & protocolMask;

    return true;
}

unsigned char IpProtocol::Get() const
{
    return __raw;
}

bool IpProtocol::GetRaw(unsigned char *buffer, unsigned int size) const
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP protocol detected" << std::endl;
        return false;
    }

    if (size < rawSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP protocol buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    buffer[0] = __raw;

    return true;
}

bool IpProtocol::SetRaw(const unsigned char *buffer, unsigned int size)
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP protocol detected" << std::endl;
        return false;
    }

    if (size < rawSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP protocol buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    __raw = buffer[0];

    return true;
}

unsigned int IpProtocol::GetStringSize() const
{
    return stringMinSize;
}

bool IpProtocol::Parse(const char *buffer, unsigned int size)
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP protocol detected" << std::endl;
        return false;
    }

    if (size < stringMinSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP protocol buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    unsigned int raw;
    int rc = sscanf(buffer, "%x", &raw);
    if (rc != 1) {
        std::cerr << __PRETTY_FUNCTION__ << ": Error reading IP protocol buffer" << std::endl;
        return false;
    }

    __raw = static_cast<unsigned char>(raw);

    return true;
}

bool IpProtocol::ToString(char *buffer, unsigned int size) const
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP protocol detected" << std::endl;
        return false;
    }

    if (size < stringMinSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP protocol buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    snprintf(buffer, size, "%02x", __raw);

    return true;
}

bool IpProtocol::operator==(const IpProtocol &right) const
{
    return (__raw == right.__raw);
}

bool IpProtocol::operator!=(const IpProtocol &right) const
{
    return !operator==(right);
}

IpProtocol IpProtocol::operator++()
{
    __raw++;

    return *this;
}

IpProtocol IpProtocol::operator++(int)
{
    IpProtocol protocol(*this);

    ++*this;

    return protocol;
}
