/*
 * test.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#include "pdu_arp.h"

#include <stdio.h>
#include <string.h>
#include <iostream>


int main()
{
    Pdu::Arp arp;

    unsigned char pdu[50];
    unsigned int offset = 0;

    memset(pdu, 0, sizeof(pdu));

    bool rc = arp.GetRaw(pdu, sizeof(pdu), &offset);
    if (!rc) {
        std::cerr << "Main: Can't get raw ARP" << std::endl;
    }

    std::cout << "PDU size: " << offset << std::endl;
    for (unsigned int i = 0; i < offset; i++) {
        printf("%02x ", pdu[i]);
    }
    printf("\r\n");

    return 0;
}
