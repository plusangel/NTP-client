#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <string>
#ifdef _WIN32
#   include <WinSock2.h>
using Socket = SOCKET;
#else
#   include <netinet/in.h>
using Socket = int;
#endif

struct NTPPacket
{
    uint8_t li_vn_mode; // Eight bits. li, vn, and mode.
                        // li.   Two bits.   Leap indicator.
                        // vn.   Three bits. Version number of the protocol.
                        // mode. Three bits. Client will pick mode 3 for client.

    // Eight bits. Stratum level of the local clock.
    uint8_t stratum;

    // Eight bits. Maximum interval between successive messages.
    uint8_t poll;

    // Eight bits. Precision of the local clock.
    uint8_t precision;

    // 32 bits. Total round trip delay time.
    uint32_t rootDelay;

    // 32 bits. Max error aloud from primary clock source.
    uint32_t root_dispersion;

    // 32 bits. Reference clock identifier.
    uint32_t ref_id;

    // 32 bits. Reference time-stamp seconds.
    uint32_t ref_timestamp_sec;

    // 32 bits. Reference time-stamp fraction of a second.
    uint32_t ref_timestamp_sec_frac;

    // 32 bits. Originate time-stamp seconds.
    uint32_t orig_timestamp_sec;

    // 32 bits. Originate time-stamp fraction of a second.
    uint32_t orig_timestamp_sec_frac;

    // 32 bits. Received time-stamp seconds.
    uint32_t received_timestamp_sec;

    // 32 bits. Received time-stamp fraction of a second.
    uint32_t received_timestamp_sec_frac;

    // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t transmited_timestamp_sec;

    // 32 bits. Transmit time-stamp fraction of a second.
    uint32_t transmited_timestamp_sec_frac;
};

struct NTPClient
{
    NTPClient(std::string host, uint16_t port);
    ~NTPClient();

    /**
   * @brief Transmits an NTP request to the defined server and returns the
   * timestamp
   *
   * @return (uint64_t) the number of milliseconds since 1970. Return 0 if fail. 
   */
    uint64_t request_time();

private:
    /**
     * @brief Converts from hostname to ip address
     * 
     * @param hostname name of the host.
     * @return ip address. Return empty string if coun't find the ip.
     */
    std::string hostname_to_ip(const std::string &hostname);

    /// @brief Build the connection. Set all the params for the socket_client.
    void build_connection();

    /// @brief Close the connection. Set -1 to socket_fd. 
    void close_socket();

    /// @brief NTP server IP address
    std::string hostname_;

    /// @brief NTP server port
    size_t port_;

    /// @brief Socket file descriptor
    Socket socket_fd;

    /// @brief Server address data structure
    struct sockaddr_in socket_client;

    /// @brief Delta between epoch time and ntp time
    static constexpr unsigned long long NTP_TIMESTAMP_DELTA{2208988800ull};
};

#endif // NTPCLIENT_H