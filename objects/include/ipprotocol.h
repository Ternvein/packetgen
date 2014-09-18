/*
 * ipprotocol.h
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#ifndef IPPROTOCOL_H_
#define IPPROTOCOL_H_

#include "object.h"


class IpProtocol : public Object
{
    unsigned short __raw;

public:
    IpProtocol();
    IpProtocol(const IpProtocol &protocol);
    IpProtocol(unsigned short protocol);
    ~IpProtocol();

    void Set(const IpProtocol &protocol);
    unsigned short Get() const;

    bool GetRaw(unsigned char *buffer, unsigned int size) const;
    bool SetRaw(const unsigned char *buffer, unsigned int size);

    unsigned int GetStringSize() const;
    bool Parse(const char *buffer, unsigned int size);
    bool ToString(char *buffer, unsigned int size) const;

    bool operator==(const IpProtocol &right) const;
    bool operator!=(const IpProtocol &right) const;
    IpProtocol operator++();
    IpProtocol operator++(int);
};


#endif /* IPPROTOCOL_H_ */
