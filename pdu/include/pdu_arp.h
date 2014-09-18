/*
 * arp.h
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#ifndef PDU_ARP_H_
#define PDU_ARP_H_

#include "header_ethernet.h"
#include "ipaddress.h"


namespace Pdu {

class Arp
{
public:
    typedef enum
    {
        OPCODE_NONE = 0,
        OPCODE_REQUEST = 1,
        OPCODE_REPLY = 2,
        OPCODE_REQUEST_REVERSE = 3,
        OPCODE_REPLY_REVERSE = 4
    } Opcode;

private:
    static const char defaultDstMac[];
    static const char defaultSrcMac[];
    static const char defaultDstIp[];
    static const char defaultSrcIp[];

    static const unsigned short defaultHardwareType = 1;
    static const unsigned int defaultProtocolType = Header::Ethernet::ethertypeIpv4;

    static const unsigned char defaultHardwareSize = MacAddress::rawSize;
    static const unsigned char defaultProtocolSize = IpAddress::rawSize;

    static const unsigned int dataMinSize = 28;

    static const Opcode defaultOpcode = OPCODE_REQUEST;

    Header::Ethernet __ethernet;

    unsigned short __hardwareType;
    Ethertype __protocolType;

    unsigned char __hardwareSize;
    unsigned char __protocolSize;

    Opcode __opcode;

    IpAddress __senderIp;
    IpAddress __targetIp;

public:
    Arp();
    Arp(const Arp &arp);
    ~Arp();

    void Clear();

    bool Set(const Arp &arp);

    bool GetRaw(unsigned char *arp, unsigned int size, unsigned int *offset) const;
    bool SetRaw(const unsigned char *arp, unsigned int size, unsigned int *offset);

    MacAddress GetDstMac() const;
    bool SetDstMac(const MacAddress &mac);

    MacAddress GetSrcMac() const;
    bool SetSrcMac(const MacAddress &mac);

    VlanCollection GetVlans() const;
    bool AddVlan(const Vlan &vlan);
    bool RemoveVlan(const Vlan &vlan);

    IpAddress GetDstIp() const;
    bool SetDstIp(const IpAddress &ip);

    IpAddress GetSrcIp() const;
    bool SetSrcIp(const IpAddress &ip);

    unsigned short GetHardwareType() const;
    bool SetHardwareType(unsigned short hardware);

    Ethertype GetProtocolType() const;
    bool SetProtocolType(const Ethertype &protocol);

    Opcode GetOpcode() const;
    bool SetOpcode(const Opcode &protocol);

    bool operator==(const Arp &right) const;
    bool operator!=(const Arp &right) const;
};

}

#endif /* PDU_ARP_H_ */
