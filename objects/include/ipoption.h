/*
 * ipoption.h
 *
 *  Created on: Sep 19, 2014
 *      Author: elestar
 */

#ifndef IPOPTION_H_
#define IPOPTION_H_

#include "object.h"


class IpOption : public Object
{
public:
    static const unsigned int rawMinSize = 2;
    static const unsigned int stringMinSize = 5;

private:
    unsigned char __header;
    unsigned char *__data;

    unsigned int __dataSize;

public:
    IpOption();
    IpOption(const IpOption &option);
    ~IpOption();

    void Clear();

    void Set(const IpOption &option);

    bool GetRaw(unsigned char *buffer, unsigned int size) const;
    bool SetRaw(const unsigned char *buffer, unsigned int size);

    unsigned int GetStringSize() const;
    bool Parse(const char *buffer, unsigned int size);
    bool ToString(char *buffer, unsigned int size) const;

    bool operator==(const IpOption &right) const;
    bool operator!=(const IpOption &right) const;
    IpOption operator++();
    IpOption operator++(int);
};


#endif /* IPOPTION_H_ */
