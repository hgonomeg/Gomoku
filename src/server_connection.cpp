#include "server_connection.hpp"

server_connection::server_connection() {

}

server_connection::~server_connection() {

}

field server_connection::get_current_player() {
    return field::cross;
}

bool server_connection::register_move(board* bd,std::size_t x,std::size_t y) {
    return false;
}