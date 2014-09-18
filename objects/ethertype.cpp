/*
 * ethertype.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#include "ethertype.h"


Ethertype::Ethertype()
{
    Clear();
}

Ethertype::Ethertype(const Ethertype &ethertype)
{
    Set(ethertype);
}

Ethertype::Ethertype(unsigned int ethertype)
{
    Set(ethertype);
}

Ethertype::~Ethertype()
{
    Clear();
}

void Ethertype::Clear()
{
    __raw = 0;
}

void Ethertype::Set(const Ethertype &ethertype)
{
    Clear();

    __raw = ethertype.__raw;
}

unsigned int Ethertype::Get() const
{
    return __raw;
}

bool Ethertype::Set(unsigned int ethertype)
{
    Clear();

    __raw = ethertype;

    return true;
}

bool Ethertype::GetRaw(unsigned char *ethertype, unsigned int size) const
{
    if (ethertype == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL Ethertype detected" << std::endl;
        return false;
    }

    if (size < rawSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Ethertype buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    return ConvertToRaw(ethertype, __raw);
}

bool Ethertype::SetRaw(const unsigned char *ethertype, unsigned int size)
{
    if (ethertype == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL Ethertype detected" << std::endl;
        return false;
    }

    if (size < rawSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Ethertype buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    __raw = *reinterpret_cast<const unsigned short *>(ethertype);

    return true;
}

unsigned int Ethertype::GetStringSize() const
{
    return stringMinSize;
}

bool Ethertype::ToString(char *buffer, unsigned int size) const
{
    if (buffer == NULL) {
        std::cerr << __PRETTY_FUNCTION__ << ": NULL buffer detected" << std::endl;
        return false;
    }

    if (size < stringMinSize) {
        std::cerr << __PRETTY_FUNCTION__ << ": Ethertype buffer size " << size
                  << " is too short" << std::endl;
        return false;
    }

    snprintf(buffer, size, "%04x", __raw);

    return true;
}

bool Ethertype::operator==(const Ethertype &right) const
{
    if (__raw != right.__raw) {
        return false;
    }

    return true;
}

bool Ethertype::operator!=(const Ethertype &right) const
{
    return !operator==(right);
}
