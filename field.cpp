#include "field.h"
#include "oni.h"
#include <QGraphicsRectItem>
#include <QPointF>
#include <QApplication>

extern Oni *game;

Field::Field(double size, QGraphicsItem *parent) : QGraphicsRectItem(parent) {
    // presettings
    row = -1;
    col = -1;
    pieceType = ' ';

    //create a field to put to the scene
    QGraphicsRectItem *rect = new QGraphicsRectItem;
    setRect(0, 0, size, size);

    //allow responding to hover and dropping events
    setAcceptHoverEvents(true);
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // identify possible piece from pieces list
    int pieceNumber = identifyPiece();
    if (game->getPickedUpPiece() == NULL && pieceNumber != -1) {
        // try to pick piece up
        game->setPieceToReposition(game->getPieces()->at(pieceNumber));
        pickUpPiece(this);
    }
}

void Field::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (game->getPickedUpPiece() != NULL) {
        int pieceNumber = identifyPiece();
        if (pieceNumber == -1)
            // try to drop piece
            dropPiece();
        else
            // try to capture piece
            capturePiece(game->getPieces()->at(pieceNumber));
    }
}

void Field::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if ((game->getFirstPlayersTurn() && (this->getPieceType() == 'M' || this->getPieceType() == 'S')) ||
        (!game->getFirstPlayersTurn() && (this->getPieceType() == 'm' || this->getPieceType() == 's'))) {
        QBrush brush;
        brush.setStyle(Qt::Dense4Pattern);
        brush.setColor(Qt::gray);
        setBrush(brush);
        setCursor(Qt::PointingHandCursor);
    }
}

void Field::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    QBrush brush;
    brush.setStyle(Qt::NoBrush);
    setBrush(brush);
    setCursor(Qt::ArrowCursor);
}

void Field::capturePiece(Piece *target) {
    // remove captured piece
    //game->getCapturedPieces()->append(target);
    //game->getPieces()->removeAll(target);

    // drop piece
    //dropPiece();
}

void Field::dropPiece() {
    this->linkPiece(game->getPieceToReposition());

    // cleaning up
    game->setPieceToReposition(NULL);
    game->getWindow()->getScene()->removeItem(game->getPickedUpPiece());
    game->setPickedUpPiece(NULL);
    game->changePlayersTurn();
    //game->exchangeCards(usedCard, game->getCards()[game->identifyCards(0).at(0)]);
    game->getWindow()->redrawWindow();
}

int Field::identifyPiece() {
    for (int i = 0; i < game->getPieces()->size(); i++)
        if (game->getPieces()->at(i)->getRow() == this->getRow() && game->getPieces()->at(i)->getCol() == this->getCol())
            return i;
    return -1;
}

void Field::linkPiece(Piece *linkedPiece) {
    // link field to piece
    this->piece = linkedPiece;
    this->setPieceType(piece->getType());

    // link piece to field
    piece->setParentItem(this);
    piece->setCol(this->getCol());
    piece->setRow(this->getRow());
}

void Field::pickUpPiece(Field *field) {
    if ((game->getFirstPlayersTurn() && (game->getPieceToReposition()->getType() == 'M' ||
                                         game->getPieceToReposition()->getType() == 'S')) ||
        (!game->getFirstPlayersTurn() && (game->getPieceToReposition()->getType() == 'm' ||
                                         game->getPieceToReposition()->getType() == 's'))) {
        // pick the piece up
        game->setPickedUpPiece(game->getPieceToReposition());

        // save origin of piece
        game->setPieceToReposition(game->getPieceToReposition());

        // remove old piece
        field->setPieceType(' ');
        game->getWindow()->getScene()->removeItem(game->getPieceToReposition());
    }

}
