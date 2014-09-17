/*
 * ipaddress.h
 *
 *  Created on: Sep 15, 2014
 *      Author: elestar
 */

#ifndef IPADDRESS_H_
#define IPADDRESS_H_


class IpAddress
{
public:
    static const unsigned int rawSize = 4;
    static const unsigned int stringSizeMin = 8;
    static const unsigned int stringSizeMax = 16;

private:
    static const unsigned int multicastIp = 0xE0000000;
    static const unsigned int multicastIpMask = 0xF0000000;

    static const unsigned int broadcastIp = 0xFFFFFFFF;

    static const unsigned int loopbackIp = 0x7F000000;
    static const unsigned int loopbackIpMask = 0xFF000000;

    static const unsigned int reservedIp = 0xF0000000;
    static const unsigned int reservedIpMask = 0xFF000000;

    unsigned char __raw[rawSize];
    bool __isMulticast;
    bool __isBroadcast;

protected:
    void CheckMulticast();
    void CheckBroadcast();

public:
    IpAddress();
    IpAddress(const IpAddress &ip);
    IpAddress(const unsigned char *ip, unsigned int size);
    IpAddress(const char *ip, unsigned int size);
    ~IpAddress();

    void Clear();

    bool GetRaw(unsigned char *ip, unsigned int size) const;
    bool SetRaw(const unsigned char *ip, unsigned int size);

    bool Set(const IpAddress &ip);

    bool Parse(const char *ip, unsigned int size);
    bool ToString(char *ip, unsigned int size) const;

    bool IsMulticast() const;
    bool IsBroadcast() const;

    bool operator==(const IpAddress &right) const;
    bool operator!=(const IpAddress &right) const;
    IpAddress operator++();
    IpAddress operator++(int);
};


#endif /* IPADDRESS_H_ */
