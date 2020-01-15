#include "client.hpp"
#include <iostream>
#include <iomanip>

int main() {

  NTPClient client{"192.168.1.109", 123};
  std::cout << std::fixed << client.request_time() << std::endl;
}