/*
 * header_ip.h
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#ifndef HEADER_IP_H_
#define HEADER_IP_H_

#include "ipaddress.h"
#include "ipprotocol.h"


namespace Header {

class Ip
{
public:
    static const unsigned int rawMinSize = 20;

    static const unsigned int versionIpv4 = 4;

    static const unsigned int defaultTtl = 128;

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
    static const unsigned int flagDfMask = 0x40;
    static const unsigned int flagMfMask = 0x20;
    static const unsigned int fragmentFieldMask = 0x1FFF;

    static const unsigned int versionMaskOffset = 4;
    static const unsigned int dscpMaskOffset = 2;

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

protected:
    static unsigned short CalculateChecksum(const unsigned char *buffer, unsigned int size);

public:
    Ip();
    Ip(const Ip &ip);
    ~Ip();

    void Clear();

    void Set(const Ip &ip);

    bool GetRaw(unsigned char *buffer, unsigned int size, unsigned int *offset) const;
    bool SetRaw(const unsigned char *buffer, unsigned int size);

    bool SetHeaderLength(unsigned int length);
    unsigned int GetHeaderLength() const;

    bool SetTotalLength(unsigned int length);
    unsigned int GetTotalLength() const;

    bool SetFlagDontFragment(bool flag);
    bool GetFlagDontFragment() const;

    bool SetFlagMoreFragments(bool flag);
    bool GetFlagMoreFragments() const;

    bool SetFragmentOffset(unsigned int offset);
    unsigned int GetFragmentOffset() const;

    bool SetTtl(unsigned int ttl);
    unsigned int GetTtl() const;

    bool SetIpProtocol(const IpProtocol &protocol);
    IpProtocol GetIpProtocol() const;

    bool SetChecksum(unsigned short checksum);
    unsigned short GetChecksum() const;
    unsigned short CalculateChecksum() const;
    bool IsChecksumValid() const;

    bool SetSourceIp(const IpAddress &ip);
    IpAddress GetSourceIp() const;

    bool SetDestinationIp(const IpAddress &ip);
    IpAddress GetDestinationIp() const;

    bool operator==(const Ip &right) const;
    bool operator!=(const Ip &right) const;
};

}


#endif /* HEADER_IP_H_ */
