#include <iostream>
#include <boost/asio.hpp>
#include <bitset>
#include "DNS_Relay_Server.h"
#include <sstream>
using boost::asio::ip::udp;

int main() {
    DNS_Relay_Server server(53);
    server.start();

//    std::bitset<16> mm;
//    std::cout<<mm.to_ulong()<<std::endl;
//    std::bitset<8> test1('c');
//    std::bitset<8> test2('d');
//    std::cout<<"  \n"<<test1<<" "<<test2<<"\n";
//    //std::stringstream bitHelper("10111");
//    std::stringstream bitHelper;
//    bitHelper<<test1<<test2;
//    bitHelper>>mm;
//
//    std::bitset<16> ag('cd');
//    //mm.set(4,0);
//    //bitHelper<<mm;
//
// //   std::stringstream bitHelper(hi);
//
//    std::cout<<mm<<"\n";
//
//    bitHelper<<ag;
//    bitHelper>>mm;
//    std::cout<<mm;
    return 0;
}