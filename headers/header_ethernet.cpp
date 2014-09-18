/*
 * ethernet.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#include "header_ethernet.h"

#include <stdio.h>
#include <string.h>
#include <iostream>


const Header::Ethernet::Ethertype Header::Ethernet::ethertypeArp;
const Header::Ethernet::Ethertype Header::Ethernet::ethertypeIpv4;

Header::Ethernet::Ethernet()
{
    Clear();
}

Header::Ethernet::Ethernet(const Ethernet &ethernet)
{
    VlanCollection::ConstIterator iter;

    Clear();

    __dstMac = ethernet.__dstMac;
    __srcMac = ethernet.__srcMac;

    for (iter = ethernet.__vlans.Begin(); iter != ethernet.__vlans.End(); iter++) {
        __vlans.Add(*iter);
    }

    __ethertype = ethernet.__ethertype;
}

Header::Ethernet::~Ethernet()
{
    Clear();
}

void Header::Ethernet::Clear()
{
    __dstMac.Clear();
    __srcMac.Clear();
    __vlans.Clear();
    __ethertype = 0;
}

bool Header::Ethernet::GetRaw(unsigned char *ethernet, unsigned int size, unsigned int *offset) const
{
    VlanCollection::ConstIterator iter;

    if (ethernet == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL header detected" << std::endl;
        return false;
    }

    unsigned int vlansSize = __vlans.GetCount() * Vlan::rawSize;
    if (size < (rawMinSize + vlansSize)) {
        std::cerr << __PRETTY_FUNCTION__ << ": Header buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    unsigned char ethernetTemp[size];
    bool rc;

    rc = __dstMac.GetRaw(&ethernetTemp[dstMacOffset], MacAddress::rawSize);
    if (!rc) {
        return false;
    }

    rc = __srcMac.GetRaw(&ethernetTemp[srcMacOffset], MacAddress::rawSize);
    if (!rc) {
        return false;
    }

    unsigned int currentOffset = ethertypeOffset;
    for (iter = __vlans.Begin(); iter != __vlans.End(); iter++) {
        rc = iter->GetRaw(&ethernetTemp[currentOffset], Vlan::rawSize);
        if (!rc) {
            return false;
        }

        currentOffset += Vlan::rawSize;
    }

    for (int i = sizeof(__ethertype) - 1; i >= 0; i--) {
        ethernetTemp[currentOffset] = (__ethertype & (0xFF << (i * 8))) >> i * 8;
        currentOffset++;
    }

    memcpy(ethernet, ethernetTemp, size);

    if (offset != NULL) {
        *offset = currentOffset;
    }

    return true;
}

bool Header::Ethernet::SetRaw(const unsigned char *ethernet, unsigned int size, unsigned int *offset)
{
    Clear();

    if (ethernet == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL header detected" << std::endl;
        return false;
    }

    if (size < rawMinSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Header buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    bool rc;
    rc = __dstMac.SetRaw(&ethernet[dstMacOffset], MacAddress::rawSize);
    if (!rc) {
        return false;
    }

    rc = __srcMac.SetRaw(&ethernet[srcMacOffset], MacAddress::rawSize);
    if (!rc) {
        Clear();
        return false;
    }

    Ethertype ethertype;
    Vlan vlan;
    unsigned int currentOffset = ethertypeOffset;
    while (currentOffset < size) {
        ethertype = *reinterpret_cast<const Ethertype *>(&ethernet[currentOffset]);
        switch (ethertype) {
        case Vlan::tpidCtag:
        case Vlan::tpidStag:
            if ((size - currentOffset) < Vlan::rawSize) {
                std::cerr << __PRETTY_FUNCTION__ << ": Header buffer size " << size
                          << " is too short to read VLAN" << std::endl;
                return false;
            }

            rc = vlan.SetRaw(&ethernet[currentOffset], Vlan::rawSize);
            if (!rc) {
                Clear();
                return false;
            }

            rc = AddVlan(vlan);
            if (!rc) {
                Clear();
                return false;
            }

            currentOffset += Vlan::rawSize;

            break;
        default:
            __ethertype = ethertype;

            if (offset != NULL) {
                currentOffset += sizeof(__ethertype);
                *offset = currentOffset;
            }

            return true;
        }
    }

    return false;
}

MacAddress Header::Ethernet::GetDstMac() const
{
    return __dstMac;
}

bool Header::Ethernet::SetDstMac(const MacAddress &mac)
{
    return __dstMac.Set(mac);
}

MacAddress Header::Ethernet::GetSrcMac() const
{
    return __srcMac;
}

bool Header::Ethernet::SetSrcMac(const MacAddress &mac)
{
    return __srcMac.Set(mac);
}

VlanCollection Header::Ethernet::GetVlans() const
{
    return __vlans;
}

bool Header::Ethernet::SetVlans(const VlanCollection &vlans)
{
    __vlans = vlans;

    return true;
}

bool Header::Ethernet::AddVlan(const Vlan &vlan)
{
    return __vlans.Add(vlan);
}

bool Header::Ethernet::RemoveVlan(const Vlan &vlan)
{
    return __vlans.Remove(vlan);
}

bool Header::Ethernet::IsVlanPresent(const Vlan &vlan) const
{
    return __vlans.IsMember(vlan);
}

Header::Ethernet::Ethertype Header::Ethernet::GetEthertype() const
{
    return __ethertype;
}

bool Header::Ethernet::SetEthertype(const Ethertype &ethertype)
{
    if (ethertype == Vlan::tpidCtag || ethertype == Vlan::tpidStag) {
        return false;
    }

    __ethertype = ethertype;

    return true;
}

bool Header::Ethernet::operator==(const Ethernet &right) const
{
    if (__dstMac != right.__dstMac) {
        return false;
    }

    if (__srcMac != right.__srcMac) {
        return false;
    }

    if (__ethertype != right.__ethertype) {
        return false;
    }

    if (__vlans != right.__vlans) {
        return false;
    }

    return true;
}

bool Header::Ethernet::operator!=(const Ethernet &right) const
{
    return !operator==(right);
}
