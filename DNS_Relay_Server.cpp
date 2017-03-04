//
// Created by thief on 17-3-4.
//
#include <iostream>
#include <boost/array.hpp>
#include "DNS_Relay_Server.h"

void DNS_Relay_Server::start()
{
    try
    {
        udp::socket socket(ioService, udp::endpoint(udp::v4(), serverPort));

        while(true)
        {
            // TODO
            boost::array<char,RECE_BUFFER_SIZE> recvBuf;

            udp::endpoint remoteEndpoint;
            boost::system::error_code error;
            std::cout<<"1\n";
            socket.receive_from(boost::asio::buffer(recvBuf),
                                remoteEndpoint, 0, error);
            std::cout<<"2\n";
            if (error && error != boost::asio::error::message_size)
                throw boost::system::system_error(error);

            for (int i = 0; i < 200; ++i) {
                std::cout<<recvBuf.at(i);
            }


        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void DNS_Relay_Server::stop()
{

}

void DNS_Relay_Server::resovlerDNSData(
        boost::array<char, RECE_BUFFER_SIZE> &receiveBuf,
        DNS_Data &dnsData)
{

}

void DNS_Relay_Server::responseClient()
{

}

void DNS_Relay_Server::requestOther()
{

}

std::string DNS_Relay_Server::requestSelf()
{
    return std::__cxx11::string();
}
