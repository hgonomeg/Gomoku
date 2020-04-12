#include "server.hpp"
#include <sstream>
namespace network {

    server::server() 
    :loop_thread(&server::main_loop,this) {
        global_state_mut.lock();
        not_quit_time = true;
        listener.setBlocking(false);
        global_state_mut.unlock();
    }

    server::~server() {
        global_state_mut.lock();
        not_quit_time = false;
        global_state_mut.unlock();
        if(loop_thread.joinable())
            loop_thread.join();
    }

    void server::main_loop() {
        while(not_quit()) {
            std::this_thread::yield();
        }
    }

    bool server::not_quit() const {
        std::lock_guard<std::mutex> mlock(global_state_mut);
        return not_quit_time;
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
                    cout<<"There are currently no available commands."<<endl;
                } else {
                    std::lock_guard<std::mutex> ctl(cout_mut);
                    cout<<"Unknown command!"<<endl;
                }
            }
        };

        cout<<"Gomoku server."<<endl;

        std::string command;
        while(command!="quit"&&command!="exit"&&command!="stop") {
            handle_command(command);
            cout_mut.lock();
            cout<<endl<<"> ";
            cout_mut.unlock();
            getline(cin,command);
        }
    }

    client::client(std::string nk ,client_type tp ,sf::IpAddress ip) 
    :nick(nk),
    type(tp) {
        ip_address = ip;
        match_id = 0;
    }

    match::match(client* c,client* n) 
    :game(32) {
        cross = c;
        nought = n;
    }

}