//
// Created by thief on 17-3-4.
//
#include <iostream>
#include <boost/array.hpp>
#include "DNS_Relay_Server.h"
#include "DNS_utils.h"

void DNS_Relay_Server::start()
{
    try
    {
        udp::socket socket(ioService, udp::endpoint(udp::v4(), serverPort));

        while(true)
        {
            // TODO
            boost::array<char,RECE_BUFFER_SIZE> recvBuf;
            int bufferSize=0;
            udp::endpoint remoteEndpoint;
            boost::system::error_code error;
            bufferSize=socket.receive_from(boost::asio::buffer(recvBuf),
                                remoteEndpoint, 0, error);

            DNS_Data dnsData;

         //   socket.send_to(recvBuf,remoteEndpoint);
            resovleDNSData(recvBuf,dnsData);
            std::cout<<"receive DNS Data: \n";
            dnsData.debugPrintDNSData();

            handleDNSRequest(socket,dnsData,remoteEndpoint);
            std::cout<<"response DNS Data: \n";
            dnsData.debugPrintDNSData();
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

void DNS_Relay_Server::handleDNSRequest(udp::socket& sendSocket,DNS_Data &request, udp::endpoint &remoteEndPoint) {
    std::string queryName=request.getQName();
    // TODO query
    std::string responseStr;
    request.setQR(1);
    request.setRA(1);
    request.setRD(1);
    request.setANCOUNT(1);
    responseStr+=request.getHeaderData();
    responseStr+=rrConstructor("www.baidu.com",A,1,1,4,"0.0.0.1");

    std::cout<<">>"<<responseStr<<"  <<";
   // udp::socket responSocket(ioService);
    sendSocket.send_to(boost::asio::buffer(responseStr),remoteEndPoint);
}

std::string
DNS_Relay_Server::rrConstructor(std::string name, RR_TYPE type, int class_in, int ttl,int rlength, std::string address) {
    std::stringstream bitHelper;

    bitHelper<<domainNameToBitStr(name);
    std::bitset<16> rType(1);
    std::bitset<16> rClass(class_in);
    std::bitset<32> rTtl(ttl);
    std::bitset<16> rLen(rlength);
    bitHelper<<getStrFrom16Bits(rType)<<getStrFrom16Bits(rClass)<<getStrFrom32Bits(rTtl)<<getStrFrom16Bits(rLen);
    bitHelper<<addressToBitStr(address);
    return bitHelper.str();
}


DNS_Data::DNS_Data(boost::array<char, RECE_BUFFER_SIZE> &dataBuf) {
    this->resolveData(dataBuf);
}

/* resolve dns data*/
void DNS_Data::resolveData(boost::array<char, RECE_BUFFER_SIZE> &dataBuf)
{
    /* header section */
    // id
    set16Bits(dataBuf[0],dataBuf[1],ID);
    idStr+=dataBuf[0];
    idStr+=dataBuf[1];

    std::stringstream bitHelper;
    std::bitset<8> flagBits1(dataBuf[2]);
    std::bitset<8> flagBits2(dataBuf[3]);
    bitHelper<<flagBits1<<flagBits2;
    bitHelper>>QR>>OPCODE>>AA>>TC>>RD>>RA>>Z>>RCODE;

    set16Bits(dataBuf[4],dataBuf[5],QDCOUNT);
    set16Bits(dataBuf[6],dataBuf[7],ANCOUNT);
    set16Bits(dataBuf[8],dataBuf[9],NSCOUNT);
    set16Bits(dataBuf[10],dataBuf[11],ARCOUNT);

    //question section
    int currentFlag=12;
    currentFlag=resolveQuestionSec(dataBuf,currentFlag);
    set16Bits(dataBuf[currentFlag],dataBuf[currentFlag+1],QTYPE);
    set16Bits(dataBuf[currentFlag+2],dataBuf[currentFlag+3],QCLASS);

    //currentFlag+=4;
    //resource section

}

void DNS_Data::debugPrintDNSData() {
    std::cout<<"\n***********************************************\n";
    std::cout<<"ID: "<<ID<<"  "<<ID.to_ulong()<<"\n";
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


int DNS_Data::resolveQuestionSec(boost::array<char, RECE_BUFFER_SIZE> &dataBuf,
                                  int currentFlag)
{
    std::bitset<8> nameCounter(dataBuf[currentFlag]);
    questionSection+=dataBuf[currentFlag];
    currentFlag++;

    int counter=nameCounter.to_ulong();
    while(counter>0)
    {
        qName+=dataBuf[currentFlag];
        questionSection+=dataBuf[currentFlag];
        currentFlag++;

        counter--;
        if(counter==0)
        {
            std::bitset<8> nameCounter(dataBuf[currentFlag]);
            questionSection+=dataBuf[currentFlag];
            currentFlag++;

            counter=nameCounter.to_ulong();
            if(counter)
                qName+='.';
        }
    }
    questionSection+=(char)NULL;
    questionSection+=getStrFrom16Bits(QTYPE);
    questionSection+=getStrFrom16Bits(QCLASS);

    return currentFlag;
}

DNS_Data::DNS_Data() {

}

std::string DNS_Data::getHeaderData() {
    std::stringstream bitHelper;

    bitHelper<<idStr<<getFlag()<<getStrFrom16Bits(QDCOUNT)<<getStrFrom16Bits(ANCOUNT)
             <<getStrFrom16Bits(NSCOUNT)<<getStrFrom16Bits(ARCOUNT);
    bitHelper<<questionSection;
    std::cout<<"\n>>bitHelper :"<<bitHelper.str()<<" \n";
    return bitHelper.str();
}

void DNS_Data::setQR(const std::bitset<1> &QR) {
    DNS_Data::QR = QR;
}

void DNS_Data::setRD(const std::bitset<1> &RD) {
    DNS_Data::RD = RD;
}

void DNS_Data::setRA(const std::bitset<1> &RA) {
    DNS_Data::RA = RA;
}

std::string DNS_Data::getFlag() {
     std::bitset<16> flagBits;
    std::stringstream bitHelper;

    bitHelper<<QR<<OPCODE<<AA<<TC<<RD<<RA<<Z<<RCODE;
    bitHelper>>flagBits;
    return getStrFrom16Bits(flagBits);
}

void DNS_Data::setANCOUNT(const std::bitset<16> &ANCOUNT) {
    DNS_Data::ANCOUNT = ANCOUNT;
}
