#!/usr/bin/sh

mkdir ./lib > /dev/null
rm ./lib/*.a ./lib/*.o ./lib/*.so > /dev/null
rm ./test > /dev/null

echo "Compiling libs..."

#Objects
g++ -Wall -c -I./objects/include -o ./lib/macaddress.o ./objects/macaddress.cpp
g++ -Wall -c -I./objects/include -o ./lib/ipaddress.o ./objects/ipaddress.cpp
g++ -Wall -D_REENTERANT -c -I./objects/include -o ./lib/vlan.o ./objects/vlan.cpp

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
