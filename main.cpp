#include <iostream>
#include <boost/asio.hpp>
#include "DNS_Relay_Server.h"
using boost::asio::ip::udp;

int main() {
    DNS_Relay_Server server(53);
    server.start();
    return 0;
}