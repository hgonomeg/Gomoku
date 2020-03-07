#include "board.hpp"

board::board(std::size_t board_size) 
{
    for(std::size_t i = 0; i<board_size ; i++) {
        std::vector<field> empty_row;
        for(std::size_t j = 0; j<board_size ; j++) {
            empty_row.push_back(field::empty);
        }
        fields.push_back(empty_row);
    }
}

board::~board() {

}

field board::victory() const {
    return field::empty;
}

void board::set_value(std::size_t x,std::size_t y,field fd) {
    fields[x][y] = fd;
}
field board::get_value(std::size_t x ,std::size_t y) {
    return fields[x][y];
}