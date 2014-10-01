/*
 * pdu_icmp.h
 *
 *  Created on: Sep 24, 2014
 *      Author: elestar
 */

#ifndef PDU_ICMP_H_
#define PDU_ICMP_H_

#include "header_ethernet.h"
#include "header_ip.h"


namespace Pdu {

class Icmp
{
public:
    typedef enum {
        TYPE_ECHOREPLY = 0,
        TYPE_DESTUNREACHABLE = 3,
        TYPE_SOURCEQUENCH = 4,
        TYPE_REDIRECTMESSAGE = 5,
        TYPE_ALTERSOURCEADDRESS = 6,
        TYPE_ECHOREQUEST = 8,
        TYPE_ROUTERADVERT = 9,
        TYPE_ROUTERSOLICE = 10,
        TYPE_TIMEEXCEED = 11,
        TYPE_BADIPHEADER = 12,
        TYPE_TIMESTAMP = 13,
        TYPE_TIMESTAMPREPLY = 14,
        TYPE_INFOREQUEST = 15,
        TYPE_INFOREPLY = 16,
        TYPE_ADDRMASKREQUEST = 17,
        TYPE_ADDRMASKREPLY = 18,
        TYPE_TRACEROUTE = 30
    } Type;

    typedef enum {
        CODE_NETWORKUNREACHABLE = 0,
        CODE_HOSTUNREACHABLE = 1,
        CODE_PROTOCOLUNREACHABLE = 2,
        CODE_PORTUNREACHABLE = 3,
        CODE_FRAGREQUIRED = 4,
        CODE_ROUTEFAILED = 5,
        CODE_NETWORKUNKNOWN = 6,
        CODE_HOSTUNKNOWN = 7,
        CODE_HOSTISOLATED = 8,
        CODE_NETWORKPROHIBITED = 9,
        CODE_HOSTPROHIBITED = 10
    } Code;

    static const unsigned int rawMinSize = 8;
    static const unsigned int stringMinSize = 17;

private:
    static const unsigned int typeOffset = 0;
    static const unsigned int codeOffset = 1;
    static const unsigned int checksumOffset = 2;
    static const unsigned int restOffset = 4;
    static const unsigned int dataOffset = 8;

    Header::Ethernet __ethernet;
    Header::Ip __ip;

    unsigned char __type;
    unsigned char __code;

    unsigned short __checksum;
    unsigned int __rest;

    unsigned char *__data;

protected:
    static unsigned short CalculateChecksum(const unsigned char *buffer, unsigned int size);

public:
    Icmp();
    Icmp(const Icmp &icmp);
    ~Icmp();

    void Clear();

    void Set(const Icmp &icmp);

    bool GetRaw(unsigned char *buffer, unsigned int size) const;
    bool SetRaw(const unsigned char *buffer, unsigned int size);

    bool SetType(Type value);
    Type GetType() const;

    bool SetCode(Code value);
    Code GetCode() const;

    bool SetChecksum(unsigned short checksum);
    unsigned short GetChecksum() const;
    unsigned short CalculateChecksum() const;
    bool IsChecksumValid() const;

    bool SetRest(unsigned int rest);
    unsigned int GetRest() const;

    bool operator==(const Icmp &right) const;
    bool operator!=(const Icmp &right) const;
};

} /* end of namespace Pdu */


#endif /* PDU_ICMP_H_ */
