#ifndef CARDSLOT_H
#define CARDSLOT_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "card.h"

class CardSlot : public QGraphicsRectItem {
private:
    int owner = 0;          // 0 = neutral, 1 = player white, 2 = player black

public:
    //variables
    Card *card_p1_c1, *card_p1_c2, *card_p2_c1, *card_p2_c2, *card_neutral;

    //constructors
    CardSlot(double size, QGraphicsItem *parent = NULL);

    // getter
    inline int getOwner() { return owner; }

    // setter
    inline void setOwner(int newOwner) { if (newOwner < 3 && newOwner > -1) owner = newOwner; }

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
