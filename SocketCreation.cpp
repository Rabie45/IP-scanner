#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
class SocketCreation {
public:
  int createSocket() { return socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP); }
};