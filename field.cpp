#include "field.h"
#include "oni.h"
#include <QGraphicsRectItem>

extern oni *game;

Field::Field(double size, QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    //create a field to put to the scene
    QGraphicsRectItem *rect = new QGraphicsRectItem;
    setRect(0, 0, size, size);

    //colorize field
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);
    setBrush(brush);

    //allow responding to hover events
    setAcceptHoverEvents(true);
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

void Field::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::gray);
    setBrush(brush);
    setCursor(Qt::OpenHandCursor);
}

void Field::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
}
