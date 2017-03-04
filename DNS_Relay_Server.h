//
// Created by thief on 17-3-4.
//

#ifndef DNS_RELAY_SERVER_DNS_RELAY_SERVER_H
#define DNS_RELAY_SERVER_DNS_RELAY_SERVER_H

#include <string>
#include <boost/asio.hpp>

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
    void resovlerDNSData(boost::array<char,RECE_BUFFER_SIZE> &receiveBuf,DNS_Data& dnsData);
    void responseClient();
    void requestOther();
    std::string requestSelf();


private:
    int serverPort;
    boost::asio::io_service ioService;
};



class DNS_Data
{

};



#endif //DNS_RELAY_SERVER_DNS_RELAY_SERVER_H
