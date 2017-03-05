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

            DNS_Data dnsData;
            resovleDNSData(recvBuf,dnsData);
            dnsData.debugPrintDNSData();
            std::cout<<"2\n";
            if (error && error != boost::asio::error::message_size)
                throw boost::system::system_error(error);
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

void DNS_Relay_Server::resovleDNSData(
        boost::array<char, RECE_BUFFER_SIZE> &receiveBuf,
        DNS_Data &dnsData)
{
    dnsData.resolveData(receiveBuf);
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



DNS_Data::DNS_Data(boost::array<char, RECE_BUFFER_SIZE> &dataBuf) {
    this->resolveData(dataBuf);
}

void DNS_Data::resolveData(boost::array<char, RECE_BUFFER_SIZE> &dataBuf)
{
    set16Bits(dataBuf[0],dataBuf[1],ID);

    std::stringstream bitHelper;
    std::bitset<8> flagBits1(dataBuf[2]);
    std::bitset<8> flagBits2(dataBuf[3]);
    bitHelper<<flagBits1<<flagBits2;
    bitHelper>>QR>>OPCODE>>AA>>TC>>RD>>RA>>Z>>RCODE;

    set16Bits(dataBuf[4],dataBuf[5],QDCOUNT);
    set16Bits(dataBuf[6],dataBuf[7],ANCOUNT);
    set16Bits(dataBuf[8],dataBuf[9],NSCOUNT);
    set16Bits(dataBuf[10],dataBuf[11],ARCOUNT);

    int currentFlag=12;
    resolveQuestionSec(dataBuf,currentFlag);
}

void DNS_Data::debugPrintDNSData() {
    std::cout<<"\n***********************************************";
    std::cout<<"ID: "<<ID<<"\n";
    std::cout<<"QR: "<<QR<<"\n";
    std::cout<<"OPCODE: "<<OPCODE<<"\n";
    std::cout<<"AA: "<<AA<<"\n";
    std::cout<<"TC: "<<TC<<"\n";
    std::cout<<"RD: "<<RD<<"\n";
    std::cout<<"RA: "<<RA<<"\n";
    std::cout<<"Z: "<<Z<<"\n";
    std::cout<<"RCODE: "<<RCODE<<"\n";
    std::cout<<"QDCOUNT: "<<QDCOUNT<<"\n";
    std::cout<<"ANCOUNT: "<<ANCOUNT<<"\n";
    std::cout<<"NSCOUNT: "<<NSCOUNT<<"\n";
    std::cout<<"ARCOUNT: "<<ARCOUNT<<"\n";
    std::cout<<"QNAME: "<<qName<<"\n";
    std::cout<<"***********************************************\n";
}

std::string DNS_Data::getQName()
{
    return qName;
}

void DNS_Data::set16Bits(char one, char two, std::bitset<16> &bitsetDes)
{
    std::bitset<8> bits1(one);
    std::bitset<8> bits2(two);
    std::stringstream bitHelper;
    bitHelper<<bits1<<bits2;
    bitHelper>>bitsetDes;
}

void DNS_Data::resolveQuestionSec(boost::array<char, RECE_BUFFER_SIZE> &dataBuf,
                                  int currentFlag)
{
    std::bitset<8> nameCounter(dataBuf[currentFlag++]);
    int counter=nameCounter.to_ulong();
    while(counter>0)
    {
        qName+=dataBuf[currentFlag++];
        counter--;
        if(counter==0)
        {
            std::bitset<8> nameCounter(dataBuf[currentFlag++]);
            counter=nameCounter.to_ulong();
            if(counter)
                qName+='.';
        }
    }
}

DNS_Data::DNS_Data() {

}
