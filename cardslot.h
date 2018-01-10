#ifndef CARDSLOT_H
#define CARDSLOT_H

#include <QMouseEvent>
#include <QObject>

#include "card.h"

class CardSlot : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    //constructors
    CardSlot(QGraphicsItem *parent = NULL);

    // getter
    inline int getOwner() { return owner; }
    inline Card *getCard() { return card; }

    // setter
    inline void setOwner(int newOwner) { if (newOwner < 3 && newOwner > -1) owner = newOwner; }
    inline void setCard(Card *newCard) { card = newCard; }

    // methods
    void assignCard(int player, int number);
    void colorizePlayersSlots(int player, int number);

protected:
    //events
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    //variables
    Card *card;
    int owner;          // 0 = neutral, 1 = player white, 2 = player black
};

#endif // CARDSLOT_H
