#ifndef CARDSLOT_H
#define CARDSLOT_H

#include <QMouseEvent>
#include <QObject>
#include <QGraphicsRectItem>

#include "card.h"

class CardSlot : public QObject, public QGraphicsRectItem {
    Q_OBJECT

private:
    //variables
    Card *card;
    int owner;          // 0 = neutral, 1 = player white, 2 = player black

public:
    //constructors
    CardSlot(double size, QGraphicsItem *parent = NULL);

    // getter
    inline int getOwner() { return owner; }

    // setter
    void setOwner(int newOwner);

    //functions
    void addCard(int id, double slotWidth, double slotHeight, int player);

    //events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // CARDSLOT_H
