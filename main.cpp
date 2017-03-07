#include <iostream>
#include <boost/asio.hpp>
#include <bitset>
#include "DNS_Relay_Server.h"
#include <sstream>
using boost::asio::ip::udp;

int main() {
    DNS_Relay_Server server(53);
    server.start();

    int h;

    std::bitset<16> mm1(34360);
    std::bitset<16> mm2("111011");
    std::stringstream bitH;
    bitH<<(char)(mm2.to_ulong());
    std::cout<<bitH.str();


    std::string str("1111111000000011001101");

    std::bitset<32> bitvec5(str, 5, 4); // 4 bits starting at str[5], 1100

    std::bitset<32> bitvec6(str, str.size() - 4);     // use last 4 characters
    std::cout<<"\n "<<bitvec5<<"  "<<bitvec6<<"  ";
    //std::cout<<mm1.to_string()<<std::endl;
//    std::cout<<mm2.to_string()<<std::endl;
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