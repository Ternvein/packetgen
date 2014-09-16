/*
 * test.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: elestar
 */

#include "pdu_arp.h"
#include <stdio.h>
#include <string.h>


int main()
{
    Pdu::Arp arp;

    unsigned char pdu[50];
    unsigned int offset = 0;

    memset(pdu, 0, sizeof(pdu));

    arp.GetRaw(pdu, sizeof(pdu), &offset);

    for (unsigned int i = 0; i < offset; i++) {
        printf("%x ", pdu[i]);
    }
    printf("\r\n");

    return 0;
}
