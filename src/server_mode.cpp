#include "server_mode.hpp"

void server_mode() {
    using namespace network;
    std::cout<<rang::fg::yellow<<"Starting server mode."<<rang::fg::reset<<std::endl;
    server m_server;
    m_server.console();
    std::cout<<rang::fg::yellow<<"Server closed."<<rang::fg::reset<<std::endl;

}