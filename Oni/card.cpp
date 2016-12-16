//  card.cpp
//  Oni
//  Copyright © 2016/2017 Benjamin Brinkmann, Alexandar Tomic, Georg Wolffgang. All rights reserved.

#include "card.hpp"

void oni_card::set_Cardvalues(){
    if (id == 0)
        id = randomizer(16);
    switch (id) {
        case 1:
            memcpy(name,"BOAR", 5 * sizeof(char));
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = 0;
            break;
        case 2:
            memcpy(name,"COBRA", 6 * sizeof(char));
            choice[0][0] = 1;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = -1;
            break;
        case 3:
            memcpy(name,"CRAB", 5 * sizeof(char));
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = -2;  choice[1][1] = 0;
            choice[2][0] = 2;   choice[2][1] = 0;
            break;
        case 4:
            memcpy(name,"CRANE", 6 * sizeof(char));
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = -1;
            choice[2][0] = 1;   choice[2][1] = -1;
            break;
        case 5:
            memcpy(name,"DRAGON", 7 * sizeof(char));
            choice[0][0] = -2;  choice[0][1] = 1;
            choice[1][0] = 2;   choice[1][1] = 1;
            choice[2][0] = -1;  choice[2][1] = -1;
            choice[3][0] = 1;   choice[3][1] = -1;
            break;
        case 6:
            memcpy(name,"EEL", 4 * sizeof(char));
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 0;
            choice[2][0] = -1;  choice[2][1] = -1;
            break;
        case 7:
            memcpy(name,"ELEPHANT", 9 * sizeof(char));
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 1;
            choice[2][0] = -1;  choice[2][1] = 0;
            choice[3][0] = 1;   choice[3][1] = 0;
            break;
        case 8:
            memcpy(name,"FROG", 5 * sizeof(char));
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = -2;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = -1;
            break;
        case 9:
            memcpy(name,"GOOSE", 6 * sizeof(char));
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = 0;
            choice[3][0] = 1;   choice[3][1] = -1;
            break;
        case 10:
            memcpy(name,"HORSE", 6 * sizeof(char));
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 0;   choice[2][1] = -1;
            break;
        case 11:
            memcpy(name,"MANTIS", 7 * sizeof(char));
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 1;
            choice[2][0] = 0;   choice[2][1] = -1;
            break;
        case 12:
            memcpy(name,"MONKEY", 7 * sizeof(char));
            choice[0][0] = -1;  choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 1;
            choice[2][0] = -1;  choice[2][1] = -1;
            choice[3][0] = 1;   choice[3][1] = -1;
            break;
        case 13:
            memcpy(name,"OX", 3 * sizeof(char));
            choice[0][0] = 0;   choice[0][1] = 1;
            choice[1][0] = 1;   choice[1][1] = 0;
            choice[2][0] = 0;   choice[2][1] = -1;
            break;
        case 14:
            memcpy(name,"RABBIT", 7 * sizeof(char));
            choice[0][0] = 1;   choice[0][1] = 1;
            choice[1][0] = 2;   choice[1][1] = 0;
            choice[2][0] = -1;  choice[2][1] = -1;
            break;
        case 15:
            memcpy(name,"ROOSTER", 8 * sizeof(char));
            choice[0][0] = 1;   choice[0][1] = 1;
            choice[1][0] = -1;  choice[1][1] = 0;
            choice[2][0] = 1;   choice[2][1] = 0;
            choice[3][0] = -1;  choice[3][1] = -1;
            break;
        case 16:
            memcpy(name,"TIGER", 6 * sizeof(char));
            choice[0][0] = 0;   choice[0][1] = 2;
            choice[1][0] = 0;   choice[1][1] = -1;
    }
}


oni_card::oni_card() {
    id = -1;
    memcpy(name,"unassigned", 11 * sizeof(char));
}

oni_card::oni_card(int card_id) : id (card_id) {
    this->set_Cardvalues();
}
