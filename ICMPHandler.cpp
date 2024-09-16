#include <cstring>
#include <netinet/ip_icmp.h>
#include <unistd.h>
class ICMPHandler {
  struct icmp icmp_hdr;

public:
  ICMPHandler() {}
  icmp ICMPInit() {
    memset(&icmp_hdr, 0, sizeof(icmp_hdr));
    icmp_hdr.icmp_type = ICMP_ECHO;
    icmp_hdr.icmp_cksum = 0;
    icmp_hdr.icmp_id = getpid();
    icmp_hdr.icmp_seq = 0;
    icmp_hdr.icmp_cksum = ICMPCheckSum(&icmp_hdr, sizeof(icmp_hdr));
    return icmp_hdr;
  }
  unsigned short ICMPCheckSum(void *b, int len) {

    unsigned short *buf = (unsigned short *)b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
      sum += *buf++;
    if (len == 1)
      sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
  }
};