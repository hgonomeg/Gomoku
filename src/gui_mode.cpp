#include "gui_mode.hpp"

void gui_mode() {
    auto fut = std::async([](){
        std::shared_ptr<resource_manager> ret(new resource_manager());
        return ret;
    });
    

    std::shared_ptr<resource_manager> res = fut.get();
    game_window single_game(res,new hotseat());
    while(single_game.is_alive()) {
        single_game.process_events();
        single_game.draw();
        std::this_thread::yield();
    }
}