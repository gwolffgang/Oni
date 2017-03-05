#include "field.h"
#include "oni.h"
#include <QGraphicsRectItem>

extern Oni *game;

Field::Field(double size, QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    // presettings
    row = -1;
    col = -1;
    piecetype = ' ';

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
    if (!game->getPickedUpPiece()) {
        // identify piece from pieces list
        int piece_number = identifiedPiece();
        if (piece_number != -1)
            if ((game->getFirstPlayersTurn() && (game->getPieces()->at(piece_number)->getType() == 'M' ||
                                                game->getPieces()->at(piece_number)->getType() == 'S')) ||
                (!game->getFirstPlayersTurn() &&(game->getPieces()->at(piece_number)->getType() == 'm' ||
                                                game->getPieces()->at(piece_number)->getType() == 's'))) {
                game->setPickedUpPiece(game->getPieces()->at(piece_number));
                game->getPickedUpPiece()->setPieceValues(game->getPieces()->at(piece_number)->getType(), this->rect().height());
                game->getPickedUpPiece()->setPos(this->pos());
                game->getScene()->removeItem(game->getPieces()->at(piece_number));
                game->getPieces()->removeAll(game->getPieces()->at(piece_number));
                game->getScene()->addItem(game->getPickedUpPiece());
            }
    }
}

void Field::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if ((game->getFirstPlayersTurn() && (this->getPiecetype() == 'M' || this->getPiecetype() == 'S')) ||
        (!game->getFirstPlayersTurn() && (this->getPiecetype() == 'm' || this->getPiecetype() == 's'))) {
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

void Field::addPiece(char type, double fieldSize) {
    Piece *piece = new Piece(this);
    piece->setPieceValues(type, fieldSize);
    this->setPiecetype(piece->getType());
}

int Field::identifiedPiece() {
    for (int i = 0; i < game->getPieces()->size(); i++)
        if (game->getPieces()->at(i)->getRow() == this->getRow() && game->getPieces()->at(i)->getCol() == this->getCol())
            return i;
    return -1;
}
