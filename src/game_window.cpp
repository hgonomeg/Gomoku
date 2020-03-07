#include "game_window.hpp"

game_window::game_window(std::shared_ptr<resource_manager> m_res)
:m_window(sf::VideoMode(512,512),"Gomoku game"),
board(32),
res(m_res) {
    m_window.setVerticalSyncEnabled(true);
    distribute_lines();
}

game_window::~game_window() {
    if(m_window.isOpen()) {
        m_window.close();
    }
}

void game_window::process_events() {
    sf::Event event;
    while(m_window.pollEvent(event)) {
        switch(event.type) {
            default: {
                break;
            };
            case sf::Event::Closed: {
                m_window.close();
                break;
            }
        }
    }
}

bool game_window::is_alive() {
    return m_window.isOpen();
}

void game_window::draw() {
    m_window.clear(sf::Color::White);
    for(const auto& x: vertical_lines) {
        m_window.draw(x);
    }
    for(const auto& x: horizontal_lines) {
        m_window.draw(x);
    }
    draw_board();
    m_window.display();
}

void game_window::distribute_lines() {
    for(std::size_t i = 0; i < 32 ; i++) {
        vertical_lines[i].clear();
        horizontal_lines[i].clear();
    }
    for(std::size_t i = 0; i < 32 ; i++) {
        sf::VertexArray inserted_h(sf::Lines);
        inserted_h.append(sf::Vertex(sf::Vector2f(0,i*16),sf::Color::Black));
        inserted_h.append(sf::Vertex(sf::Vector2f(512,i*16),sf::Color::Black));
        horizontal_lines[i] = inserted_h;

        sf::VertexArray inserted_v(sf::Lines);
        inserted_v.append(sf::Vertex(sf::Vector2f(i*16,0),sf::Color::Black));
        inserted_v.append(sf::Vertex(sf::Vector2f(i*16,512),sf::Color::Black));
        vertical_lines[i] = inserted_v;
    }
}

void game_window::draw_board() {
    sf::RectangleShape nought;
    nought.setTexture(res->nought);
    nought.setSize({16,16});

    sf::RectangleShape cross;
    nought.setTexture(res->cross);
    nought.setSize({16,16});
    
    for(std::size_t x = 0; x < 32; x++)
        for(std::size_t y = 0; y < 32; y++) {
            switch(fields[x][y]) {
                case field::empty: {
                    break;
                }
                case field::cross: {
                    cross.setPosition(x*16,y*16);
                    m_window.draw(cross);
                    break;
                }
                case field::nought: {
                    nought.setPosition(x*16,y*16);
                    m_window.draw(nought);
                    break;
                }
            }
        }
        
}