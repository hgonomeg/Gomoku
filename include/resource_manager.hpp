#include <SFML/Graphics.hpp>
#pragma once

struct resource_manager {
    sf::Texture* nought; 
    sf::Texture* cross;
    resource_manager();
    ~resource_manager();

};