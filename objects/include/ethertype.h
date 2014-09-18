/*
 * ethertype.h
 *
 *  Created on: Sep 18, 2014
 *      Author: elestar
 */

#ifndef ETHERTYPE_H_
#define ETHERTYPE_H_

#include "object.h"


class Ethertype : public Object
{
public:
    static const unsigned int rawSize = 2;
    static const unsigned int stringMinSize = 5;

private:
    unsigned short __raw;

public:
    Ethertype();
    Ethertype(const Ethertype &ethertype);
    Ethertype(unsigned int ethertype);
    ~Ethertype();

    void Clear();

    void Set(const Ethertype &ethertype);

    unsigned int Get() const;
    bool Set(unsigned int ethertype);

    bool GetRaw(unsigned char *ethertype, unsigned int size) const;
    bool SetRaw(const unsigned char *ethertype, unsigned int size);

    unsigned int GetStringSize() const;
    bool Parse(const char *buffer, unsigned int size);
    bool ToString(char *buffer, unsigned int size) const;

    bool operator==(const Ethertype &right) const;
    bool operator!=(const Ethertype &right) const;
};


#endif /* ETHERTYPE_H_ */
