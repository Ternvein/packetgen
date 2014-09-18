#!/usr/bin/sh

mkdir ./lib > /dev/null 2>&1
rm ./lib/*.a ./lib/*.o ./lib/*.so > /dev/null 2>&1
rm ./test > /dev/null 2>&1

echo "Compiling libs..."

#Objects
g++ -Wall -c -I./objects/include -o ./lib/object.o ./objects/object.cpp
g++ -Wall -c -I./objects/include -o ./lib/ethertype.o ./objects/ethertype.cpp
g++ -Wall -c -I./objects/include -o ./lib/macaddress.o ./objects/macaddress.cpp
g++ -Wall -c -I./objects/include -o ./lib/ipaddress.o ./objects/ipaddress.cpp
g++ -Wall -c -I./objects/include -o ./lib/vlan.o ./objects/vlan.cpp
g++ -Wall -c -I./objects/include -o ./lib/vlancollection.o ./objects/vlancollection.cpp

#g++ -Wall -c -I./objects/include ./objects/*.c*

#Headers
g++ -Wall -c -I./objects/include -I./headers/include -o ./lib/header_ethernet.o ./headers/header_ethernet.cpp

#Pdu
g++ -Wall -c -I./objects/include -I./headers/include -I./pdu/include -o ./lib/pdu_arp.o ./pdu/pdu_arp.cpp

echo "Archiving..."

ar rcs ./lib/libpacketgen.a ./lib/*.o

echo "Compiling test..."

g++ -static -fPIE -Wall -I./objects/include -I./headers/include -I./pdu/include -o test test.cpp -L./lib -lpacketgen
#g++ -I./objects/include -I./headers/include -I./pdu/include test.cpp ./lib/macaddress.o ./lib/ipaddress.o ./lib/vlan.o ./lib/header_ethernet.o ./lib/pdu_arp.o

echo "Done."
