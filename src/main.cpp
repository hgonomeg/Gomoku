#include <SFML/Graphics.hpp>
#include "game_window.hpp"
#include "resource_manager.hpp"
#include "game_controller.hpp"
#include "hotseat.hpp"

int main() {
    std::shared_ptr<resource_manager> res(new resource_manager());
    game_window single_game(res,new hotseat());
    while(single_game.is_alive()) {
        single_game.process_events();
        single_game.draw();
        std::this_thread::yield();
    }
}