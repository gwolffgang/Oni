#ifndef CARDSLOT_H
#define CARDSLOT_H

#include <QMouseEvent>
#include <QObject>

#include "card.h"

class CardSlot : public QObject, public QGraphicsRectItem {
    Q_OBJECT

private:
    //variables
    Card *card;
    int owner;          // 0 = neutral, 1 = player white, 2 = player black

protected:
    //events
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

public:
    //constructors
    CardSlot(double size, QGraphicsItem *parent = NULL);

    // getter
    inline int getOwner() { return owner; }
    inline Card *getCard() { return card; }

    // setter
    void setOwner(int newOwner);
    void setCard(Card *newCard) { card = newCard; }

    //functions
    void assignCard(int player, int number);
    void colorizePlayersSlots(int player, int number);
};

#endif // CARDSLOT_H
