#pragma once
#include <iostream>
#include <memory>
#include <atomic>
#include <list>
#include <thread>
#include <mutex>
#include <map>
#include <SFML/Network.hpp>
#include "board.hpp"
#include <nlohmann/json.hpp> 
#include <rang.hpp>

namespace network {

    enum class client_type {
        active,
        passive
    };

    class server;

    namespace communication_protocol {
        
    }

    class client {
        sf::IpAddress ip_address;
        sf::UdpSocket socket;
        client_type type;
        unsigned int match_id, last_rx_packet_id, last_tx_packet_id;
        std::string nick;
        std::mutex mut;
        client(std::string,client_type,sf::IpAddress);

        friend class server;
    };

    class match {
        board game;
        field current_round;
        client* cross;
        client* nought;
        std::list<std::shared_ptr<client>> passives;
        std::mutex mut;

        public: 
        match(client*,client*);
        friend class server;
    };

    class server {
        sf::UdpSocket discovery_socket;

        std::map<unsigned int,match> matches;
        std::list<std::weak_ptr<client>> clients;
        std::atomic_ushort comm_port;
        std::mutex cout_mut;
        mutable std::mutex global_state_mut;
        bool not_quit_time;
        void main_loop();
        bool not_quit() const;
        std::thread loop_thread;

    public:
        server(unsigned short port = 35567);
        ~server();
        void console();
    };
}
