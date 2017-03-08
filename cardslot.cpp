#include "cardslot.h"
#include "oni.h"
#include <QGraphicsRectItem>

extern Oni *game;

CardSlot::CardSlot(double size, QGraphicsItem *parent) {
    // presettings
    owner = 0;

    double scale_y = size, scale_x = size;

    //create a slot to put to the scene
    QGraphicsRectItem *rect = new QGraphicsRectItem;
    setRect(0, 0, scale_x, scale_y);

    //colorize field
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::gray);
    setBrush(brush);

    //allow responding to hover events
    setAcceptHoverEvents(true);
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

}

void CardSlot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if ((game->getFirstPlayersTurn() && this->getOwner() == 1) || (!game->getFirstPlayersTurn() && this->getOwner() == 2)) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::darkGray);
        setBrush(brush);
        setCursor(Qt::PointingHandCursor);
    }
}

void CardSlot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::gray);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
}
