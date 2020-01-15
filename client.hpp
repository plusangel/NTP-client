#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct NTPPacket {

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

struct NTPClient {
  NTPClient(std::string host, size_t port);
  ~NTPClient();

  double request_time();

private:
  /// @brief NTP server IP address
  std::string host;

  /// @brief NTP server port
  size_t port;

  /// @brief Socket file descriptor
  int sockfd;

  /// @brief Server address data structure
  struct sockaddr_in servaddr;

  /// @brief Delta between epoch time and ntp time
  static constexpr long NTP_TIMESTAMP_DELTA{2208988800ull};
};

#endif // NTPCLIENT_H