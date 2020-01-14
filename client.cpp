#include "client.hpp"

struct ntp_packet {

  uint8_t li_vn_mode; // Eight bits. li, vn, and mode.
                      // li.   Two bits.   Leap indicator.
                      // vn.   Three bits. Version number of the protocol.
                      // mode. Three bits. Client will pick mode 3 for client.

  uint8_t stratum; // Eight bits. Stratum level of the local clock.
  uint8_t poll;    // Eight bits. Maximum interval between successive messages.
  uint8_t precision; // Eight bits. Precision of the local clock.

  uint32_t rootDelay; // 32 bits. Total round trip delay time.
  uint32_t
      rootDispersion; // 32 bits. Max error aloud from primary clock source.
  uint32_t refId;     // 32 bits. Reference clock identifier.

  uint32_t refTm_s; // 32 bits. Reference time-stamp seconds.
  uint32_t refTm_f; // 32 bits. Reference time-stamp fraction of a second.

  uint32_t origTm_s; // 32 bits. Originate time-stamp seconds.
  uint32_t origTm_f; // 32 bits. Originate time-stamp fraction of a second.

  uint32_t rxTm_s; // 32 bits. Received time-stamp seconds.
  uint32_t rxTm_f; // 32 bits. Received time-stamp fraction of a second.

  uint32_t txTm_s; // 32 bits and the most important field the client cares
                   // about. Transmit time-stamp seconds.
  uint32_t txTm_f; // 32 bits. Transmit time-stamp fraction of a second.
};

NTPClient::NTPClient(std::string host, size_t port) {

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    std::cerr << "socket creation failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  servaddr.sin_addr.s_addr = inet_addr(host.c_str());
}

long NTPClient::request_time() {

  int n; // return result from writing/reading from the socket

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    std::cerr << "ERROR connecting" << std::endl;

  ntp_packet packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  packet.li_vn_mode = 0x1b;

  n = write(sockfd, (char *)&packet, sizeof(ntp_packet));

  if (n < 0)
    std::cerr << "ERROR writing to socket" << std::endl;

  n = read(sockfd, (char *)&packet, sizeof(ntp_packet));

  if (n < 0)
    std::cerr << "ERROR reading from socket" << std::endl;

  // These two fields contain the time-stamp seconds as the packet left the NTP
  // server. The number of seconds correspond to the seconds passed since 1900.
  // ntohl() converts the bit/byte order from the network's to host's
  // "endianness".

  packet.txTm_s = ntohl(packet.txTm_s); // Time-stamp seconds.
  packet.txTm_f = ntohl(packet.txTm_f); // Time-stamp fraction of a second.

  // Extract the 32 bits that represent the time-stamp seconds (since NTP epoch)
  // from when the packet left the server. Subtract 70 years worth of seconds
  // from the seconds since 1900. This leaves the seconds since the UNIX epoch
  // of 1970.
  // (1900)------------------(1970)**************************************(Time
  // Packet Left the Server)

  time_t txTm = (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);

  // Print the time we got from the server, accounting for local timezone and
  // conversion from UTC time.

  std::cout << "Time: " << ctime((const time_t *)&txTm);

  return (long)txTm;
}

int main() {
  NTPClient client{"192.168.1.109", 123};
  std::cout << client.request_time() << std::endl;
}