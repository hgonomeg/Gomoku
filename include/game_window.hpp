#pragma once
#include <SFML/Graphics.hpp>
#include "board.hpp"
#include "resource_manager.hpp"
#include "game_controller.hpp"
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <functional>
#include <list>

class game_window :public board {
    std::unique_ptr<game_controller> controller;

    sf::RenderWindow m_window;
    sf::VertexArray vertical_lines[32];
    sf::VertexArray horizontal_lines[32];
    std::size_t cell_size;

    std::shared_ptr<resource_manager> res;
    std::thread logic_thread;
    std::list<std::function<void()>> logic_async_queue;
    std::mutex logic_mut;

    void distribute_lines();
    void draw_board();
    void thread_fx();
    bool is_alive_logic_async();
    void logic_async(std::function<void()>);
    std::size_t compute_cell_size();

public:
    void process_events();
    void draw();
    bool is_alive() const;
    game_window(std::shared_ptr<resource_manager>,game_controller*);
    ~game_window();
};