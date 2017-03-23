#include <QApplication>
#include <QPointF>
#include <QPainter>
#include <QTimer>

#include "field.h"
#include "oni.h"

extern Oni *game;

Field::Field(QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    // presettings
    row = -1;
    col = -1;
    pieceType = ' ';
    color = Qt::transparent;

    //create a field to put to the scene
    float size = game->getWindow()->getFieldSize();
    QGraphicsRectItem *rect = new QGraphicsRectItem;
    setRect(0, 0, size, size);

    //allow responding to hover events
    setAcceptHoverEvents(true);
}

void Field::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if ((game->getFirstPlayersTurn() && (this->getPieceType() == 'M' || this->getPieceType() == 'S')) ||
        (!game->getFirstPlayersTurn() && (this->getPieceType() == 'm' || this->getPieceType() == 's'))) {
        color = game->getWindow()->colorHovered;
        QBrush brush(color, Qt::Dense4Pattern);
        setBrush(brush);
        setCursor(Qt::PointingHandCursor);
    }
}

void Field::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    color = game->getWindow()->colorNone;
    if (game->getFieldOfOrigin() != NULL && (game->getFieldOfOrigin()->getCol() == this->getCol() &&
          game->getFieldOfOrigin()->getRow() == this->getRow())) {
        color = game->getWindow()->colorSelected;
    }
    QBrush brush(color, Qt::Dense4Pattern);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // identify possible piece from pieces list
    int pieceNumber = identifyPiece();
    if (pieceNumber == -1) {
        // no piece on that field
        if (game->getPickedUpPiece() != NULL)
            // try to drop piece
            dropPiece();
    } else {
        // existing piece on that field
        if (game->getPickedUpPiece() != NULL)
            // capture piece
            captureOrChangePiece(game->getPieces()->at(pieceNumber));
        else
            // pick up piece
            pickUpPiece(game->getPieces()->at(pieceNumber));
    }
}

void Field::captureOrChangePiece(Piece *target) {
    if ((!game->getFirstPlayersTurn() && (target->getType() == 'M' || target->getType() == 'S')) ||
        (game->getFirstPlayersTurn() && (target->getType() == 'm' || target->getType() == 's'))) {
        // remove captured piece
        game->getWindow()->getScene()->removeItem(target);
        game->getCapturedPieces()->append(target);
        game->getPieces()->removeAll(target);

        // drop piece
        dropPiece();
    } else {
        // put back picked up piece
        game->getFieldOfOrigin()->setPieceType(game->getPickedUpPiece()->getType());

        color = game->getWindow()->colorNone;
        QBrush brush(color, Qt::Dense4Pattern);
        setBrush(brush);

        // pick up new piece
        pickUpPiece(target);
    }
}

void Field::dropPiece() {
    linkPiece(game->getPickedUpPiece());
    game->changePlayersTurn();
    game->getWindow()->saveTurnInNotation();
    //game->exchangeCards(usedCard, game->getCards()[game->identifyCards(0).at(0)]);

    // cleaning up
    game->setFieldOfOrigin(NULL);
    game->setPickedUpPiece(NULL);

    QTimer::singleShot( 1, game->getWindow(), SLOT(refreshWindow()) );
}

int Field::identifyPiece() {
    for (int i = 0; i < game->getPieces()->size(); i++)
        if (game->getPieces()->at(i)->getRow() == this->getRow() && game->getPieces()->at(i)->getCol() == this->getCol())
            return i;
    return -1;
}

void Field::linkPiece(Piece *linkedPiece) {
    // link field to piece
    piece = linkedPiece;
    setPieceType(piece->getType());

    // link piece to field
    piece->setParentItem(this);
    piece->setCol(this->getCol());
    piece->setRow(this->getRow());
}

void Field::pickUpPiece(Piece *piece) {
    if ((game->getFirstPlayersTurn() && (piece->getType() == 'M' || piece->getType() == 'S')) ||
        (!game->getFirstPlayersTurn() && (piece->getType() == 'm' || piece->getType() == 's'))) {
        // save origin of piece
        game->setFieldOfOrigin(this);

        // pick the piece up
        game->setPickedUpPiece(piece);

        // mark field as pieceless but choosen
        this->setPieceType(' ');
        color = game->getWindow()->colorSelected;
        QBrush brush(color, Qt::Dense4Pattern);
        setBrush(brush);
    }
}
