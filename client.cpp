#include "client.hpp"

#include <iostream>
#include <cstring>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

NTPClient::NTPClient(std::string hostname, size_t port) : hostname_(hostname), port_(port)
{
}

void NTPClient::build_connection()
{
    // Creating socket file descriptor
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "socket creation failed" << std::endl;
    }

    memset(&socket_client, 0, sizeof(socket_client));

    std::string ntp_server_ip = hostname_to_ip(hostname_);

    std::cout << "Sending request to: " << ntp_server_ip << "\n";

    // Filling server information
    socket_client.sin_family = AF_INET;
    socket_client.sin_port = htons(port_);
    socket_client.sin_addr.s_addr = inet_addr(ntp_server_ip.c_str());
}

NTPClient::~NTPClient() { close(socket_fd); }

uint64_t NTPClient::request_time()
{
    int response; // return result from writing/reading from the socket

    build_connection();

    if (connect(socket_fd, (struct sockaddr *)&socket_client, sizeof(socket_client)) < 0)
        std::cerr << "ERROR connecting" << std::endl;

    NTPPacket packet = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    packet.li_vn_mode = 0x1b;

    response = write(socket_fd, (char *)&packet, sizeof(NTPPacket));

    if (response < 0)
        std::cerr << "ERROR writing to socket" << std::endl;

    response = read(socket_fd, (char *)&packet, sizeof(NTPPacket));

    if (response < 0)
        std::cerr << "ERROR reading from socket" << std::endl;

    // These two fields contain the time-stamp seconds as the packet left the NTP
    // server. The number of seconds correspond to the seconds passed since 1900.
    // ntohl() converts the bit/byte order from the network's to host's
    // "endianness".

    packet.transmited_timestamp_sec = ntohl(packet.transmited_timestamp_sec);           // Time-stamp seconds.
    packet.transmited_timestamp_sec_frac = ntohl(packet.transmited_timestamp_sec_frac); // Time-stamp fraction of a second.

    // Extract the 32 bits that represent the time-stamp seconds (since NTP epoch)
    // from when the packet left the server. Subtract 70 years worth of seconds
    // from the seconds since 1900. This leaves the seconds since the UNIX epoch
    // of 1970.
    // (1900)---------(1970)**********(Time Packet Left the Server)

    // seconds since UNIX epoch
    uint32_t txTm = packet.transmited_timestamp_sec - NTP_TIMESTAMP_DELTA;
    // convert seconds to milliseconds
    double milliseconds = (double)txTm * 1000l;
    // add fractional part
    milliseconds = milliseconds + ((double)packet.transmited_timestamp_sec_frac / ONE_SECOND) * 1000.0l;

    return milliseconds;
}

std::string NTPClient::hostname_to_ip(const std::string &host)
{
    hostent *hostname = gethostbyname(host.c_str());
    if (hostname)
        return std::string(inet_ntoa(**(in_addr **)hostname->h_addr_list));
    return {};
}