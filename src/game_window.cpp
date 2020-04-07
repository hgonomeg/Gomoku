#include "game_window.hpp"

game_window::game_window(std::shared_ptr<resource_manager> m_res,game_controller* m_contr)
:board(32),
controller(m_contr),
res(m_res),
m_window(sf::VideoMode(512,512),"Gomoku game") {
    logic_thread = std::thread(&game_window::thread_fx,this);
    std::lock_guard<std::mutex> lo(window_mut);
    m_window.setVerticalSyncEnabled(true);
    compute_cell_size();
    distribute_lines();
    rescale_threads_active = false;
}

game_window::~game_window() {
    window_mut.lock();
    if(m_window.isOpen()) {
        m_window.close();
    }
    window_mut.unlock();
    if(logic_thread.joinable()) logic_thread.join();
    while(true) {
        window_mut.lock();
        if(!rescale_threads_active)
            break;
        window_mut.unlock();
        std::this_thread::yield();
    }
}

std::size_t game_window::compute_cell_size() {
    auto view = m_window.getView();
    cell_size = view.getSize().x / 32;
    return cell_size;
}

void game_window::process_events() {
    sf::Event event;
    while(m_window.pollEvent(event)) {
        switch(event.type) {
            default: {
                break;
            };
            case sf::Event::Closed: {
                std::lock_guard lo(window_mut);
                m_window.close();
                break;
            }
            case sf::Event::MouseButtonPressed: {
                auto mbp = event.mouseButton;
                window_mut.lock();
                std::size_t x = mbp.x / cell_size;
                std::size_t y = mbp.y / cell_size;
                window_mut.unlock();
                if(y<32 && x<32 && mbp.button == sf::Mouse::Button::Left) 
                {
                    logic_async([this,x,y](){controller->register_move(this,x,y);});
                    
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
                static std::chrono::system_clock::time_point lastresize;
                static std::mutex lr_mut;

                lr_mut.lock();
                lastresize = std::chrono::system_clock::now();
                lr_mut.unlock();

                auto size = event.size;
                unsigned int edge = std::min(size.height,size.width);
                while(edge%32!=0) {
                    edge--;
                }

                auto defer_resize = [this,edge](std::chrono::system_clock::time_point& lastresize, std::mutex& lr_mut){
                    lr_mut.lock();
                    auto invoke_time = lastresize;
                    lr_mut.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(200)); //the time
                    lr_mut.lock();
                    auto final_time = lastresize;
                    lr_mut.unlock();
                    if(invoke_time == final_time) { //make sure that the last window resize happened 200ms ago in order to prevent the window from not maintaining its' aspect ratio 
                        std::lock_guard lo(window_mut); //prevent raid with drawing
                        m_window.setSize({edge,edge});
                        sf::View new_view;
                        new_view.setSize(edge,edge);
                        new_view.setCenter(edge/2,edge/2);
                        m_window.setView(new_view);
                        compute_cell_size();
                        distribute_lines();
                        rescale_threads_active = false;
                    }

                };
                std::lock_guard lo(window_mut);
                rescale_threads_active = true;

                std::thread deferred_resize(defer_resize,std::ref(lastresize),std::ref(lr_mut));
                deferred_resize.detach();
                break;
            }
        }
    }
}

bool game_window::is_alive() {
    std::lock_guard<std::mutex> lo(window_mut);
    return m_window.isOpen();
}

void game_window::draw() {
    window_mut.lock();
    m_window.clear(sf::Color::White);
    for(const auto& x: vertical_lines) {
        m_window.draw(x);
    }
    for(const auto& x: horizontal_lines) {
        m_window.draw(x);
    }
    window_mut.unlock();
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
    std::lock_guard<std::mutex> w_lo(window_mut);
    sf::RectangleShape nought;
    nought.setTexture(res->nought);
    nought.setSize({cell_size,cell_size});

    sf::RectangleShape cross;
    cross.setTexture(res->cross);
    cross.setSize({cell_size,cell_size});
    
    std::lock_guard<std::mutex> lo(data_mut);
    
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

void game_window::thread_fx() {

    auto fetch = [this]() -> std::function<void()> {
        std::lock_guard<std::mutex> lo(logic_mut);
        if(logic_async_queue.empty()) {
            return [](){};
        }
        auto last = logic_async_queue.front();
        logic_async_queue.pop_front();
        return last;
    };
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    while(is_alive()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        fetch()();
    }

}
void game_window::logic_async(std::function<void()> fx) {
    std::lock_guard<std::mutex> lo(logic_mut);
    logic_async_queue.push_back(std::move(fx));
}
