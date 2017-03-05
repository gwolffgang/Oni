#include "cardslot.h"
#include "oni.h"
#include <QGraphicsRectItem>

extern Oni *game;

CardSlot::CardSlot(double size, QGraphicsItem *parent) {
    // presettings
    owner = 0;

    // scaling the card to DIN A8
    double scale_y = size, scale_x = scale_y/74*52;

    //create a slot to put to the scene
    QGraphicsRectItem *rect = new QGraphicsRectItem;
    setRect(0, 0, scale_x, scale_y);

    //colorize field
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    setBrush(brush);

    //allow responding to hover events
    setAcceptHoverEvents(true);
}

Card *CardSlot::getCard(int player, int number) {
    if (player == 0) return card_neutral;
    else if (player == 1 && number == 1) return card_p1_c1;
    else if (player == 1 && number == 2) return card_p1_c2;
    else if (player == 2 && number == 1) return card_p2_c1;
    else if (player == 2 && number == 2) return card_p2_c2;
    return NULL;
}

void CardSlot::setCard(Card *newCard, int player, int number) {
    if (player == 0) card_neutral = newCard;
    else if (player == 1 && number == 1) card_p1_c1 = newCard;
    else if (player == 1 && number == 2) card_p1_c2 = newCard;
    else if (player == 2 && number == 1) card_p2_c1 = newCard;
    else if (player == 2 && number == 2) card_p2_c2 = newCard;
}

void CardSlot::setOwner(int newOwner) {
    if (newOwner < 3 && newOwner > -1)
        owner = newOwner;
}

void CardSlot::addCard(int id, double slotWidth, double slotHeight) {
    Card *card = new Card(this);
    card->setCardValues(id, slotWidth, slotHeight);
}

void CardSlot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

void CardSlot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if ((game->getFirstPlayersTurn() && this->getOwner() == 1) || (!game->getFirstPlayersTurn() && this->getOwner() == 2)) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::darkGreen);
        setBrush(brush);
        setCursor(Qt::PointingHandCursor);
    }
}

void CardSlot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
}
