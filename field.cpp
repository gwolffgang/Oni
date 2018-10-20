#include <QApplication>
#include <QPointF>
#include <QPainter>
#include <QTimer>

#include "field.h"
#include "oni.h"

extern Oni *game;

Field::Field(QGraphicsItem *parent) : QGraphicsRectItem(parent), col(-1), row(-1), pieceType(' ') {
    double size = game->getWindow()->getFieldSize();
    QGraphicsRectItem *rect;
    rect = new QGraphicsRectItem;
    setRect(0, 0, size, size);
    setAcceptHoverEvents(true);
}

Piece *Field::identifyPiece() {
    QList<Piece*> *pieces = game->getMatch()->getPieces();
    for (int i = 0; i < pieces->size(); i++)
        if (pieces->at(i)->getRow() == row && pieces->at(i)->getCol() == col)
            return pieces->at(i);
    return nullptr;
}

void Field::linkPiece(Piece *linkedPiece) {
    piece = linkedPiece;
    setPieceType(piece->getType());
    piece->setParentItem(this);
    piece->setCol(col);
    piece->setRow(row);
}

void Field::hoverEnterEvent(QGraphicsSceneHoverEvent *) {
    if (!game->getCardChoiceActive() && game->getMatch()->getResult() == 0) {
        if ((game->getFirstPlayersTurn() && (pieceType == 'M' || pieceType == 'S')) ||
            (!game->getFirstPlayersTurn() && (pieceType == 'm' || pieceType == 's'))) {
            if (piece != game->getMatch()->getPickedUpPiece()) {
                QBrush brush(game->getWindow()->colorHovered, Qt::Dense4Pattern);
                setBrush(brush);
            }
        }
        if (brush().style() != Qt::NoBrush)
            setCursor(Qt::PointingHandCursor);
    }
}

void Field::hoverLeaveEvent(QGraphicsSceneHoverEvent *) {
    if (brush().color() == game->getWindow()->colorHovered && !game->getCardChoiceActive()) {
        QBrush brush(Qt::NoBrush);
        setBrush(brush);
        setCursor(Qt::ArrowCursor);
    }
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent *) {
    Match *match = game->getMatch();
    match->setChosenField(this);
    if (brush().style() != Qt::NoBrush && !game->getCardChoiceActive() && match->getResult() == 0) {
        Piece *piece = identifyPiece();
        if (piece) {
            if (match->getPickedUpPiece()) {
                if (piece->getOwner() == match->getPickedUpPiece()->getOwner()) {
                    if (piece != match->getPickedUpPiece()) {
                        match->dropPiece();
                        match->setFieldOfOrigin(this);
                        match->pickUpPiece();
                    }
                    else
                        match->dropPiece();
                }
                else
                    match->makeMove();
            }
            else {
                match->setFieldOfOrigin(this);
                match->pickUpPiece();
            }
        }
        else
            match->makeMove();
    }
}
