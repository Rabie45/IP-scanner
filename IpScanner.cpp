#include "DNSHandler.cpp"
#include "ICMPHandler.cpp"
#include "ServerCreation.cpp"
#include "SocketCreation.cpp"
#include "_Connection.cpp"
#include <list>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <thread>
#include <vector>

class IpScanner {
public:
  std::list<std::string> ls, name;
  int range;
  std::mutex mtx;
  IpScanner(int range) : range(range) {}
  int scan(int start, int end) {
    struct hostent *hname;
    struct sockaddr_in addr;
    int value;
    int sock;
    std::string hostString;
    std::string va;
    char *hostname;
    char *revHosetName;
    ServerCreation serverCreation;
    SocketCreation socket;
    struct icmp icmp_hdr;
    ICMPHandler icmp_handler;
    socklen_t len;
    ssize_t ret;
    auto val = (ssize_t)0;
    for (int i = start; i < end; i++) {
      system("clear");
      // system("clear");
      std::cout << "Device number: " << i << "  out of "<< end << std::endl;

      hostString = "192.168.1.";
      value = i;
      va = std::to_string(value);
      hostString.insert(10, va);

      addr = serverCreation.createServer(hostString);

      sock = socket.createSocket();

      if (sock < 0) {
       
        perror("socket");
        return -1;
      }

      DNSHandler handler(hostString, &addr);
      hostname = handler.dns_lookup();
      revHosetName = handler.reverse_dns_lookup(hostname);

      if (revHosetName == NULL) {
        continue;
      }

      icmp_hdr = icmp_handler.ICMPInit();
      _Connection connection(sock, addr, icmp_hdr);
      val = connection.sendPacket();

      if (!val) {
        perror("sendto");
        close(sock);
        continue;
      }

     
      len = sizeof(addr);
      ret = connection.recivePacket();
      if (ret <= 0) {
        close(sock);
        continue;
      }

      if (connection.checkReply()) {
        std::lock_guard<std::mutex> lock(mtx);
        ls.push_back(hostname);
        name.push_back(revHosetName);
      }

      close(sock);
    }
    return 0;
  }
  int scanning() {
    int numOfThreads = std::thread::hardware_concurrency();
    if (numOfThreads == 0) {
      numOfThreads = 1; // Fallback in case hardware_concurrency returns 0
    }
    std::vector<std::thread> threadPool;
    int splitRange = range / numOfThreads;
    for (int i = 0; i < numOfThreads; i++) {
      int start = i * splitRange + 1;
      int end = (i == numOfThreads - 1) ? range : (i + 1) * splitRange;
      threadPool.emplace_back([this, start, end]() { this->scan(start, end); });
    }
    for (auto &thread : threadPool) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }
};
