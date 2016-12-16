//  card.hpp
//  Oni
//  Copyright © 2016 Georg Wolffgang. All rights reserved.

#ifndef card_hpp
#define card_hpp

#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

class oni_card {
private:
    char name[9];
    int id;
    int choice[4][2] = {};

    inline int randomizer(const int max) {
        static bool seeded = false;
        if(!seeded) {
            srand(unsigned(time(NULL)));
            seeded = true;
        }
        return(rand() % max +1);
    }
    
    void set_Cardvalues();
    
public:
    oni_card();
    oni_card(int card_id);
    
    inline int get_x_from_choice(const int number) { return choice[number][0]; }
    inline int get_y_from_choice(const int number) { return choice[number][1]; }
    
    inline oni_card& operator=(const oni_card& source) { id = source.id; set_Cardvalues(); return *this; }
    inline oni_card& operator=(const int& source) { id = source; set_Cardvalues(); return *this; }
    inline bool operator==(const oni_card& compare) { return (name == compare.name); }
    inline bool operator!=(const oni_card& compare) { return (name != compare.name); }
    friend ostream& operator<<(ostream& output, const oni_card& card) { output << card.name; return output; }
    friend istream& operator>>(istream& input, oni_card& card) { input >> card.id; return input; }

};

#endif /* card_hpp */
