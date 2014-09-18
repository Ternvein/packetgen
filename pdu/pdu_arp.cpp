/*
 * arp.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#include "pdu_arp.h"

#include <stdio.h>
#include <string.h>
#include <iostream>


const char Pdu::Arp::defaultDstMac[] = "FF:FF:FF:FF:FF:FF";
const char Pdu::Arp::defaultSrcMac[] = "00:13:AA:00:00:01";
const char Pdu::Arp::defaultDstIp[] = "172.17.1.1";
const char Pdu::Arp::defaultSrcIp[] = "172.17.1.100";

Pdu::Arp::Arp()
{
    Clear();
}

Pdu::Arp::Arp(const Arp &arp)
{
    Set(arp);
}

Pdu::Arp::~Arp()
{
    Clear();
}

void Pdu::Arp::Clear()
{
    __ethernet.Clear();

    __hardwareType = defaultHardwareType;
    __protocolType = defaultProtocolType;

    __hardwareSize = defaultHardwareSize;
    __protocolSize = defaultProtocolSize;

    __opcode = defaultOpcode;

    __senderIp.Parse(defaultSrcIp, sizeof(defaultSrcIp));

    __targetIp.Parse(defaultDstIp, sizeof(defaultDstIp));

    MacAddress srcMac(defaultSrcMac, sizeof(defaultSrcMac));
    MacAddress dstMac(defaultDstMac, sizeof(defaultDstMac));
    __ethernet.SetSrcMac(srcMac);
    __ethernet.SetDstMac(dstMac);
    __ethernet.SetEthertype(Header::Ethernet::ethertypeArp);
}

bool Pdu::Arp::Set(const Arp &arp)
{
    Clear();

    __ethernet = arp.__ethernet;
    __hardwareType = arp.__hardwareType;
    __protocolType = arp.__protocolType;
    __hardwareSize = arp.__hardwareSize;
    __protocolSize = arp.__protocolSize;
    __opcode = arp.__opcode;
    __senderIp = arp.__senderIp;
    __targetIp = arp.__targetIp;

    return true;
}

bool Pdu::Arp::GetRaw(unsigned char *arp, unsigned int size, unsigned int *offset) const
{
    if (arp == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL packet detected" << std::endl;
        return false;
    }

    if (size < (dataMinSize + Header::Ethernet::rawMinSize)) {
        std::cerr << __PRETTY_FUNCTION__ << ": Packet buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    unsigned char arpTemp[size];
    unsigned int currentOffset = 0;
    bool rc;

    rc = __ethernet.GetRaw(arpTemp, size, &currentOffset);
    if (!rc) {
        return false;
    }

    memcpy(&arpTemp[currentOffset], &__hardwareType, sizeof(__hardwareType));
    currentOffset += sizeof(__hardwareType);

    memcpy(&arpTemp[currentOffset], &__protocolType, sizeof(__protocolType));
    currentOffset += sizeof(__protocolType);

    memcpy(&arpTemp[currentOffset], &__hardwareSize, sizeof(__hardwareSize));
    currentOffset += sizeof(__hardwareSize);

    memcpy(&arpTemp[currentOffset], &__protocolSize, sizeof(__protocolSize));
    currentOffset += sizeof(__protocolSize);

    memcpy(&arpTemp[currentOffset], &__opcode, sizeof(__opcode));
    currentOffset += sizeof(__opcode);

    rc = __ethernet.GetSrcMac().GetRaw(&arpTemp[currentOffset], size - currentOffset);
    if (!rc) {
        return false;
    }
    currentOffset += MacAddress::rawSize;

    rc = __senderIp.GetRaw(&arpTemp[currentOffset], size - currentOffset);
    if (!rc) {
        return false;
    }
    currentOffset += IpAddress::rawSize;

    rc = __ethernet.GetDstMac().GetRaw(&arpTemp[currentOffset], size - currentOffset);
    if (!rc) {
        return false;
    }
    currentOffset += MacAddress::rawSize;

    rc = __targetIp.GetRaw(&arpTemp[currentOffset], size - currentOffset);
    if (!rc) {
        return false;
    }

    memcpy(arp, arpTemp, size);

    if (offset != NULL) {
        currentOffset += IpAddress::rawSize;
        *offset = currentOffset;
    }

    return true;
}

bool Pdu::Arp::SetRaw(const unsigned char *arp, unsigned int size, unsigned int *offset)
{
    Clear();

    if (arp == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL packet detected" << std::endl;
        return false;
    }

    if (size < (dataMinSize + Header::Ethernet::rawMinSize)) {
        std::cerr << __PRETTY_FUNCTION__ << ": Packet buffer size " << size << " is too short" << std::endl;
        return false;
    }

    unsigned int currentOffset = 0;
    bool rc;

    rc = __ethernet.SetRaw(&arp[currentOffset], size, &currentOffset);
    if (!rc) {
        return false;
    }

    __hardwareType = *reinterpret_cast<const unsigned short *>(&arp[currentOffset]);
    currentOffset += sizeof(__hardwareType);

    __protocolType = *reinterpret_cast<const Header::Ethernet::Ethertype *>(&arp[currentOffset]);
    currentOffset += sizeof(__protocolType);

    __hardwareSize = *reinterpret_cast<const unsigned char *>(&arp[currentOffset]);
    currentOffset += sizeof(__hardwareSize);

    __protocolSize = *reinterpret_cast<const unsigned char *>(&arp[currentOffset]);
    currentOffset += sizeof(__protocolSize);

    __opcode = *reinterpret_cast<const Opcode *>(&arp[currentOffset]);
    currentOffset += sizeof(__opcode);

    MacAddress mac;
    rc = mac.SetRaw(&arp[currentOffset], size - currentOffset);
    if (!rc) {
        Clear();
        return false;
    }

    rc = SetSrcMac(mac);
    if (!rc) {
        Clear();
        return false;
    }
    currentOffset += MacAddress::rawSize;

    IpAddress ip;
    rc = ip.SetRaw(&arp[currentOffset], size - currentOffset);
    if (!rc) {
        Clear();
        return false;
    }

    rc = SetSrcIp(ip);
    if (!rc) {
        Clear();
        return false;
    }
    currentOffset += IpAddress::rawSize;

    rc = mac.SetRaw(&arp[currentOffset], size - currentOffset);
    if (!rc) {
        Clear();
        return false;
    }

    rc = SetDstMac(mac);
    if (!rc) {
        Clear();
        return false;
    }
    currentOffset += MacAddress::rawSize;

    rc = ip.SetRaw(&arp[currentOffset], size - currentOffset);
    if (!rc) {
        Clear();
        return false;
    }

    rc = SetDstIp(ip);
    if (!rc) {
        Clear();
        return false;
    }

    if (offset != NULL) {
        currentOffset += IpAddress::rawSize;
        *offset = currentOffset;
    }

    return true;
}

MacAddress Pdu::Arp::GetDstMac() const
{
    return __ethernet.GetDstMac();
}

bool Pdu::Arp::SetDstMac(const MacAddress &mac)
{
    return __ethernet.SetDstMac(mac);
}

MacAddress Pdu::Arp::GetSrcMac() const
{
    return __ethernet.GetSrcMac();
}

bool Pdu::Arp::SetSrcMac(const MacAddress &mac)
{
    return __ethernet.SetSrcMac(mac);
}

VlanCollection Pdu::Arp::GetVlans() const
{
    return __ethernet.GetVlans();
}

bool Pdu::Arp::AddVlan(const Vlan &vlan)
{
    return __ethernet.AddVlan(vlan);
}

bool Pdu::Arp::RemoveVlan(const Vlan &vlan)
{
    return __ethernet.RemoveVlan(vlan);
}

IpAddress Pdu::Arp::GetDstIp() const
{
    return __targetIp;
}

bool Pdu::Arp::SetDstIp(const IpAddress &ip)
{
    return __targetIp.Set(ip);
}

IpAddress Pdu::Arp::GetSrcIp() const
{
    return __senderIp;
}

bool Pdu::Arp::SetSrcIp(const IpAddress &ip)
{
    return __senderIp.Set(ip);
}

unsigned short Pdu::Arp::GetHardwareType() const
{
    return __hardwareType;
}

bool Pdu::Arp::SetHardwareType(unsigned short hardware)
{
    __hardwareType = hardware;

    return true;
}

Header::Ethernet::Ethertype Pdu::Arp::GetProtocolType() const
{
    return __protocolType;
}

bool Pdu::Arp::SetProtocolType(const Header::Ethernet::Ethertype &protocol)
{
    __protocolType = protocol;

    return true;
}

Pdu::Arp::Opcode Pdu::Arp::GetOpcode() const
{
    return __opcode;
}

bool Pdu::Arp::SetOpcode(const Opcode &opcode)
{
    __opcode = opcode;

    return true;
}

bool Pdu::Arp::operator==(const Arp &right) const
{
    if (__ethernet != right.__ethernet) {
        return false;
    }

    if (__hardwareType != right.__hardwareType) {
        return false;
    }

    if (__protocolType != right.__protocolType) {
        return false;
    }

    if (__hardwareSize != right.__hardwareSize) {
        return false;
    }

    if (__protocolSize != right.__protocolSize) {
        return false;
    }

    if (__opcode != right.__opcode) {
        return false;
    }

    if (__senderIp != right.__senderIp) {
        return false;
    }

    if (__targetIp != right.__targetIp) {
        return false;
    }

    return true;
}

bool Pdu::Arp::operator!=(const Arp &right) const
{
    return !operator==(right);
}
