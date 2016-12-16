//  rulebook.hpp
//  Oni
//  Copyright © 2016/2017 Benjamin Brinkmann, Alexandar Tomic, Georg Wolffgang. All rights reserved.

#ifndef rulebook_hpp
#define rulebook_hpp
#include "board.hpp"
#include "card.hpp"

bool check_Card(int from[2], oni_card used_card, int to[2], bool move_first_player);
int check_Infiltration(oni_board board);
bool check_Piece(oni_board board, int piece[2], bool move_first_player);

#endif /* rulebook_hpp */
