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
    class match;

    namespace communication_protocol {
        
    }

    class client {
        sf::IpAddress ip_address;
        sf::UdpSocket socket;
        unsigned short port;
        client_type type;
        unsigned int match_id, last_rx_packet_id, last_tx_packet_id;
        std::string nick;
        std::mutex mut; //protect every data field
        client(std::string,client_type,sf::IpAddress,unsigned short port);

        friend class server;
        friend class match;
    };

    class match {
        board game;
        field current_round;
        std::shared_ptr<client> cross;
        std::shared_ptr<client> nought;
        std::list<std::shared_ptr<client>> passives;
        mutable std::mutex mut; //protect everything but the active clients (the two pointers)
        std::mutex& server_client_existence; //lock when deleting any client
        bool deconstruct; //if the match should end now or if it has completed
        std::thread match_thread;
        void work_thread();
        public: 
        bool should_deconstruct() const;
        std::weak_ptr<client> push_passive_client(std::shared_ptr<client>);
        match(std::shared_ptr<client> cross,std::shared_ptr<client> nought, std::mutex& serv_client_existence);
        ~match();
    };

    class server {
        sf::UdpSocket discovery_socket;

        std::map<unsigned int,match> matches;
        mutable std::mutex matches_clients_existence; //lock to ensure that clients don't disappear when iterating
        std::list<std::weak_ptr<client>> clients;
        std::atomic_ushort comm_port;
        std::mutex cout_mut;
        mutable std::mutex global_state_mut;
        bool not_quit_time;
        void main_loop();
        bool not_quit() const;
        std::thread loop_thread;
        void list_clients() const;
        void list_matches() const;
        void drop_match(unsigned int);
        //void kick_client(sf::IpAddress );
    public:
        server(unsigned short port = 35567);
        ~server();
        void console();
    };
}
