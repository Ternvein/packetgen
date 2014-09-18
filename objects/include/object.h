/*
 * object.h
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <stdio.h>
#include <iostream>


class Object
{
    static const unsigned int bitsInByte = 8;
    static const unsigned int byteMask = 0xFF;

    static const char toStringNotSupported[];

protected:
    static unsigned int GetByteMask(unsigned int offset);

    static void ToStringNotSupported(char *buffer, unsigned int size);

public:
    virtual ~Object();

    static bool ConvertToRaw(unsigned char *buffer, const unsigned long long int object);
    static bool ConvertToRaw(unsigned char *buffer, const unsigned int object);
    static bool ConvertToRaw(unsigned char *buffer, const unsigned short object);

    virtual unsigned int GetStringSize() const = 0;
    virtual bool Parse(const char *buffer, unsigned int size) = 0;
    virtual bool ToString(char *buffer, unsigned int size) const = 0;
};


#endif /* OBJECT_H_ */
