#pragma once
#include <SFML/Graphics.hpp>
#include "board.hpp"
#include "resource_manager.hpp"
#include <memory>

class game_window :public board {
    sf::RenderWindow m_window;
    sf::VertexArray vertical_lines[32];
    sf::VertexArray horizontal_lines[32];
    std::shared_ptr<resource_manager> res;

    void distribute_lines();
    void draw_board();

public:
    void process_events();
    void draw();
    bool is_alive();
    game_window(std::shared_ptr<resource_manager>);
    ~game_window();
};