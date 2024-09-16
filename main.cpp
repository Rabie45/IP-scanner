
#include "IpScanner.cpp"
#include <arpa/inet.h>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>

/*
 *
 * This main For using single thread or multi-threading
 */

int main() {
  // const char *host;
  IpScanner scanner(255);
   //comment this line in case using multithreading
  scanner.scan(0, 255);

  // Uncomment this line for multi thread
  // scanner.scanning();

  
   int s = 0;
  std::cout << "..................Devices connedted.........." << std::endl;
  while (scanner.ls.size() > 0) {
    s++;
    std::cout << scanner.ls.front() << "  " << scanner.name.front()
              << std::endl;
    scanner.ls.pop_front();
    scanner.name.pop_front();
  }

  return 0;
}

