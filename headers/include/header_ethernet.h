/*
 * ethernet.h
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#ifndef HEADER_ETHERNET_H_
#define HEADER_ETHERNET_H_

#include "macaddress.h"
#include "vlan.h"
#include <vector>


namespace Header {

class Ethernet
{
public:
    typedef unsigned short Ethertype;
    typedef std::vector<Vlan> VlanCollection;

    static const unsigned int rawMinSize = 15;

    static const Ethertype ethertypeArp = 0x0806;
    static const Ethertype ethertypeIpv4 = 0x0800;

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
    bool AddVlan(const Vlan &vlan);
    bool RemoveVlan(const Vlan &vlan);

    Ethertype GetEthertype() const;
    bool SetEthertype(const Ethertype &ethertype);
};

}


#endif /* HEADER_ETHERNET_H_ */
