#ifndef CARD_H
#define CARD_H

#include <QGraphicsPixmapItem>
#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>

class Card : public QObject, public QGraphicsPixmapItem {

private:
    // variables
    QString name = "";
    int choice[4][2] = {};
    int owner = 0;          // 0 = neutral, 1 = player white, 2 = player black

public:
    // variables
    int id;

    // constructors
    Card(QGraphicsItem *parent = NULL);

    // getters
    inline QString getName() { return name; }
    inline int getOwner() { return owner; }
    inline int getXFromChoice(const int number) { return choice[number][0]; }
    inline int getYFromChoice(const int number) { return choice[number][1]; }

    // setters
    inline void setName(int newName) { name = newName; }
    inline void setOwner(int newOwner) { owner = newOwner; }

    //functions
    void setCardValues(int cardID, double slotWidth, double slotHeight, double border_x, double border_y);

    // operators
    inline bool operator==(const Card *compare) { return (name == compare->name); }
    inline bool operator!=(const Card *compare) { return (name != compare->name); }
};

#endif // CARD_H
