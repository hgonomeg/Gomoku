#include "singleplayer.hpp"

singleplayer::singleplayer() {
    std::default_random_engine random_eng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distro(0,1);
    auto random_number =  distro(random_eng);
    if(random_number == 0) {
        last_move = field::cross;
    }
    else {
        last_move = field::nought;
    }

}

singleplayer::~singleplayer() {

}

field singleplayer::get_current_player() {
    if(last_move == field::cross)
        return field::nought;
    else
        return field::cross;
}

bool singleplayer::register_move(board* curr,std::size_t x,std::size_t y) {
    if(curr->get_value(x,y) == field::empty) {
        if(last_move == field::cross)
            last_move = field::nought;
        else
            last_move = field::cross;
        
        curr->set_value(x,y,get_current_player());
        return true;
    }
    return false;
}