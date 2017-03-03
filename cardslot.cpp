#include "cardslot.h"
#include "oni.h"
#include <QGraphicsRectItem>

extern oni *game;

CardSlot::CardSlot(double size, QGraphicsItem *parent) {
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

void CardSlot::addCard(int id, double slotWidth, double slotHeight, double border_x, double border_y) {
    Card *card = new Card(this);
    card->setCardValues(id, slotWidth, slotHeight, border_x, border_y);
}

void CardSlot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

void CardSlot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::darkGreen);
    setBrush(brush);
    setCursor(Qt::OpenHandCursor);
}

void CardSlot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
}
