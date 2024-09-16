# Ip-scanner
- An IP scanner is a networking tool used to scan IP addresses within a network to discover and gather information about devices connected to that network.

## How to use
- Clone the repo ```git clone https://github.com/Rabie45/IP-scanner.git```
- Use g++ compiler to compile the code ```g++ -pthread main.cpp DNSHandler.cpp ICMPHandler.cpp IpScanner.cpp ServerCreation.cpp SocketCreation.cpp```
- Use this command to see the output ```./a.out```

## Explination
- Connection class responsiple for send packet, recive packet and check the reply from recived packet
- The recivePacket function is designed to receive data from a socket in a non-blocking manner, using the select system call to wait for data to become available.
- DNSHandler class responsiple for get the ip for avilable addresses and get the name of the host connected
- ICMPHanler handles the check sum fo ICMP
- ServerCreation and SocketCreation is responsiple for create the server and socket
- The program have two modes one for mutlithreading for max speed second single threaded for only one thread devices

## The output 
- SingleThreaded

![SingleThreaded](https://github.com/user-attachments/assets/f650b185-f897-46fd-891d-69be3b733b4a)

- MultiThreaded

![MultiThreaded](https://github.com/user-attachments/assets/1fbcc1fb-ac15-40be-b0f7-b36a892e43f7)
