#pragma once
#include "game_controller.hpp"
#include <random>
#include <chrono>

class hotseat :public game_controller {
    field last_move;
public:
    virtual field get_current_player() override;
    virtual bool register_move(board*,std::size_t,std::size_t) override;
    hotseat();
    ~hotseat();
};