/*
 * ipoption.cpp
 *
 *  Created on: Sep 19, 2014
 *      Author: elestar
 */

#include "ipoption.h"

#include <stdio.h>
#include <string.h>


IpOption::IpOption()
{
    __data = NULL;

    Clear();
}

IpOption::IpOption(const IpOption &option)
{
    Set(option);
}

IpOption::~IpOption()
{
    Clear();
}

void IpOption::Clear()
{
    __header = 0;

    if (__data != NULL) {
        delete[] __data;
        __data = NULL;
    }

    __dataSize = 0;
}

void IpOption::Set(const IpOption &option)
{
    Clear();

    __header = option.__header;
    __data = new unsigned char [option.__dataSize];
    __dataSize = option.__dataSize;
}

bool IpOption::GetRaw(unsigned char *buffer, unsigned int size) const
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP option detected" << std::endl;
        return false;
    }

    if (size < (rawMinSize + __dataSize)) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP option buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    unsigned char raw[size];
    bool rc = ConvertToRaw(raw, __header);
    if (!rc) {
        std::cerr << __PRETTY_FUNCTION__ << ": Error writing IP option header to buffer" << std::endl;
        return false;
    }
    unsigned int offset = sizeof(__header);

    for (unsigned int i = 0; i < __dataSize; i++) {
        if (offset >= size) {
            std::cerr << __PRETTY_FUNCTION__ << ": IP option buffer size " << size
                      << " is too short" << std::endl;
            return false;
        }

        raw[offset] = __data[i];

        offset++;
    }

    memcpy(buffer, raw, size);

    return true;
}

bool IpOption::SetRaw(const unsigned char *buffer, unsigned int size)
{
    Clear();

    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL IP option detected" << std::endl;
        return false;
    }

    if (size < rawMinSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": IP option buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    __header = buffer[0];
    __data = new unsigned char;
    __data[0] = buffer[1];

    return true;
}

unsigned int IpOption::GetStringSize() const
{
    return stringMinSize;
}

bool IpOption::Parse(const char *buffer, unsigned int size)
{
    return false;
}

bool IpOption::ToString(char *buffer, unsigned int size) const
{
    return ToStringNotSupported(buffer, size);
}

void IpOption::EnableCopied()
{
    __header |= copiedMask;
}

void IpOption::DisableCopied()
{
    __header &= ~copiedMask;
}

bool IpOption::IsCopied() const
{
    if (__header & copiedMask) {
        return true;
    }

    return false;
}

bool IpOption::SetClass(unsigned int optionClass)
{
    __header &= ~classMask;
    __header |= (optionClass << classOffset) & classMask;

    return true;
}

unsigned int IpOption::GetClass() const
{
    return (__header & classMask) >> classOffset;
}

bool IpOption::SetNumber(unsigned int number)
{
    __header &= ~numberMask;
    __header |= (number & numberMask);

    return true;
}

unsigned int IpOption::GetNumber() const
{
    return (__header & numberMask);
}

bool IpOption::operator==(const IpOption &right) const
{
    if (__header != right.__header) {
        return false;
    }

    if (__dataSize != right.__dataSize) {
        return false;
    }

    int rc = memcmp(__data, right.__data, __dataSize);
    if (rc != 0) {
        return false;
    }

    return true;
}

bool IpOption::operator!=(const IpOption &right) const
{
    return !operator==(right);
}
