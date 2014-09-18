/*
 * vlancollection.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#include "vlancollection.h"


VlanCollection::VlanCollection()
{
    Clear();
}

VlanCollection::VlanCollection(const VlanCollection &vlans)
{
    Set(vlans);
}

VlanCollection::~VlanCollection()
{
    Clear();
}

void VlanCollection::Clear()
{
    __vlans.clear();
}

VlanCollection::ConstIterator VlanCollection::Begin() const
{
    return __vlans.begin();
}

VlanCollection::ConstIterator VlanCollection::End() const
{
    return __vlans.end();
}

void VlanCollection::Set(const VlanCollection &vlans)
{
    Clear();

    ConstIterator iter;
    for (iter = vlans.Begin(); iter != vlans.End(); iter++) {
        __vlans.push_back(*iter);
    }
}

bool VlanCollection::Add(const Vlan &vlan)
{
    __vlans.push_back(vlan);

    return true;
}

bool VlanCollection::Remove(const Vlan &vlan)
{
    Iterator iter;

    for (iter = __vlans.begin(); iter != __vlans.end(); iter++) {
        if (*iter == vlan) {
            __vlans.erase(iter);

            return true;
        }
    }

    return false;
}

bool VlanCollection::IsMember(const Vlan &vlan) const
{
    ConstIterator iter;

    for (iter = __vlans.begin(); iter != __vlans.end(); iter++) {
        if (*iter == vlan) {
            return true;
        }
    }

    return false;
}

unsigned int VlanCollection::GetCount() const
{
    return __vlans.size();
}

bool VlanCollection::operator==(const VlanCollection &right) const
{
    if (__vlans.size() != right.__vlans.size()) {
        return false;
    }

    for (unsigned int i = 0; i < __vlans.size(); i++) {
        if (__vlans[i] != right.__vlans[i]) {
            return false;
        }
    }

    return true;
}

bool VlanCollection::operator!=(const VlanCollection &right) const
{
    return !operator==(right);
}
