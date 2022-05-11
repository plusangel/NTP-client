#include "client.hpp"

#include <iostream>
#include <iomanip>

#include "time.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " \"Address of NTP server pool\" \"port\" \n";

        std::cout << "Example: \n\t";
        std::cout << argv[0] << " 0.pool.ntp.org 123\n";

        return EXIT_FAILURE;
    }

    NTPClient client{argv[1], std::stoi(argv[2])};

    auto epoch_server_ms = client.request_time();
    
    if (0 == epoch_server_ms)
        return EXIT_FAILURE;

    // The function ctime receives the timestamps in seconds. 
    time_t epoch_server = (uint32_t)(epoch_server_ms / 1000);

    std::cout << "Server time: " << ctime(&epoch_server);
    std::cout << "Timestamp server: " << (uint32_t)epoch_server << "\n\n";

    time_t local_time;
    local_time = time(0);

    std::cout << "System time is " << (epoch_server - local_time) << " seconds off\n";

    return EXIT_SUCCESS;
}