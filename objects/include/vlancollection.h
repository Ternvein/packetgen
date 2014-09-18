/*
 * vlancollection.h
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#ifndef VLANCOLLECTION_H_
#define VLANCOLLECTION_H_

#include "vlan.h"

#include <vector>


class VlanCollection
{
    std::vector<Vlan> __vlans;

public:
    typedef std::vector<Vlan>::iterator Iterator;
    typedef std::vector<Vlan>::const_iterator ConstIterator;

    VlanCollection();
    VlanCollection(const VlanCollection &vlans);
    ~VlanCollection();

    void Clear();

    ConstIterator Begin() const;
    ConstIterator End() const;

    void Set(const VlanCollection &vlans);

    bool Add(const Vlan &vlan);
    bool Remove(const Vlan &vlan);
    bool IsMember(const Vlan &vlan) const;

    unsigned int GetCount() const;

    bool operator==(const VlanCollection &right) const;
    bool operator!=(const VlanCollection &right) const;
};


#endif /* VLANCOLLECTION_H_ */
