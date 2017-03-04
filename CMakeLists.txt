cmake_minimum_required(VERSION 3.6)
project(dns_relay_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lboost_system -lpthread")

set(SOURCE_FILES main.cpp DNS_Relay_Server.cpp)
add_executable(dns_relay_server ${SOURCE_FILES})