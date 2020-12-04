#include "board.hpp"

board::board(std::size_t board_size) 
{
    for(std::size_t i = 0; i<board_size ; i++) {
        std::vector<field> empty_column;
        for(std::size_t j = 0; j<board_size ; j++) {
            empty_column.push_back(field::empty);
        }
        fields.push_back(empty_column);
    }
}

board::~board() {

}

field board::victory() const {
    std::lock_guard<std::mutex> lo(data_mut);

    auto has_five_in_a_row = [this](std::size_t x, std::size_t y) -> field {
        const std::pair<unsigned char,unsigned char> seek_directions[] = {
            {1,0},
            {0,1},
            {1,1} 
        };
        auto counted_field = fields[x][y];
        if(counted_field != field::empty) {
            for(const auto& direction: seek_directions) {
                std::pair<std::size_t,std::size_t> next_field = {x + direction.first, y + direction.second};
                unsigned char count = 1;
                while(count<5) {
                    if(next_field.first >= fields.size() || next_field.second >= fields.size()) 
                        break;
                    if(fields[next_field.first][next_field.second] != counted_field)
                        break;
                    count++;
                    if(count == 5)
                        return counted_field;
                    next_field.first += direction.first;
                    next_field.second += direction.second;
                }
            }
        }
        return field::empty;
    };

    for(std::size_t x = 0; x<fields.size(); x++) {
        for(std::size_t y = 0; y<fields.size(); y++) {
            auto ret = has_five_in_a_row(x,y);
            if(ret != field::empty) 
                return ret;
        }
    }
    return field::empty;
}

void board::set_value(std::size_t x,std::size_t y,field fd) {
    std::lock_guard<std::mutex> lo(data_mut);
    if(x>=fields.size()||y>=fields.size())
        throw std::out_of_range("board::set_value");
    fields[x][y] = fd;
}
field board::get_value(std::size_t x ,std::size_t y) const {
    std::lock_guard<std::mutex> lo(data_mut);
    if(x>=fields.size()||y>=fields.size())
        throw std::out_of_range("board::get_value");
    return fields[x][y];
}