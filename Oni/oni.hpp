//  oni.hpp
//  oni
//  Copyright © 2016/2017 Benjamin Brinkmann, Alexandar Tomic, Georg Wolffgang. All rights reserved.

#ifndef oni_hpp
#define oni_hpp

#include <iostream>
#include <algorithm>

#include "board.hpp"
#include "card.hpp"
#include "rulebook.hpp"
using namespace std;

class oni {
    
private:
    oni_card card[2][2];            // card: [player_number][card_number]
    oni_card card_neutral;          // communitycard
    oni_board board;
    int game_result = 0;            // game_result: 0 = ongoing game; 1 = player 1 has won; -1 = player 2 has won
    int students_left[2] = {4, 4};  // students_left: [0] = first player, [1] = second player
    bool move_first_player = true;  // move_first_player: true = turn of player 1, false = turn of player 2
    
    void capture_piece(const char target);
    
public:
    oni();
    
    void load_game();
    int make_turn(int from[2], bool first_card, int to[2]);
    void save_game();
    void show_board();
    void show_cards();
};

#endif /* oni_hpp */
