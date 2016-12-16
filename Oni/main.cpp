//  main.cpp
//  Oni
//  Copyright © 2016/2017 Benjamin Brinkmann, Alexandar Tomic, Georg Wolffgang. All rights reserved.

#include <iostream>
#include "oni.hpp"

int main(int argc, const char * argv[]) {
    oni test_game;
    test_game.show_cards();
    int from[2] = {2,0}, to[2] = {2,2};
    int exit = test_game.make_turn(from, false, to);
    cout << "Turn ended with exit code: " << exit << endl;
    test_game.show_board();
    test_game.show_cards();
    return 0;
}
