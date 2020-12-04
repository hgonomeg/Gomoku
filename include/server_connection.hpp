#pragma once
#include "game_controller.hpp"
#include "server.hpp"

class server_connection : game_controller {
    board* m_board;
    field last_move;
    network::client_type m_type;
    unsigned int match_id, last_rx_packet_id, last_tx_packet_id;
    unsigned short port;
    sf::IpAddress server_ip;
    sf::UdpSocket comm;
    std::thread listener;
    void listener_fx();
    virtual field get_current_player() override;
    virtual bool register_move(board*,std::size_t,std::size_t) override;
public:
    server_connection(board*,sf::IpAddress,network::client_type,unsigned short port);
    ~server_connection();
};