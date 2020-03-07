#include "resource_manager.hpp"

resource_manager::resource_manager() {
    this->nought = new sf::Texture();
    this->cross = new sf::Texture();
    nought->loadFromFile("resources/nought.png");
    cross->loadFromFile("resources/cross.png");
}

resource_manager::~resource_manager() {
    delete this->nought;
    delete this->cross;
}