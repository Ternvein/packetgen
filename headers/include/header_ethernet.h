/*
 * ethernet.h
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#ifndef HEADER_ETHERNET_H_
#define HEADER_ETHERNET_H_

#include "macaddress.h"
#include "ethertype.h"
#include "vlancollection.h"

#include <vector>


namespace Header {

class Ethernet
{
public:
    static const unsigned int rawMinSize = 15;

    static const Ethertype ethertypeArp;
    static const Ethertype ethertypeIpv4;

private:
    static const unsigned int dstMacOffset = 0;
    static const unsigned int srcMacOffset = 6;
    static const unsigned int ethertypeOffset = 12;

    MacAddress __dstMac;
    MacAddress __srcMac;

    VlanCollection __vlans;

    Ethertype __ethertype;

public:
    Ethernet();
    Ethernet(const Ethernet &ethernet);
    ~Ethernet();

    void Clear();

    bool GetRaw(unsigned char *ethernet, unsigned int size, unsigned int *offset) const;
    bool SetRaw(const unsigned char *ethernet, unsigned int size, unsigned int *offset);

    MacAddress GetDstMac() const;
    bool SetDstMac(const MacAddress &mac);

    MacAddress GetSrcMac() const;
    bool SetSrcMac(const MacAddress &mac);

    VlanCollection GetVlans() const;
    bool SetVlans(const VlanCollection &vlans);
    bool AddVlan(const Vlan &vlan);
    bool RemoveVlan(const Vlan &vlan);
    bool IsVlanPresent(const Vlan &vlan) const;

    Ethertype GetEthertype() const;
    bool SetEthertype(const Ethertype &ethertype);

    bool operator==(const Ethernet &right) const;
    bool operator!=(const Ethernet &right) const;
};

}


#endif /* HEADER_ETHERNET_H_ */
