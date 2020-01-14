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

struct NTPClient {
  NTPClient(std::string host, size_t port);
  long request_time();

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