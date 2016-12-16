//  oni.cpp
//  oni
//  Copyright © 2016 Georg Wolffgang. All rights reserved.

#include "oni.hpp"
#include <algorithm>
#include <time.h>

void oni::capture_piece(const char target) {
    switch (target) {
        case 'S':
            students_left[0]--;
            break;
        case 's':
            students_left[1]--;
            break;
        case 'M':
            game_result = -1;
            break;
        case 'm':
            game_result = 1;
    }
}

int myrandom(int i) { return std::rand()%i; }

oni::oni() {
    int int_array[16] = {1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16};
    srand(unsigned(time(NULL)));
    random_shuffle(&int_array[0], &int_array[15], myrandom);
    card_neutral = int_array[0];
    card[0][0] = int_array[1];
    card[0][1] = int_array[2];
    card[1][0] = int_array[3];
    card[1][1] = int_array[4];
}

void oni::load_game() {
    
}

int oni::make_turn(int from[2], bool first_card, int to[2]) {
    /* ERROR-CODES: 0 - All fine and move done,
                    1 - No piece to move,
                    2 - Card does not allow this move,
                    3 - Targeted piece not allowed to capture
                    4 - Game already ended
                    5 - Illigal coordinates */
    if (game_result == 0) {
        if (from[0] > -1 && from[0] < 5 && from[1] > -1 && from[1] < 5
            && to[0] > -1 && to[0] < 5 && to[1] > -1 && to[1] < 5) {
            
            oni_card used_card;
            char piece = board.get_Field(from);
            if (piece == ' ')
                return 1;
            char target = board.get_Field(to);
            if (abs(piece-target) < 10)
                return 3;
            used_card = card[!move_first_player][!first_card];
            if (move_first_player) {
                if (piece != 'M' && piece != 'S')
                    return 1;
            } else {
                if (piece != 'm' && piece != 's')
                    return 1;
            }
            if (check_Card(from, used_card, to, move_first_player)) {
                capture_piece(target);
                board.set_Field(to, piece);
                board.set_Field(from, ' ');
                card[!move_first_player][!first_card] = card_neutral;
                card_neutral = used_card;
                move_first_player = !move_first_player;
            } else
                return 2;
            game_result = check_Infiltration(board);
            return 0;
        } else
            return 5;
    }
    return 4;
}

void oni::save_game() {
    
}

void oni::show_board() {
    cout << board;
}

void oni::show_cards() {
    cout << "Cards player 1: " << card[0][0] << " + " << card[0][1] << endl;
    cout << "Cards player 2: " << card[1][0] << " + " << card[1][1] << endl;
    cout << "  Neutral Card: " << card_neutral << endl;
}
