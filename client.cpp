#include "client.hpp"

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

NTPClient::~NTPClient() {
  close(sockfd);
}

double NTPClient::request_time() {

  int n; // return result from writing/reading from the socket

  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    std::cerr << "ERROR connecting" << std::endl;

  NTPPacket packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  packet.li_vn_mode = 0x1b;

  n = write(sockfd, (char *)&packet, sizeof(NTPPacket));

  if (n < 0)
    std::cerr << "ERROR writing to socket" << std::endl;

  n = read(sockfd, (char *)&packet, sizeof(NTPPacket));

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

  // seconds since UNIX epoch
  uint32_t txTm = packet.txTm_s - NTP_TIMESTAMP_DELTA;
  // convert seconds to milliseconds
  double milliseconds = (double)txTm * 1000.0;
  // add fractional part (one second == 2^32-1 == 4294967295)
  milliseconds = milliseconds + ((double)packet.txTm_f / 4294967295.0) * 1000.0;
  

  // Print the time we got from the server, accounting for local timezone and
  // conversion from UTC time.

  //time_t txTm = (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);
  //std::cout << "Time: " << ctime((const time_t *)&txTm);

  return milliseconds;
}