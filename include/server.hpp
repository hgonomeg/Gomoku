#pragma once
#include <iostream>
#include <memory>
#include <list>
#include <thread>
#include <mutex>
#include <map>
#include <SFML/Network.hpp>
#include "board.hpp"
// #include <nlohmann/json.hpp> This is considered an option

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
        sf::UdpSocket sender;
        client_type type;
        unsigned int match_id;
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
        std::mutex mut;

        public: 
        match(client*,client*);
        friend class server;
    };

    class server {
        sf::UdpSocket listener;

        std::map<unsigned int,match> matches;
        std::list<client> clients;
        std::mutex clients_mut;
        std::mutex cout_mut;
        void main_loop();
        
        std::thread loop_thread;

    public:
        server();
        ~server();
        void console();
    };
}
