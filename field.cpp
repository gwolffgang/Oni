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

void Field::pickUpPiece(Piece *piece) {
    Match *match = game->getMatch();
    if ((game->getFirstPlayersTurn() && (piece->getType() == 'M' || piece->getType() == 'S')) ||
        (!game->getFirstPlayersTurn() && (piece->getType() == 'm' || piece->getType() == 's'))) {
        match->setFieldOfOrigin(this);
        match->setPickedUpPiece(piece);
        game->getMatch()->unmarkAllFields();
        QBrush brush(game->getWindow()->colorSelected, Qt::Dense4Pattern);
        setBrush(brush);
        QList<QList<Field*>> fields;
        int player = match->getFieldOfOrigin()->getPiece()->getOwner();
        int factor = 1;
        char master = 'M', scolar = 'S';
        if (player == 2) {
            factor = -1;
            master = 'm';
            scolar = 's';
        }
        int fieldCol = col, fieldRow = row;
        QList<Card*> cards = match->identifyCards(player);
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
                        char newFieldPieceType = match->getBoard()->at(newFieldRow).at(newFieldCol)->getPieceType();
                        if (newFieldPieceType != master && newFieldPieceType != scolar)
                            cardFieldList.append(match->getBoard()->at(newFieldRow).at(newFieldCol));
                    }
                }
            }
            fields.append(cardFieldList);
        }

        QList<Field*> doubleAccessFields;
        foreach (Field *fieldCard2, fields.at(1)) {
            brush = QBrush(game->getWindow()->colorChooseableCard2, Qt::Dense4Pattern);
            fieldCard2->setBrush(brush);
        }
        foreach (Field *fieldCard1, fields.at(0)) {
            brush = QBrush(game->getWindow()->colorChooseableCard1, Qt::Dense4Pattern);
            fieldCard1->setBrush(brush);
            foreach (Field *fieldCard2, fields.at(1)) {
                if (fieldCard1 == fieldCard2)
                    doubleAccessFields.append(fieldCard1);
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
        if (match->getPickedUpPiece())
            match->makeMove();
        else
            pickUpPiece(piece);
    }
}
