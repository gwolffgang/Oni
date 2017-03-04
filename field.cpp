#include "field.h"
#include "oni.h"
#include <QGraphicsRectItem>

extern Oni *game;

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
    if ((game->move_first_player && (this->getPiece() == 'M' || this->getPiece() == 'S')) ||
        (!game->move_first_player && (this->getPiece() == 'm' || this->getPiece() == 's'))) {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::gray);
        setBrush(brush);
        setCursor(Qt::PointingHandCursor);
    }
}

void Field::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
}

void Field::addPiece(char pieceType, double fieldSize) {
    Piece *piece = new Piece(this);
    piece->setPieceValues(pieceType, fieldSize);
    this->setPiece(pieceType);
}
