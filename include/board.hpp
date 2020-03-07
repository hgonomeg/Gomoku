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
    void set_value(std::size_t,std::size_t,field);
    field get_value(std::size_t,std::size_t);
    field victory() const;
    ~board();
};