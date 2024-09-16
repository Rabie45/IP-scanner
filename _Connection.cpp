#include <fcntl.h>
#include <iostream>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/types.h>
class _Connection {
  int sock;
  struct sockaddr_in addr;
  struct icmp icmp_hdr;

public:
  char buffer[1024];
  _Connection(int sock, struct sockaddr_in addr, struct icmp icmp_hdr)
      : sock(sock), addr(addr), icmp_hdr(icmp_hdr) {}

  ssize_t sendPacket() {
   

    auto val = sendto(sock, &icmp_hdr, sizeof(icmp_hdr), 0,
                      (struct sockaddr *)&addr, sizeof(addr));
    return val;
  }
  ssize_t recivePacket() {
    socklen_t len = sizeof(addr);

    // Set the socket to non-blocking mode
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    fd_set readfds;
    struct timeval timeout;

    // Initialize the file descriptor set
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    // Set timeout to 5 seconds
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;


    // Use select to wait for data to be available on the socket
    int activity = select(sock + 1, &readfds, NULL, NULL, &timeout);
   
    if (activity < 0 && errno != EINTR) {
      perror("select error");
      return -1;
    } else if (activity == 0) {

      return -1; // Timeout occurred
    }

    if (FD_ISSET(sock, &readfds)) {
      // std::cout << "Data available, receiving packet..." << std::endl;
      ssize_t ret = recvfrom(sock, buffer, sizeof(buffer), 0,
                             (struct sockaddr *)&addr, &len);

      if (ret < 0) {
        perror("recvfrom error");
      }

      return ret;
    }

    return -1;
  }
  bool checkReply() {
    struct iphdr *ip_hdr = (struct iphdr *)buffer;
    struct icmphdr *icmp_hdr_reply =
        (struct icmphdr *)(buffer + (ip_hdr->ihl * 4));

    if (icmp_hdr_reply->type == ICMP_ECHOREPLY) {
      return true;
    } else if (icmp_hdr_reply->type == ICMP_DEST_UNREACH) {
     
      return false;
    }

    return false;
  }
};