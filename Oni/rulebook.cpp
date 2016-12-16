//  rulebook.cpp
//  Oni
//  Copyright © 2016 Georg Wolffgang. All rights reserved.

#include "rulebook.hpp"

bool check_Card(int from[2], oni_card used_card, int to[2], bool move_first_player) {
    int flip_card = (2 * move_first_player) -1;
    for (int i=0; i<4; i++) {
        if (to[0] == from[0] + (4 - used_card.get_y_from_choice(i)) * flip_card &&
            to[1] == from[1] + used_card.get_x_from_choice(i) * flip_card)
            return true;
    }
    return false;
}

int check_Infiltration(oni_board board) {
    if (board.get_Field(2,0) == 'm') return -1;
    if (board.get_Field(2,4) == 'M') return 1;
    return 0;
}

bool check_Piece(oni_board board, int piece[2], bool move_first_player) {
    char piece_char = board.get_Field(piece);
    if (move_first_player) {
        if (piece_char == 'S' || piece_char == 'M') return true;
    } else {
        if (piece_char == 's' || piece_char == 'm') return true;
    }
    return false;
}
