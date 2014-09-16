/*
 * ethernet.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#include "header_ethernet.h"
#include <stdio.h>
#include <string.h>


Header::Ethernet::Ethernet()
{
    Clear();
}

Header::Ethernet::Ethernet(const Ethernet &ethernet)
{
    VlanCollection::const_iterator iter;

    Clear();

    __dstMac = ethernet.__dstMac;
    __srcMac = ethernet.__srcMac;

    for (iter = ethernet.__vlans.begin(); iter != ethernet.__vlans.end(); iter++) {
        __vlans.push_back(*iter);
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
    __vlans.clear();
    __ethertype = 0;
}

bool Header::Ethernet::GetRaw(unsigned char *ethernet, unsigned int size, unsigned int *offset) const
{
    VlanCollection::const_iterator iter;

    if (ethernet == NULL) {
        return false;
    }

    unsigned int vlansSize = __vlans.size() * Vlan::rawSize;
    if (size < (rawMinSize + vlansSize)) {
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
    for (iter = __vlans.begin(); iter != __vlans.end(); iter++) {
        rc = iter->GetRaw(&ethernetTemp[currentOffset], Vlan::rawSize);
        if (!rc) {
            return false;
        }

        currentOffset += Vlan::rawSize;
    }

    memcpy(&ethernetTemp[currentOffset], &__ethertype, sizeof(__ethertype));
    memcpy(ethernet, ethernetTemp, size);

    if (offset != NULL) {
        currentOffset += sizeof(__ethertype);
        *offset = currentOffset;
    }

    return true;
}

bool Header::Ethernet::SetRaw(const unsigned char *ethernet, unsigned int size, unsigned int *offset)
{
    Clear();

    if (ethernet == NULL) {
        return false;
    }

    if (size < rawMinSize) {
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

Header::Ethernet::VlanCollection Header::Ethernet::GetVlans() const
{
    return __vlans;
}

bool Header::Ethernet::AddVlan(const Vlan &vlan)
{
    __vlans.push_back(vlan);

    return true;
}

bool Header::Ethernet::RemoveVlan(const Vlan &vlan)
{
    VlanCollection::iterator iter;

    for (iter = __vlans.begin(); iter != __vlans.end(); iter++) {
        if (*iter == vlan) {
            __vlans.erase(iter);

            return true;
        }
    }

    return false;
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
