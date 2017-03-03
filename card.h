#ifndef CARD_H
#define CARD_H

#include <QGraphicsPixmapItem>
#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>

class Card : public QObject, public QGraphicsPixmapItem {

private:

public:
    // variables
    QString name;
    int id;
    int choice[4][2] = {};

    // constructors
    Card(QGraphicsItem *parent = NULL);

    // functions
    void setCardValues(int cardID, double slotWidth, double slotHeight, double border_x, double border_y);
    inline int get_x_from_choice(const int number) { return choice[number][0]; }
    inline int get_y_from_choice(const int number) { return choice[number][1]; }

    // operators
    inline bool operator==(const Card &compare) { return (name == compare.name); }
    inline bool operator!=(const Card &compare) { return (name != compare.name); }
};

#endif // CARD_H
