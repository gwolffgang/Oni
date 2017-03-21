#include "cardslot.h"
#include "oni.h"
#include <QGraphicsRectItem>

extern Oni *game;

CardSlot::CardSlot(double size, QGraphicsItem *parent) {
    // presettings
    owner = 0;

    //create a slot to put to the scene
    QGraphicsRectItem *rect = new QGraphicsRectItem;
    setRect(0, 0, size, size);

    //colorize field
    QBrush brush(Qt::gray, Qt::SolidPattern);
    setBrush(brush);

    //allow responding to hover events
    setAcceptHoverEvents(true);
}

void CardSlot::setOwner(int newOwner) {
    if (newOwner < 3 && newOwner > -1)
        owner = newOwner;
}

void CardSlot::addCard(int id, double slotWidth, double slotHeight, int player) {
    Card *card = new Card(this);
    card->setOwner(player);
    card->setCardValues(id, slotWidth, slotHeight);
    if (!game->getFlipBoard()) {
        if (game->getFirstPlayersTurn() && player == 2) {
            card->setTransformOriginPoint(card->boundingRect().height() / 2, card->boundingRect().width() / 2);
            card->setRotation(180);
        }
        if (!game->getFirstPlayersTurn() && (player == 0 || player == 2)) {
            card->setTransformOriginPoint(card->boundingRect().height() / 2, card->boundingRect().width() / 2);
            card->setRotation(180);
        }
    } else {
        if (!game->getFirstPlayersTurn() && player == 1) {
            card->setTransformOriginPoint(card->boundingRect().height() / 2, card->boundingRect().width() / 2);
            card->setRotation(180);
        }
        if (game->getFirstPlayersTurn() && (player == 0 || player == 1)) {
            card->setTransformOriginPoint(card->boundingRect().height() / 2, card->boundingRect().width() / 2);
            card->setRotation(180);
        }
    }
}

void CardSlot::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void CardSlot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if ((game->getFirstPlayersTurn() && this->getOwner() == 1) || (!game->getFirstPlayersTurn() && this->getOwner() == 2)) {
        QBrush brush(Qt::darkGray, Qt::SolidPattern);
        setBrush(brush);
        setCursor(Qt::PointingHandCursor);
    }
}

void CardSlot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush(Qt::gray, Qt::SolidPattern);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
}
