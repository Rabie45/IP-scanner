#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <netdb.h>
#include <netinet/in.h>
#include <ostream>
#include <sys/socket.h>
#include <unistd.h>

class ServerCreation {
  const char *ip;
  std::string addr_host;

public:

  sockaddr_in createServer(std::string addr_host) {
    struct hostent *hname;
    struct sockaddr_in addr;
    const char *cstr = addr_host.data();
    char *nonConstStr = const_cast<char *>(cstr);
    if ((hname = gethostbyname(cstr)) == NULL) {
      // std::cerr << "Failed to resolve hostname" << std::endl;
      return {};
    }

    addr.sin_family = hname->h_addrtype;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = *(long *)hname->h_addr;
    return addr;
  }
};