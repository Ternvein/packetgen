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

protected:
    static unsigned int GetByteMask(unsigned int offset);

    static bool ConvertToRaw(unsigned char *buffer, const unsigned long long int object);
    static bool ConvertToRaw(unsigned char *buffer, const unsigned int object);
    static bool ConvertToRaw(unsigned char *buffer, const unsigned short object);

public:
    virtual ~Object();
};


#endif /* OBJECT_H_ */
