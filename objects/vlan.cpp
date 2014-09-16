/*
 * vlan.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#include "vlan.h"
#include <stdio.h>
#include <string.h>


Vlan::Vlan()
{
    Clear();
}

Vlan::Vlan(const Vlan &vlan)
{
	Set(vlan);
}

Vlan::~Vlan()
{
    Clear();
}

void Vlan::Clear()
{
    SetTpid(tpidCtag);
    SetCos(defaultCos);
    DisableDei();
    SetVid(defaultVid);
}

bool Vlan::Set(const Vlan &vlan)
{
    Clear();

    __raw = vlan.__raw;

    return true;
}

bool Vlan::GetRaw(unsigned char *vlan, unsigned int size) const
{
    if (vlan == NULL) {
        return false;
    }

    if (size < rawSize) {
        return false;
    }

    memcpy(vlan, &__raw, rawSize);

    return true;
}

bool Vlan::SetRaw(const unsigned char *vlan, unsigned int size)
{
	Clear();

    if (vlan == NULL) {
        return false;
    }

    if (size < rawSize) {
        return false;
    }

    unsigned int vlanData = *reinterpret_cast<const unsigned int *>(vlan);

    Tpid tpid = (vlanData & tpidMask) >> offsetTpid;
    if (!IsTpidValid(tpid)) {
        return false;
    }

    Cos cos = (vlanData & cosMask) >> offsetCos;
    if (!IsCosValid(cos)) {
        return false;
    }

    Vid vid = vlanData & vidMask;
    if (!IsVidValid(vid)) {
        return false;
    }

    __raw = vlanData;

    return true;
}

Vlan::Tpid Vlan::GetTpid() const
{
    Tpid tpid = (__raw & tpidMask) >> offsetTpid;

    return tpid;
}

bool Vlan::SetTpid(const Tpid &tpid)
{
    if (!IsTpidValid(tpid)) {
        return false;
    }

    __raw &= ~tpidMask;
    __raw |= (tpid << offsetTpid) & tpidMask;

    return true;
}

bool Vlan::IsTpidValid(const Tpid &tpid)
{
    if (tpid != tpidCtag && tpid != tpidStag) {
        return false;
    }

    return true;
}

bool Vlan::IsCtag() const
{
    Tpid tpid = (__raw & tpidMask) >> offsetTpid;

    if (tpid == tpidCtag) {
        return true;
    }

    return false;
}

void Vlan::SetCtag()
{
    __raw &= ~tpidMask;
    __raw |= tpidCtag << offsetTpid;
}

bool Vlan::IsStag() const
{
    Tpid tpid = (__raw & tpidMask) >> offsetTpid;

    if (tpid == tpidStag) {
        return true;
    }

    return false;
}

void Vlan::SetStag()
{
    __raw &= ~tpidMask;
    __raw |= tpidStag << offsetTpid;
}

Vlan::Tci Vlan::GetTci() const
{
    Tci tci = __raw & tciMask;

    return tci;
}

bool Vlan::SetTci(const Tci &tci)
{
    Cos cos = (tci & cosMask) >> offsetCos;
    if (!IsCosValid(cos)) {
        return false;
    }

    __raw &= ~tciMask;
    __raw |= (tci & tciMask);

    return true;
}

Vlan::Cos Vlan::GetCos() const
{
    Cos cos = (__raw & cosMask) >> offsetCos;

    return cos;
}

bool Vlan::SetCos(const Cos &cos)
{
    if (!IsCosValid(cos)) {
        return false;
    }

    __raw &= ~cosMask;
    __raw |= (cos & cosMask);

    return true;
}

bool Vlan::IsCosValid(const Cos &cos)
{
    if (cos > maxCos) {
        return false;
    }

    return true;
}

bool Vlan::IsDei() const
{
    if (__raw & deiMask) {
        return true;
    }

    return false;
}

void Vlan::EnableDei()
{
    __raw |= deiMask;
}

void Vlan::DisableDei()
{
    __raw &= ~deiMask;
}

Vlan::Vid Vlan::GetVid() const
{
    Vid vid = (__raw & vidMask);

    return vid;
}

bool Vlan::SetVid(const Vid &vid)
{
    if (!IsVidValid(vid)) {
        return false;
    }

    __raw &= ~vidMask;
    __raw |= (vid & vidMask);

    return true;
}

bool Vlan::IsVidValid(const Vid &vid)
{
    if (vid < minVid || vid > maxVid) {
        return false;
    }

    return true;
}

bool Vlan::operator==(const Vlan &right) const
{
    if (__raw == right.__raw) {
        return true;
    }

    return false;
}

bool Vlan::operator!=(const Vlan &right) const
{
    return !operator==(right);
}
