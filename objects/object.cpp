/*
 * object.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#include "object.h"


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
