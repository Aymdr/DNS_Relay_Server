//
// Created by thief on 17-3-4.
//

#ifndef DNS_RELAY_SERVER_DNS_RELAY_SERVER_H
#define DNS_RELAY_SERVER_DNS_RELAY_SERVER_H

#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <bitset>

#define  RECE_BUFFER_SIZE 1024
using boost::asio::ip::udp;


class DNS_Data;

class DNS_Relay_Server {
public:
    DNS_Relay_Server(int port)
    :serverPort(port)
    {

    };
    void start();
    void stop();
    void resovleDNSData(boost::array<char,RECE_BUFFER_SIZE> &receiveBuf,DNS_Data& dnsData);
    void responseClient();
    void requestOther();
    std::string requestSelf();


private:
    int serverPort;
    boost::asio::io_service ioService;
};



class DNS_Data
{
public:
    DNS_Data();
    DNS_Data(boost::array<char,RECE_BUFFER_SIZE> &dataBuf);
    void resolveData(boost::array<char,RECE_BUFFER_SIZE> &dataBuf);

    std::string getQName();


    void set16Bits(char one,char two,std::bitset<16>& bitsetDes);
    void debugPrintDNSData();
private:
    void resolveQuestionSec(boost::array<char,RECE_BUFFER_SIZE> &dataBuf,int currentFlag);




    std::bitset<16> ID;
    std::bitset<1> QR;
    std::bitset<4> OPCODE;
    std::bitset<1> AA;
    std::bitset<1> TC;
    std::bitset<1> RD;
    std::bitset<1> RA;
    std::bitset<3> Z;
    std::bitset<4> RCODE;
    std::bitset<16> QDCOUNT;
    std::bitset<16> ANCOUNT;
    std::bitset<16> NSCOUNT;
    std::bitset<16> ARCOUNT;

    std::string qName;

};



#endif //DNS_RELAY_SERVER_DNS_RELAY_SERVER_H
