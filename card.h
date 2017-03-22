#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <QGraphicsPixmapItem>
#include <string.h>
#include <time.h>
#include <stdlib.h>

class Card : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    // constructors
    Card(QGraphicsItem *parent = NULL);

    // getters
    inline QString getName() { return name; }
    inline int getOwner() { return owner; }
    inline int getXFromChoice(const int number) { return choice[number][0]; }
    inline int getYFromChoice(const int number) { return choice[number][1]; }

    // setters
    inline void setName(QString newName) { if (name != "") name = newName; }
    inline void setOwner(int newOwner) { if (newOwner > -1 && newOwner < 3) owner = newOwner; }

    //functions
    void setCardValues(int cardID, double slotWidth, double slotHeight);

private:
    // variables
    QString name;
    int owner;
    int choice[4][2], id;
};

#endif // CARD_H
