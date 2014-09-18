/*
 * test.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#include "pdu_arp.h"

#include <stdio.h>
#include <string.h>
#include <iostream>


int main()
{
    Pdu::Arp arp;

    unsigned char pdu[50];
    unsigned int offset = 0;

    memset(pdu, 0, sizeof(pdu));

    bool rc = arp.GetRaw(pdu, sizeof(pdu), &offset);
    if (!rc) {
        std::cerr << "Main: Can't get raw ARP" << std::endl;
    }

    char buffer[100];
    std::cout << "Statistic PDU 1:" << std::endl;
    arp.GetDstMac().ToString(buffer, sizeof(buffer));
    std::cout << "DstMac: " << buffer << std::endl;
    arp.GetSrcMac().ToString(buffer, sizeof(buffer));
    std::cout << "SrcMac: " << buffer << std::endl;
    VlanCollection vlans = arp.GetVlans();
    VlanCollection::ConstIterator iter;
    for (iter = vlans.Begin(); iter != vlans.End(); iter++) {
        std::cout << "__________" << std::endl;
        std::cout << "CoS: " << iter->GetCos() << std::endl;
        std::cout << "Dropped: " << (iter->IsDei() ? "yes" : "no") << std::endl;
        std::cout << "VID: " << iter->GetVid() << std::endl;
    }
    std::cout << "__________" << std::endl;
    std::cout << "Ethertype: " << arp.GetProtocolType().Get() << std::endl;
    arp.GetSrcIp().ToString(buffer, sizeof(buffer));
    std::cout << "SenderIp: " << buffer << std::endl;
    arp.GetDstIp().ToString(buffer, sizeof(buffer));
    std::cout << "TargetIp: " << buffer << std::endl;

    MacAddress mac(arp.GetDstMac());
    mac++;
    arp.SetDstMac(mac);
    mac.Set(arp.GetSrcMac());
    mac++;
    arp.SetSrcMac(mac);

    Vlan vlan;
    arp.AddVlan(vlan);
    vlan.SetStag();
    vlan++;
    arp.AddVlan(vlan);

    IpAddress ip("255.255.255.255", 16);
    ip++;
    arp.SetDstIp(ip);
    ip.Set(arp.GetSrcIp());
    ip++;
    arp.SetSrcIp(ip);

    unsigned char pdu2[100];
    unsigned int offset2 = 0;

    memset(pdu2, 0, sizeof(pdu2));

    rc = arp.GetRaw(pdu2, sizeof(pdu2), &offset2);
    if (!rc) {
        std::cerr << "Main: Can't get raw ARP" << std::endl;
    }

    std::cout << "PDU size: " << offset << std::endl;
    for (unsigned int i = 0; i < offset; i++) {
        printf("%02x ", pdu[i]);
    }
    printf("\r\n");

    std::cout << "PDU changed size: " << offset2 << std::endl;
    for (unsigned int i = 0; i < offset2; i++) {
        printf("%02x ", pdu2[i]);
    }
    printf("\r\n");

    return 0;
}
