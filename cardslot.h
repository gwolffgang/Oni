#ifndef CARDSLOT_H
#define CARDSLOT_H

#include "card.h"

class CardSlot : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    //constructors
    CardSlot(QGraphicsItem *parent = nullptr);

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
    void mousePressEvent(QGraphicsSceneMouseEvent *) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *) override;

private:
    //variables
    Card *card;
    int owner;          // 0 = neutral, 1 = player white, 2 = player black
};

#endif // CARDSLOT_H
