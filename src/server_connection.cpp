#include "server_connection.hpp"

server_connection::server_connection(board* brd,sf::IpAddress ip,network::client_type tp, unsigned short port) {
    m_board = brd;
    server_ip = ip;
    m_type = tp;
    this->port = port;
    comm.bind(port);
    listener = std::thread(&server_connection::listener_fx,this);
    last_move = field::empty;
}

server_connection::~server_connection() {
    if(listener.joinable())
        listener.join();
}

field server_connection::get_current_player() {
    return field::cross;
}

bool server_connection::register_move(board* bd,std::size_t x,std::size_t y) {
    if(bd!=m_board) {
        throw std::runtime_error("server_connection::register_move received improper board");
    }
    return false;
}

void server_connection::listener_fx() {

}