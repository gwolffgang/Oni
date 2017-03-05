#ifndef CARDSLOT_H
#define CARDSLOT_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "card.h"

class CardSlot : public QGraphicsRectItem {
private:
    //variables
    Card *card_p1_c1, *card_p1_c2, *card_p2_c1, *card_p2_c2, *card_neutral;
    int owner;          // 0 = neutral, 1 = player white, 2 = player black

public:
    //constructors
    CardSlot(double size, QGraphicsItem *parent = NULL);

    // getter
    Card *getCard(int player, int number = 1);
    inline int getOwner() { return owner; }

    // setter
    void setCard(Card *newCard, int player, int number = 1);
    void setOwner(int newOwner);

    //functions
    void addCard(int id, double slotWidth, double slotHeight);

    //events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void clicked() {}
};

#endif // CARDSLOT_H
