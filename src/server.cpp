#include "server.hpp"
#include <sstream>
namespace network {

    server::server(unsigned short port) 
    :not_quit_time(true),
    comm_port(port+1) {
        global_state_mut.lock();
        discovery_socket.setBlocking(false);
        discovery_socket.bind(port);
        global_state_mut.unlock();
        loop_thread = std::thread(&server::main_loop,this);
    }

    server::~server() {
        global_state_mut.lock();
        not_quit_time = false;
        global_state_mut.unlock();
        if(loop_thread.joinable())
            loop_thread.join();
    }

    void server::main_loop() {
        cout_mut.lock();
        std::cout<<rang::fg::yellow<<"[MAIN LOOP]: Starting up."<<std::endl<<rang::fg::reset;
        cout_mut.unlock();
        while(not_quit()) {
            std::this_thread::yield();
        }
        cout_mut.lock();
        std::cout<<rang::fg::yellow<<"[MAIN LOOP]: Exiting."<<std::endl<<rang::fg::reset;
        cout_mut.unlock();
    }

    bool server::not_quit() const {
        std::lock_guard<std::mutex> mlock(global_state_mut);
        return not_quit_time;
    }

    void server::list_clients() const {

    }
    void server::list_matches() const {

    }
    void server::drop_match(unsigned int m_id) {

    }

    void server::console() {
        using std::cout;
        using std::endl;
        using std::cin;
        using std::getline;

        auto handle_command = [this](std::string command){
            if(!command.empty()) {
                std::stringstream command_str(command);
                command_str>>command;
                if(command=="help") {
                    std::lock_guard<std::mutex> ctl(cout_mut);
                    cout<<rang::fg::green<<"There are currently no available commands."<<rang::fg::reset<<endl;
                } else if(command=="list" || command=="ls") {
                    if(!command_str.eof()) {
                        command_str>>command;
                        if(command=="clients"||command=="client") {

                        } else if(command=="matches"||command=="match") {

                        } else {
                            std::lock_guard<std::mutex> ctl(cout_mut);
                            cout<<rang::fg::red<<"Unknown sub-command!"<<rang::fg::reset<<endl;
                        }
                    } else {
                        list_matches();
                        list_clients();
                    }
                } else {
                    std::lock_guard<std::mutex> ctl(cout_mut);
                    cout<<rang::fg::red<<"Unknown command!"<<rang::fg::reset<<endl;
                }
            }
        };

        cout<<rang::fg::green<<"Gomoku server."<<rang::fg::reset<<endl;

        std::string command;
        while(command!="quit"&&command!="exit"&&command!="stop") {
            handle_command(command);
            cout_mut.lock();
            cout<<endl<<"> ";
            cout_mut.unlock();
            getline(cin,command);
        }
    }

    client::client(std::string nk ,client_type tp ,sf::IpAddress ip, unsigned short port) 
    :nick(nk),
    type(tp),
    ip_address(ip) {
        match_id = 0;
        last_rx_packet_id = 0;
        last_tx_packet_id = 0;
        socket.bind(port);
        this->port = port;
        socket.setBlocking(false);
    }

    match::match(std::shared_ptr<client> c,std::shared_ptr<client> n, std::mutex& sce) 
    :game(32),
    cross(c),
    nought(n),
    server_client_existence(sce),
    deconstruct(false),
    match_thread(&match::work_thread,this) {
        std::lock_guard<std::mutex> mlock(mut);
        current_round = field::nought; //to be randomized I guess
    }

    std::weak_ptr<client> match::push_passive_client(std::shared_ptr<client> new_client) {
        std::weak_ptr<client> ret(new_client);
        std::lock_guard<std::mutex> mlock(mut);
        passives.push_back(std::move(new_client));
        return ret;
    }

    void match::work_thread() {
        while(true) {
            mut.lock();
            if(deconstruct)
                break;
            mut.unlock();
            std::this_thread::yield();
        }
        mut.unlock();
    }

    bool match::should_deconstruct() const {
        std::lock_guard<std::mutex> mlock(mut);
        return deconstruct;
    }

    match::~match() {
        mut.lock();
        deconstruct = true;
        mut.unlock();
        if(match_thread.joinable())
            match_thread.join();
        server_client_existence.lock();
        cross = nullptr;
        nought = nullptr;
        mut.lock();
        passives.clear();
        server_client_existence.unlock();
        mut.unlock();

    }

}