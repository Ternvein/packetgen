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

bool Object::ConvertToRaw(unsigned char *buffer, unsigned long long int object)
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    unsigned int offset = 0;
    for (int i = sizeof(object) - 1; i >= 0; i--) {
        buffer[offset] = (object & GetByteMask(i)) >> (i * bitsInByte);
        offset++;
    }

    return true;
}

bool Object::ConvertToRaw(unsigned char *buffer, unsigned int object)
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    unsigned int offset = 0;
    for (int i = sizeof(object) - 1; i >= 0; i--) {
        buffer[offset] = (object & GetByteMask(i)) >> (i * bitsInByte);
        offset++;
    }

    return true;
}

bool Object::ConvertToRaw(unsigned char *buffer, unsigned short object)
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    unsigned int offset = 0;
    for (int i = sizeof(object) - 1; i >= 0; i--) {
        buffer[offset] = (object & GetByteMask(i)) >> (i * bitsInByte);
        offset++;
    }

    return true;
}

bool Object::ConvertToRaw(unsigned char *buffer, unsigned char object)
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    buffer[0] = object;

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
