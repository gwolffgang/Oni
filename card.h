#ifndef CARD_H
#define CARD_H

#include <QGraphicsPixmapItem>
#include <QString>
#include <time.h>
#include <stdlib.h>

class Card : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    // constructor
    Card(QGraphicsItem *parent = NULL);

    // operators
    inline bool operator==(Card *card) { if (card->id == id) return true; else return false; }

    // getter
    inline QString getName() { return name; }
    inline int getID() { return id; }
    inline int getColFromChoice(const int number) { return choice[number][0]; }
    inline int getRowFromChoice(const int number) { return choice[number][1]; }

    // methods
    void setCardValues(int cardID);
    void drawCard(int player);
    void drawYinYang(QPainter *painter, int x, int y);

private:
    // variables
    QString name;
    int choice[4][2], id;
    QPixmap card;
};

#endif // CARD_H
