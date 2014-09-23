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
    static const unsigned char copiedMask = 0x80;
    static const unsigned char classMask = 0x60;
    static const unsigned char numberMask = 0x1F;

    static const unsigned int classOffset = 5;

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

    void EnableCopied();
    void DisableCopied();
    bool IsCopied() const;

    bool SetClass(unsigned int optionClass);
    unsigned int GetClass() const;

    bool SetNumber(unsigned int number);
    unsigned int GetNumber() const;

    bool operator==(const IpOption &right) const;
    bool operator!=(const IpOption &right) const;
};


#endif /* IPOPTION_H_ */
