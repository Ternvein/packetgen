/*
 * object.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#include "object.h"


const char Object::toStringNotSupported[] = "Output not supported";

Object::~Object()
{

}

unsigned int Object::GetByteMask(unsigned int offset)
{
    return byteMask << (offset * bitsInByte);
}

bool Object::ConvertToRaw(unsigned char *buffer, const unsigned long long int object)
{
    unsigned int offset = 0;
    for (int i = sizeof(object) - 1; i >= 0; i--) {
        buffer[offset] = (object & GetByteMask(i)) >> (i * bitsInByte);
        offset++;
    }

    return true;
}

bool Object::ConvertToRaw(unsigned char *buffer, const unsigned int object)
{
    unsigned int offset = 0;
    for (int i = sizeof(object) - 1; i >= 0; i--) {
        buffer[offset] = (object & GetByteMask(i)) >> (i * bitsInByte);
        offset++;
    }

    return true;
}

bool Object::ConvertToRaw(unsigned char *buffer, const unsigned short object)
{
    unsigned int offset = 0;
    for (int i = sizeof(object) - 1; i >= 0; i--) {
        buffer[offset] = (object & GetByteMask(i)) >> (i * bitsInByte);
        offset++;
    }

    return true;
}

void Object::ToStringNotSupported(char *buffer, unsigned int size)
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return;
    }

    if (size < sizeof(toStringNotSupported)) {
        std::cerr << __PRETTY_FUNCTION__ << ": buffer size " << size
                  << " is too short" << std::endl;
        return;
    }

    snprintf(buffer, size, "%s", toStringNotSupported);
}
