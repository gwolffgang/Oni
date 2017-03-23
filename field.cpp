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

    //create a field to put to the scene
    float size = game->getWindow()->getFieldSize();
    QGraphicsRectItem *rect;
    rect = new QGraphicsRectItem;
    setRect(0, 0, size, size);

    //allow responding to hover events
    setAcceptHoverEvents(true);
}

void Field::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if ((game->getFirstPlayersTurn() && (this->getPieceType() == 'M' || this->getPieceType() == 'S')) ||
        (!game->getFirstPlayersTurn() && (this->getPieceType() == 'm' || this->getPieceType() == 's'))) {
        if (piece != game->getPickedUpPiece()) {
            QBrush brush(game->getWindow()->colorHovered, Qt::Dense4Pattern);
            setBrush(brush);
        }
        setCursor(Qt::PointingHandCursor);
    }
    event->ignore();
}

void Field::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    if (this->brush().color() == game->getWindow()->colorHovered) {
        QBrush brush(Qt::NoBrush);
        setBrush(brush);
        setCursor(Qt::ArrowCursor);
        event->ignore();
    }
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
    event->ignore();
}

void Field::captureOrChangePiece(Piece *target) {
    // capture or change piece
    if ((!game->getFirstPlayersTurn() && (target->getType() == 'M' || target->getType() == 'S')) ||
        (game->getFirstPlayersTurn() && (target->getType() == 'm' || target->getType() == 's'))) {
        // remove captured piece
        game->getWindow()->getScene()->removeItem(target);
        game->getCapturedPieces()->append(target);
        game->getPieces()->removeAll(target);

        // drop piece
        dropPiece();
    } else {
        unmarkAllFields();
        // put back picked up piece
        QBrush brush(game->getWindow()->colorHovered, Qt::Dense4Pattern);
        setBrush(brush);

        if (target != game->getPickedUpPiece()) pickUpPiece(target);
        else {
            game->setPickedUpPiece(NULL);
            game->setFieldOfOrigin(NULL);
        }
    }
}

void Field::dropPiece() {
    // drop piece
    linkPiece(game->getPickedUpPiece());
    game->getFieldOfOrigin()->setPieceType(' ');
    game->changePlayersTurn();
    game->getWindow()->saveTurnInNotation();
    //game->exchangeCards(usedCard, game->getCards()[game->identifyCards(0).at(0)]);

    // cleaning up
    unmarkAllFields();
    game->setFieldOfOrigin(NULL);
    game->setPickedUpPiece(NULL);

    QTimer::singleShot( 1, game->getWindow(), SLOT(refreshWindow()) );
}

int Field::identifyPiece() {
    // identify piece
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

        // unmark all fields
        unmarkAllFields();

        // mark field as selected
        QBrush brush(game->getWindow()->colorSelected, Qt::Dense4Pattern);
        setBrush(brush);

        // determine possible fields to move to
        QList<QList<Field*>> fields;

        // setup variables
        int player = game->getFieldOfOrigin()->getPiece()->getOwner();
        int factor = 1;
        char master = 'M', scolar = 'S';
        if (player == 2) {
            factor = -1;
            master = 'm';
            scolar = 's';
        }
        int fieldCol = this->getCol();
        int fieldRow = this->getRow();

        // identify cards of player
        QList<Card*> cards = game->identifyCards(player);

        // helper list for every cards choices
        QList<Field*> cardFieldList;
        for (int i = 0; i < cards.size(); i++) {
            cardFieldList.clear();
            for (int choice = 0; choice < 4; choice++) {
                int choiceCol = cards.at(i)->getColFromChoice(choice);
                int choiceRow = cards.at(i)->getRowFromChoice(choice);
                if (choiceCol != 0 || choiceRow != 0) {
                    int newFieldCol = fieldCol + choiceCol * factor;
                    int newFieldRow = fieldRow + choiceRow * factor;
                    if ((newFieldCol < 5 && newFieldCol > -1) && (newFieldRow < 5 && newFieldRow > -1)) {
                        char newFieldPieceType = game->getBoard()->at(newFieldRow).at(newFieldCol)->getPieceType();
                        if (newFieldPieceType != master && newFieldPieceType != scolar)
                            cardFieldList.append(game->getBoard()->at(newFieldRow).at(newFieldCol));
                    }
                }
            }
            fields.append(cardFieldList);
        }

        // Colorize fields
        QList<Field*> doubleAccessFields;
        foreach (Field *fieldCard1, fields.at(0)) {
            brush = QBrush(game->getWindow()->colorChooseableCard1, Qt::Dense4Pattern);
            fieldCard1->setBrush(brush);
            foreach (Field *fieldCard2, fields.at(1)) {
                if (fieldCard1 == fieldCard2) doubleAccessFields.append(fieldCard1);
                else {
                    brush = QBrush(game->getWindow()->colorChooseableCard2, Qt::Dense4Pattern);
                    fieldCard2->setBrush(brush);
                }
            }
        }
        foreach (Field *fieldDouble, doubleAccessFields) {
            brush = QBrush(game->getWindow()->colorChooseableBoth, Qt::Dense4Pattern);
            fieldDouble->setBrush(brush);
        }
    }
}

void Field::unmarkAllFields() {
    QBrush brush(Qt::NoBrush);
    for (int k = 0; k < game->getBoard()->size(); k++) {
        for (int l = 0; l < game->getBoard()->at(k).size(); l++) {
            game->getBoard()->at(k).at(l)->setBrush(brush);
        }
    }
}

