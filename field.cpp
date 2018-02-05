#include <QApplication>
#include <QPointF>
#include <QPainter>
#include <QTimer>

#include "field.h"
#include "oni.h"

extern Oni *game;

Field::Field(QGraphicsItem *parent) : QGraphicsRectItem(parent), col(-1), row(-1), pieceType(' ') {
    //create a field to put to the scene
    float size = game->getWindow()->getFieldSize();
    QGraphicsRectItem *rect;
    rect = new QGraphicsRectItem;
    setRect(0, 0, size, size);

    //allow responding to hover events
    setAcceptHoverEvents(true);
}

void Field::captureOrChangePiece(Piece *target) {
    // capture or change piece
    int targetOwner = 0;
    switch (target->getType()) {
    case 'S':
    case 'M':
        targetOwner = 1;
        break;
    case 's':
    case 'm':
        targetOwner = 2;
    }
    if ((!game->getFirstPlayersTurn() && targetOwner == 1) || (game->getFirstPlayersTurn() && (targetOwner == 2))) {
        // remove captured piece
        game->getWindow()->getScene()->removeItem(target);
        if (targetOwner == 1) game->getCapturedRed()->append(target);
        else game->getCapturedBlue()->append(target);
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
    if (game->getCardChoiceActive()) game->setCardChoiceActive(false);
    else exchangeCards();

    if (!game->getCardChoiceActive()) {
        if (game->getTurns()->size() > (game->getActuallyDisplayedMove()+1)) {
            QMessageBox::StandardButton reply = QMessageBox::question(NULL, "Change Move", "Do you really want to enter this new move?<br>The old move and all following ones will be deleted.");
            if (reply == QMessageBox::No) {
                unmarkAllFields();
                // put back picked up piece
                game->setCardChoiceActive(false);
                game->setPickedUpPiece(NULL);
                game->setFieldOfOrigin(NULL);
                game->getWindow()->prepareGame();
                return;
            }
        }
        linkPiece(game->getPickedUpPiece());
        game->getWindow()->saveTurnInNotation();
        game->getFieldOfOrigin()->setPieceType(' ');

        // set turnmarker
        QGraphicsScene *scene = game->getWindow()->getScene();
        if (game->getFirstPlayersTurn()) {
            scene->addItem(game->getWindow()->getTurnRed());
            scene->removeItem(game->getWindow()->getTurnBlue());
        }
        else {
            scene->addItem(game->getWindow()->getTurnBlue());
            scene->removeItem(game->getWindow()->getTurnRed());
        }

        // cleaning up
        unmarkAllFields();
        game->setFieldOfOrigin(NULL);
        game->setPickedUpPiece(NULL);

        // winning conditions check
        if (game->getGameResult() == 0) {
            if ((game->getCapturedRed()->size() > 0 && game->getCapturedRed()->last()->getType() == 'M') ||
                (game->getBoard()->at(0).at(2)->getPieceType() == 'm')) {
                game->winGame(-1);
            } else {
                if ((game->getCapturedBlue()->size() > 0 && game->getCapturedBlue()->last()->getType() == 'm') ||
                    (game->getBoard()->at(4).at(2)->getPieceType() == 'M'))
                    game->winGame(1);
            }
        }
        if (game->getGameResult() != 0) {
            if (game->getGameResult() == 1) game->getWindow()->notateVictory("1-0");
            else game->getWindow()->notateVictory("0-1");
        }

        // flip board every move check
        if (game->getWindow()->getFlipEveryMove())
            QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionFlipOnce_triggered()) );
    }
    // force window refresh
    QTimer::singleShot( 1, game->getWindow(), SLOT(refreshWindow()) );
}

void Field::exchangeCards() {
    // determine used cardslot card
    CardSlot *usedCardSlot = NULL;
    int player = game->getFieldOfOrigin()->getPiece()->getOwner();
    if (this->brush().color() == game->getWindow()->colorChooseableCard1)
        usedCardSlot = game->getSlotsGrid()->at(player).at(0);
    else if (this->brush().color() == game->getWindow()->colorChooseableCard2)
            usedCardSlot = game->getSlotsGrid()->at(player).at(1);
         else {
            game->setCardChoiceActive(true);
            game->setChosenField(this);
         }

    // switch used card and neutral card
    if (!game->getCardChoiceActive()) game->switchCards(usedCardSlot);
}

int Field::identifyPiece() {
    // identify piece
    QList<Piece*> *pieces = game->getPieces();
    for (int i = 0; i < pieces->size(); i++)
        if (pieces->at(i)->getRow() == row && pieces->at(i)->getCol() == col)
            return i;
    return -1;
}

void Field::linkPiece(Piece *linkedPiece) {
    // link field to piece
    piece = linkedPiece;
    setPieceType(piece->getType());

    // link piece to field
    piece->setParentItem(this);
    piece->setCol(col);
    piece->setRow(row);
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
        int fieldCol = col;
        int fieldRow = row;

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
        foreach (Field *fieldCard2, fields.at(1)) {
            brush = QBrush(game->getWindow()->colorChooseableCard2, Qt::Dense4Pattern);
            fieldCard2->setBrush(brush);
        }
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
            game->getBoard()->at(k).at(l)->setCursor(Qt::ArrowCursor);
        }
    }
}

void Field::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    if (!game->getCardChoiceActive() && game->getGameResult() == 0) {
        if ((game->getFirstPlayersTurn() && (pieceType == 'M' || pieceType == 'S')) ||
            (!game->getFirstPlayersTurn() && (pieceType == 'm' || pieceType == 's'))) {
            if (piece != game->getPickedUpPiece()) {
                QBrush brush(game->getWindow()->colorHovered, Qt::Dense4Pattern);
                setBrush(brush);
            }
        }
        if (brush().style() != Qt::NoBrush) {
            setCursor(Qt::PointingHandCursor);
        }
    }
    event->ignore();
}

void Field::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    if (brush().color() == game->getWindow()->colorHovered && !game->getCardChoiceActive()) {
        QBrush brush(Qt::NoBrush);
        setBrush(brush);
        setCursor(Qt::ArrowCursor);
        event->ignore();
    }
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // identify possible piece from pieces list
    if (brush().style() != Qt::NoBrush && !game->getCardChoiceActive() && game->getGameResult() == 0) {
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
    event->ignore();
}
