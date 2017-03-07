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
    enum RR_TYPE{CNAME,A,AAAA,PTR,HINFO,MX,NS}; // ...etc.
    const std::string REMOTE_DNS_SERVER="10.3.9.6";

    DNS_Relay_Server(int port)
    :serverPort(port)
    {

    };
    void start();
    void stop();
    void resovleDNSData(boost::array<char,RECE_BUFFER_SIZE> &receiveBuf,DNS_Data& dnsData);
    void handleDNSRequest(udp::socket& sendSocket,DNS_Data& request,udp::endpoint& remoteEndPoint);
    void responseClient();
    void requestOther();


    std::string rrConstructor(std::string name,int type,int class_in,int ttl,int rlength, std::string address);
    // get Resource Records
    std::string getRRData();
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


    std::string getHeaderData();


    std::string getQName();

    void debugPrintDNSData();

private:
    int resolveQuestionSec(boost::array<char,RECE_BUFFER_SIZE> &dataBuf,int currentFlag);
    std::string getFlag();

    std::bitset<16> ID;
    std::bitset<1> QR;
public:
    void setANCOUNT(const std::bitset<16> &ANCOUNT);

    void setQR(const std::bitset<1> &QR);

private:
    std::bitset<4> OPCODE;
    std::bitset<1> AA;
    std::bitset<1> TC;
    std::bitset<1> RD;
    std::bitset<1> RA;
    std::bitset<3> Z;
public:
    const std::bitset<16> &getQTYPE() const;

private:
    std::bitset<4> RCODE;
    std::bitset<16> QDCOUNT;
    std::bitset<16> ANCOUNT;
    std::bitset<16> NSCOUNT;
    std::bitset<16> ARCOUNT;

    std::bitset<16> QTYPE;
public:
    void setQTYPE(const std::bitset<16> &QTYPE);

    void setQCLASS(const std::bitset<16> &QCLASS);

public:
    void setRD(const std::bitset<1> &RD);

    void setRA(const std::bitset<1> &RA);

    void setRCODE(const std::bitset<4> &RCODE);

private:
    std::bitset<16> QCLASS;

    std::string qName;
    std::string questionSection;
    std::string idStr;
    std::string flagStr;
    std::string rcodeStr;
    std::string qdCountStr;
    std::string nsCountStr;
    std::string arCountStr;

    int dataSize;

};



#endif //DNS_RELAY_SERVER_DNS_RELAY_SERVER_H
