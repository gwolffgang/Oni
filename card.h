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
    QString name;
    int choice[4][2], id;

public:
    // constructors
    Card(QGraphicsItem *parent = NULL);

    // getters
    inline QString getName() { return name; }
    inline int getXFromChoice(const int number) { return choice[number][0]; }
    inline int getYFromChoice(const int number) { return choice[number][1]; }

    // setters
    inline void setName(QString newName) { if (name != "") name = newName; }

    //functions
    void setCardValues(int cardID, double slotWidth, double slotHeight);
};

#endif // CARD_H
