/*
 * ipoption.cpp
 *
 *  Created on: Sep 19, 2014
 *      Author: elestar
 */

#include "ipoption.h"


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

    unsigned char *raw;
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

    unsigned short header = *reinterpret_cast<const unsigned short *>(buffer);
    unsigned int dataSize = header &
}

unsigned int IpOption::GetStringSize() const;
bool IpOption::Parse(const char *buffer, unsigned int size);
bool IpOption::ToString(char *buffer, unsigned int size) const;

bool IpOption::operator==(const IpOption &right) const;
bool IpOption::operator!=(const IpOption &right) const;
IpOption IpOption::operator++();
IpOption IpOption::operator++(int);
