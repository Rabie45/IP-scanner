#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <string.h>

class DNSHandler {
  std::string addr_host;
  sockaddr_in *addr_con;

public:
  DNSHandler(std::string addr_host, struct sockaddr_in *addr_con)
      : addr_host(addr_host), addr_con(addr_con) {}

  char *dns_lookup() {
    // printf("\nResolving DNS...\n");
    const char *cstr = addr_host.data();
    char *nonConstStr = const_cast<char *>(cstr);
    struct hostent *host_entity;
    char *ip = (char *)malloc(NI_MAXHOST * sizeof(char));

    if ((host_entity = gethostbyname(cstr)) == NULL) {
      // No IP found for hostname
      return NULL;
    }

    // Fill up address structure
    strcpy(ip, inet_ntoa(*(struct in_addr *)host_entity->h_addr));
    (*addr_con).sin_family = host_entity->h_addrtype;
    (*addr_con).sin_port = htons(0);
    (*addr_con).sin_addr.s_addr = *(long *)host_entity->h_addr;

    return ip;
  }

  // Resolve the reverse lookup of the hostname
  char *reverse_dns_lookup(char *ip_addr) {
    struct sockaddr_in temp_addr;
    socklen_t len;
    char buf[NI_MAXHOST], *ret_buf;

    temp_addr.sin_family = AF_INET;
    temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
    len = sizeof(struct sockaddr_in);

    if (getnameinfo((struct sockaddr *)&temp_addr, len, buf, sizeof(buf), NULL,
                    0, NI_NAMEREQD)) {
      // printf("Could not resolve reverse lookup of hostname\n");
      return NULL;
    }

    ret_buf = (char *)malloc((strlen(buf) + 1) * sizeof(char));
    strcpy(ret_buf, buf);
    return ret_buf;
  }
};