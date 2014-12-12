/*
 * test.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#include "pdu_arp.h"
#include "pdu_icmp.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>


void PrintEthernet(const Header::Ethernet &ethernet)
{
    char buffer[100];

    ethernet.GetDstMac().ToString(buffer, sizeof(buffer));
    std::cout << "DstMac: " << buffer << std::endl;
    ethernet.GetSrcMac().ToString(buffer, sizeof(buffer));
    std::cout << "SrcMac: " << buffer << std::endl;
    VlanCollection vlans = ethernet.GetVlans();
    VlanCollection::ConstIterator iter;
    for (iter = vlans.Begin(); iter != vlans.End(); iter++) {
        std::cout << "__________" << std::endl;
        std::cout << "CoS: " << iter->GetCos() << std::endl;
        std::cout << "Dropped: " << (iter->IsDei() ? "yes" : "no") << std::endl;
        std::cout << "VID: " << iter->GetVid() << std::endl;
    }
    std::cout << "__________" << std::endl;
    ethernet.GetEthertype().ToString(buffer, sizeof(buffer));
    std::cout << "Ethertype: 0x" << buffer << std::endl;
}

void PrintIp(const Header::Ip &ip)
{
    char buffer[100];

    ip.GetSrcIp().ToString(buffer, sizeof(buffer));
    std::cout << "Source IP: " << buffer << std::endl;
    ip.GetDstIp().ToString(buffer, sizeof(buffer));
    std::cout << "Destination IP: " << buffer << std::endl;
    std::cout << "Header length: " << ip.GetHeaderLength() << std::endl;
    std::cout << "Total length: " << ip.GetTotalLength() << std::endl;
    std::cout << "TTL: " << ip.GetTtl() << std::endl;
    ip.GetIpProtocol().ToString(buffer, sizeof(buffer));
    std::cout << "IP protocol: 0x" << buffer << std::endl;
    std::cout << "Checksum: 0x" << std::hex << ip.GetChecksum() << std::dec << std::endl;
}

int main()
{
    Pdu::Arp arp;
    Pdu::Icmp icmp;

    unsigned char pdu[50];
    unsigned int offset = 0;

    memset(pdu, 0, sizeof(pdu));

    bool rc = arp.GetRaw(pdu, sizeof(pdu), &offset);
    if (!rc) {
        std::cerr << "Main: Can't get raw ARP" << std::endl;
    }

    char buffer[100];
    std::cout << "Statistic ARP PDU 1:" << std::endl;
    PrintEthernet(arp);
    arp.GetProtocolType().ToString(buffer, sizeof(buffer));
    std::cout << "ProtocolType: 0x" <<  buffer << std::endl;
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

    memset(pdu, 0, sizeof(pdu));
    memset(buffer, 0, sizeof(buffer));
    offset = 0;

    mac.Parse("b8:a3:86:73:a2:a2", 18);
    icmp.SetSrcMac(mac);
    mac.Parse("01:00:5e:0a:0a:1e", 18);
    icmp.SetDstMac(mac);
    ip.Parse("192.168.1.1", 12);
    icmp.SetSrcIp(ip);
    icmp.SetDstIp(++ip);
    unsigned int data = 0x00010001;
    data = htonl(data);
    icmp.SetRest(data);
    icmp.SetTotalLength(icmp.GetSize());
    data = htonl(0xFFFFFFFF);
    icmp.SetData(reinterpret_cast<unsigned char *>(&data), sizeof(data));
    icmp.SetTotalLength(icmp.GetSize());
    Header::Ip *hIp = dynamic_cast<Header::Ip *>(&icmp);
    std::cerr << std::hex << hIp->CalculateChecksum() << std::dec << std::endl;
    hIp->SetChecksum(hIp->CalculateChecksum());
    std::cerr << std::hex << icmp.CalculateChecksum() << std::dec << std::endl;
    icmp.SetChecksum(icmp.CalculateChecksum());
    std::cout << std::endl;
    std::cout << "Statistic ICMP PDU 1:" << std::endl;
    PrintEthernet(icmp);
    PrintIp(icmp);

    rc = icmp.GetRaw(pdu, sizeof(pdu), &offset);
    if (!rc) {
        std::cerr << "Main: Can't get raw ICMP" << std::endl;
    }

    std::cout << "ICMP PDU size: " << offset << std::endl;
    for (unsigned int i = 0; i < offset; i++) {
        printf("%02x ", pdu[i]);
    }
    printf("\r\n");



    memset(pdu, 0, sizeof(pdu));
    memset(buffer, 0, sizeof(buffer));
    offset = 0;

    std::cout << std::endl;
    std::cout << "Statistic ICMP PDU 2:" << std::endl;
    PrintEthernet(icmp);
    PrintIp(icmp);

    rc = icmp.GetRaw(pdu, sizeof(pdu), &offset);
    if (!rc) {
        std::cerr << "Main: Can't get raw ICMP" << std::endl;
    }

    std::cout << "ICMP PDU size: " << offset << std::endl;
    for (unsigned int i = 0; i < offset; i++) {
        printf("%02x ", pdu[i]);
    }
    printf("\r\n");

    return 0;
}
