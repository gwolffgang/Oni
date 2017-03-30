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
    // assigning owner
    if (newOwner < 3 && newOwner > -1)
        owner = newOwner;
}

void CardSlot::assignCard(int player, int number) {
    // determine correct card
    switch (player) {
        case 0:
            card = game->getCards()->at(0);
            break;
        case 1:
            if (number == 0) card = game->getCards()->at(1);
            else card = game->getCards()->at(2);
            break;
        case 2:
            if (number == 0) card = game->getCards()->at(3);
            else card = game->getCards()->at(4);
    }
}

void CardSlot::colorizePlayersSlots(int player, int number) {
    QBrush brush(Qt::SolidPattern);
    switch (player) {
        case 0:
            brush.setColor(game->getWindow()->colorHovered);
            break;
        case 1:
            if (game->getFirstPlayersTurn()) {
                if (number == 0) brush.setColor(game->getWindow()->colorChooseableCard1);
                else brush.setColor(game->getWindow()->colorChooseableCard2);
            } else brush.setColor(game->getWindow()->colorHovered);
            break;
        case 2:
            if (!game->getFirstPlayersTurn()) {
                if (number == 0) brush.setColor(game->getWindow()->colorChooseableCard1);
                else brush.setColor(game->getWindow()->colorChooseableCard2);
            } else brush.setColor(game->getWindow()->colorHovered);
    }
    setBrush(brush);
}

void CardSlot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (game->getCardChoiceActive()) {
        if ((game->getFirstPlayersTurn() && owner == 1) || (!game->getFirstPlayersTurn() && owner == 2)) {
            game->switchCards(this);
            game->getChosenField()->dropPiece();
        }
    }
    event->ignore();
}

void CardSlot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if (game->getCardChoiceActive()) {
        if ((game->getFirstPlayersTurn() && owner == 1) || (!game->getFirstPlayersTurn() && owner == 2))
            setCursor(Qt::PointingHandCursor);
    event->ignore();
    }
}

void CardSlot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(Qt::ArrowCursor);
    event->ignore();
}
