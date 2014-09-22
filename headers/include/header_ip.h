/*
 * header_ip.h
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#ifndef HEADER_IP_H_
#define HEADER_IP_H_

#include "ipaddress.h"


namespace Header {

class Ip
{
public:
    static const unsigned int rawMinSize = 15;

    static const unsigned int ethertypeArp = 0x0806;
    static const unsigned int ethertypeIpv4 = 0x0800;

private:
    static const unsigned int versionOffset = 0;
    static const unsigned int headerLengthOffset = 0;
    static const unsigned int dscpOffset = 1;
    static const unsigned int ecnOffset = 1;
    static const unsigned int totalLengthOffset = 2;
    static const unsigned int identificationOffset = 4;
    static const unsigned int flagsOffset = 6;
    static const unsigned int fragmentFieldOffset = 6;
    static const unsigned int ttlOffset = 8;
    static const unsigned int ipProtoOffset = 9;
    static const unsigned int checksumOffset = 10;
    static const unsigned int srcIpOffset = 12;
    static const unsigned int dstIpOffset = 16;
    static const unsigned int optionsOffset = 20;

    static const unsigned int versionMask = 0xF0;
    static const unsigned int headerLengthMask = 0x0F;
    static const unsigned int dscpMask = 0xFC;
    static const unsigned int ecnMask = 0x03;
    static const unsigned int flagsMask = 0xE0;
    static const unsigned int fragmentFieldMask = 0x1FFF;

    unsigned char __version;
    unsigned char __headerLength;
    unsigned char __dscp;
    unsigned char __ecn;
    unsigned short __totalLength;
    unsigned short __identification;

    bool __isDontFragment;
    bool __isMoreFragments;

    unsigned short __fragmentOffset;
    unsigned char __ttl;

    IpProtocol __ipProto;

    unsigned short __checksum;

    IpAddress __srcIp;
    IpAddress __dstIp;

public:
    Ip();
    Ip(const Ip &ip);
    ~Ip();

    void Clear();
};

}


#endif /* HEADER_IP_H_ */
