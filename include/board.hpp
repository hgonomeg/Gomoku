#pragma once
#include <vector>

enum class field {
    empty,
    cross,
    nought
};

class board {
protected:
    std::vector<std::vector<field>> fields;
public:
    board(std::size_t board_size);
    ~board();
};