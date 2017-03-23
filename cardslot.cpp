#include "cardslot.h"
#include "oni.h"

extern Oni *game;

CardSlot::CardSlot(double size, QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    // presettings
    owner = 0;

    //create a slot to put to the scene
    QGraphicsRectItem *rect;
    rect = new QGraphicsRectItem;
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

void CardSlot::drawCard(int player, int number) {
    // determine correct card
    QList<int> list;
    for (int i = 0; i < game->getCards()->size(); i++)
         if (game->getCards()->at(i)->getOwner() == player) list.append(i);
    card = game->getCards()->at(list.at(number));
    card->setParentItem(this);
    card->drawCard();
    // check if card has to be flipped
    if (!game->getFlipBoard()) {
        if ((game->getFirstPlayersTurn() && player == 2) |
            (!game->getFirstPlayersTurn() && (player == 0 || player == 2))) {
            card->setTransformOriginPoint(card->boundingRect().width() / 2, card->boundingRect().height() / 2);
            card->setRotation(180);
        }
    } else {
        if ((!game->getFirstPlayersTurn() && player == 1) |
            (game->getFirstPlayersTurn() && (player == 0 || player == 1))) {
            card->setTransformOriginPoint(card->boundingRect().width() / 2, card->boundingRect().height() / 2);
            card->setRotation(180);
        }
    }
}

void CardSlot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    event->ignore();
}

void CardSlot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if ((game->getFirstPlayersTurn() && this->getOwner() == 1) || (!game->getFirstPlayersTurn() && this->getOwner() == 2)) {
        QBrush brush(Qt::darkGray, Qt::SolidPattern);
        setBrush(brush);
        setCursor(Qt::PointingHandCursor);
    }
    event->ignore();
}

void CardSlot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush(Qt::gray, Qt::SolidPattern);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
    event->ignore();
}
