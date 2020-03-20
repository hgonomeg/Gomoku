#include "server_mode.hpp"

void server_mode() {
    using namespace network;
    std::cout<<"Starting server mode."<<std::endl;
    server m_server;
    m_server.console();
    std::cout<<"Server closed."<<std::endl;

}