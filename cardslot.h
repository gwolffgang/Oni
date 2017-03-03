#ifndef CARDSLOT_H
#define CARDSLOT_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "card.h"

class CardSlot : public QGraphicsRectItem {
private:

public:
    //variables
    Card *card_p1_c1, *card_p1_c2, *card_p2_c1, *card_p2_c2, *card_neutral;

    //constructors
    CardSlot(double size, QGraphicsItem *parent = NULL);

    //functions
    void addCard(int id, double slotWidth, double slotHeight, double border_x, double border_y);

    //events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void clicked() {}
};

#endif // CARDSLOT_H
