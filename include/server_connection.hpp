#pragma once
#include "game_controller.hpp"
#include "server.hpp"

class server_connection : game_controller {
    virtual field get_current_player() override;
    virtual bool register_move(board*,std::size_t,std::size_t) override;
public:
    server_connection();
    ~server_connection();
};