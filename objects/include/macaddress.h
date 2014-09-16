/*
 * macaddress.h
 *
 *  Created on: Sep 15, 2014
 *      Author: elestar
 */

#ifndef MACADDRESS_H_
#define MACADDRESS_H_


class MacAddress
{
public:
    static const unsigned int rawSize = 6;
    static const unsigned int stringSize = 18;

private:
    unsigned char __raw[rawSize];
    bool __isMulticast;
    bool __isBroadcast;

protected:
    void CheckMulticast();
    void CheckBroadcast();

public:
    MacAddress();
    MacAddress(const MacAddress &mac);
    MacAddress(const unsigned char *mac, unsigned int size);
    MacAddress(const char *mac, unsigned int size);
    ~MacAddress();

    void Clear();

    bool GetRaw(unsigned char *mac, unsigned int size) const;
    bool SetRaw(const unsigned char *mac, unsigned int size);

    bool Set(const MacAddress &mac);

    bool Parse(const char *mac, unsigned int size);
    bool ToString(char *mac, unsigned int size) const;

    bool IsMulticast() const;
    bool IsBroadcast() const;

    bool operator==(const MacAddress &right) const;
    bool operator!=(const MacAddress &right) const;
};


#endif /* MACADDRESS_H_ */
