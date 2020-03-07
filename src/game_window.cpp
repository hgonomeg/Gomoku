#include "game_window.hpp"

game_window::game_window(std::shared_ptr<resource_manager> m_res,game_controller* m_contr)
:m_window(sf::VideoMode(512,512),"Gomoku game"),
board(32),
controller(m_contr),
res(m_res) {
    m_window.setVerticalSyncEnabled(true);
    distribute_lines();
    compute_cell_size();
}

game_window::~game_window() {
    if(m_window.isOpen()) {
        m_window.close();
    }
}

std::size_t game_window::compute_cell_size() {
    auto view = m_window.getView();
    cell_size = view.getSize().x / 32;
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
            case sf::Event::MouseButtonPressed: {
                auto mbp = event.mouseButton;
                std::size_t x = mbp.x / cell_size;
                std::size_t y = mbp.y / cell_size;
                if(y<32 && x<32)
                {
                    controller->register_move(this,x,y);

                }
                break;
            }
            case sf::Event::MouseButtonReleased: {
                break;
            }
            case sf::Event::MouseMoved: {
                break;
            }
            case sf::Event::Resized: {
                auto size = event.size;
                unsigned int edge = std::min(size.height,size.width);
                while(edge%32!=0) {
                    edge--;
                }
                m_window.setSize({edge,edge});
                sf::View new_view;
                new_view.setSize(edge,edge);
                new_view.setCenter(edge/2,edge/2);
                m_window.setView(new_view);
                compute_cell_size();
                distribute_lines();
                break;
            }
        }
    }
}

bool game_window::is_alive() const {
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
        inserted_h.append(sf::Vertex(sf::Vector2f(0,i*cell_size),sf::Color::Black));
        inserted_h.append(sf::Vertex(sf::Vector2f(cell_size*32,i*cell_size),sf::Color::Black));
        horizontal_lines[i] = inserted_h;

        sf::VertexArray inserted_v(sf::Lines);
        inserted_v.append(sf::Vertex(sf::Vector2f(i*cell_size,0),sf::Color::Black));
        inserted_v.append(sf::Vertex(sf::Vector2f(i*cell_size,cell_size*32),sf::Color::Black));
        vertical_lines[i] = inserted_v;
    }
}

void game_window::draw_board() {
    sf::RectangleShape nought;
    nought.setTexture(res->nought);
    nought.setSize({cell_size,cell_size});

    sf::RectangleShape cross;
    cross.setTexture(res->cross);
    cross.setSize({cell_size,cell_size});
    
    for(std::size_t x = 0; x < 32; x++)
        for(std::size_t y = 0; y < 32; y++) {
            switch(fields[x][y]) {
                case field::empty: {
                    break;
                }
                case field::cross: {
                    cross.setPosition(x*cell_size,y*cell_size);
                    m_window.draw(cross);
                    break;
                }
                case field::nought: {
                    nought.setPosition(x*cell_size,y*cell_size);
                    m_window.draw(nought);
                    break;
                }
            }
        }
        
}