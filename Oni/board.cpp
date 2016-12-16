//  board.cpp
//  Oni
//  Copyright © 2016 Georg Wolffgang. All rights reserved.

#include "board.hpp"

oni_board::oni_board() {
    char position[5][5] = {
        {'s','s','m','s','s'},
        {' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' '},
        {'S','S','M','S','S'}
    };
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++)
        memcpy(&board[x][y], &position[4-y][x], sizeof(char));
}

ostream& operator<<(ostream& output, const oni_board& board) {
    output << "-----------" << endl;
    for (int y = 4; y>-1;y--) {
        for (int x = 0; x<5; x++)
            output << "|" << board.board[x][y];
        output << "|" << endl;
        output << "-----------" << endl;
    }
    return output;
}

