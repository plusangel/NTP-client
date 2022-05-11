#include "client.hpp"
#include <iostream>
#include <iomanip>

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
    std::cout << std::fixed << client.request_time() << std::endl;

    return EXIT_SUCCESS;
}