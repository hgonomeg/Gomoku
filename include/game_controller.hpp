#pragma once
#include "board.hpp"

class game_controller {

public:
    virtual field get_current_player() = 0;
    virtual bool register_move(board*,std::size_t,std::size_t) = 0;
    game_controller();
    virtual ~game_controller();
};