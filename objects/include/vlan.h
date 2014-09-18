/*
 * vlan.h
 *
 *  Created on: Sep 15, 2014
 *      Author: elestar
 */

#ifndef VLAN_H_
#define VLAN_H_

#include "object.h"


class Vlan : public Object
{
public:
    typedef unsigned short Tpid;
    typedef unsigned short Tci;

    typedef unsigned char Cos;
    typedef unsigned short Vid;

    static const unsigned int rawSize = 4;
    static const unsigned int stringMinSize = 5;

    static const Tpid tpidCtag = 0x8100;
    static const Tpid tpidStag = 0x88A8;

private:
    static const unsigned int tpidMask = 0xFFFF0000;
    static const unsigned int tciMask = 0x0000FFFF;

    static const unsigned int cosMask = 0x0000E000;
    static const unsigned int deiMask = 0x00001000;
    static const unsigned int vidMask = 0x00000FFF;

    static const unsigned int offsetTpid = 16;
    static const unsigned int offsetCos = 13;

    static const Cos maxCos = 7;
    static const Vid minVid = 1;
    static const Vid maxVid = 0x0FFF;

    static const Cos defaultCos = 0;
    static const Vid defaultVid = 1;

    unsigned int __raw;

protected:

public:
    Vlan();
    Vlan(const Vlan &vlan);
    ~Vlan();

    void Clear();

    bool Set(const Vlan &vlan);

    bool GetRaw(unsigned char *vlan, unsigned int size) const;
    bool SetRaw(const unsigned char *vlan, unsigned int size);

    Tpid GetTpid() const;
    bool SetTpid(const Tpid &tpid);
    static bool IsTpidValid(const Tpid &tpid);

    bool IsCtag() const;
    void SetCtag();
    bool IsStag() const;
    void SetStag();

    Tci GetTci() const;
    bool SetTci(const Tci &tci);

    Cos GetCos() const;
    bool SetCos(const Cos &cos);
    static bool IsCosValid(const Cos &cos);

    bool IsDei() const;
    void EnableDei();
    void DisableDei();

    Vid GetVid() const;
    bool SetVid(const Vid &vid);
    static bool IsVidValid(const Vid &vid);

    unsigned int GetStringSize() const;
    bool Parse(const char *buffer, unsigned int size);
    bool ToString(char *buffer, unsigned int size) const;

    bool operator==(const Vlan &right) const;
    bool operator!=(const Vlan &right) const;
    Vlan operator++();
    Vlan operator++(int);
};


#endif /* VLAN_H_ */
