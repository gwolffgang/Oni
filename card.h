#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <QGraphicsPixmapItem>
#include <string.h>
#include <time.h>
#include <stdlib.h>

class Card : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

private:
    // variables
    QString name;
    int choice[4][2], id;

public:
    // constructors
    Card(QGraphicsItem *parent = NULL);

    // getters
    inline QString getName() { return name; }
    inline int getID() { return id; }
    inline int getColFromChoice(const int number) { return choice[number][0]; }
    inline int getRowFromChoice(const int number) { return choice[number][1]; }

    // setters
    inline void setName(QString newName) { if (name != "") name = newName; }

    //functions
    void setCardValues(int cardID);
    void drawCard(int player);
};

#endif // CARD_H
