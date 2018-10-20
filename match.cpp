#include "match.h"
#include "oni.h"

extern Oni *game;

Match::Match() :
    playerNameBlue("Blue"), playerNameRed("Red"), event(""), city(""), date(QDate::currentDate()),
    turns(new QList<QString>), round(0.0), result(0), openDatabaseGameNumber(-1), board(new QList<QList<Field*>>),
    pieces(new QList<Piece*>), capturedBlue(new QList<Piece*>), capturedRed(new QList<Piece*>),
    slotsGrid(new QList<QList<CardSlot*>>), cards(new QList<Card*>), pickedUpPiece(nullptr), fieldOfOrigin(nullptr), chosenField(nullptr) {

}

QList<QList<Field*>> Match::getDestinationFields() {
    QList<QList<Field*>> list;
    QList<Card*> cardsList = identifyCards(pickedUpPiece->getOwner());
    QList<Field*> cardFieldsList;
    int factor = 1;
    char master = 'M', scolar = 'S';
    if (pickedUpPiece->getOwner() == 2) {
        factor = -1;
        master = 'm';
        scolar = 's';
    }
    for (int i = 0; i < cardsList.size(); i++) {
        cardFieldsList.clear();
        for (int choice = 0; choice < 4; choice++) {
            int choiceCol = cardsList.at(i)->getColFromChoice(choice);
            int choiceRow = cardsList.at(i)->getRowFromChoice(choice);
            if (choiceCol != 0 || choiceRow != 0) {
                int newFieldCol = fieldOfOrigin->getCol() + choiceCol * factor;
                int newFieldRow = fieldOfOrigin->getRow() + choiceRow * factor;
                if ((newFieldCol < 5 && newFieldCol > -1) && (newFieldRow < 5 && newFieldRow > -1)) {
                    char newFieldPieceType = board->at(newFieldRow).at(newFieldCol)->getPieceType();
                    if (newFieldPieceType != master && newFieldPieceType != scolar)
                        cardFieldsList.append(board->at(newFieldRow).at(newFieldCol));
                }
            }
        }
        list.append(cardFieldsList);
    }
    return list;
}

void Match::setResult(QString newResult) {
    if (newResult == "1-0")
        result = 1;
    else {
        if (newResult == "0-1")
            result = -1;
        else
            result = 0;
    }
}

void Match::setWinner(int winner) {
    result = winner;
    QString victor = playerNameBlue;
    if (result == 1)
        victor = playerNameRed;
    QMessageBox::StandardButton reply = QMessageBox::information(nullptr, "VICTORY!", victor + " has won the game. Congratulations!", QMessageBox::Ok, QMessageBox::Save);
    if (reply == QMessageBox::Save)
        QTimer::singleShot(1, game->getWindow(), SLOT(on_actionSave_triggered()) );
}

void Match::capturePiece() {
    Piece *target = chosenField->identifyPiece();
    if (target->getOwner() == 1)
        capturedRed->append(target);
    else
        capturedBlue->append(target);
    pieces->removeAll(target);
    game->getWindow()->getScene()->removeItem(target);
}

void Match::dropPiece() {
    unmarkAllFields();
    QBrush brush(game->getWindow()->colorHovered, Qt::Dense4Pattern);
    chosenField->setBrush(brush);
    pickedUpPiece = nullptr;
    fieldOfOrigin = nullptr;
}

void Match::exchangeCards(QColor slotColor) {
    CardSlot *usedCardSlot = nullptr;
    int player = fieldOfOrigin->getPiece()->getOwner();
    if (slotColor == game->getWindow()->colorChooseableCard1)
        usedCardSlot = slotsGrid->at(player).at(0);
    else
        if (slotColor == game->getWindow()->colorChooseableCard2)
            usedCardSlot = slotsGrid->at(player).at(1);
        else {
            game->setCardChoiceActive(true);
         }
    if (!game->getCardChoiceActive())
        switchCards(usedCardSlot);
}

QList<Card *> Match::identifyCards(int owner) {
    QList<Card*> list;
    int slotsSize = slotsGrid->at(owner).size();
    for (int k = 0; k < slotsSize; k++) {
        list.append(slotsGrid->at(owner).at(k)->getCard());
    }
    return list;
}

void Match::makeMove() {
    if (chosenField->identifyPiece())
        capturePiece();
    movePiece();
}

void Match::markDestinationFields(QList<QList<Field*>> fields) {
    MainWindow *window = game->getWindow();
    foreach (Field *fieldCard1, fields.at(0)) {
        fieldCard1->setBrush(QBrush(window->colorChooseableCard1, Qt::Dense4Pattern));
    }
    foreach (Field *fieldCard2, fields.at(1)) {
        foreach (Field *fieldCard1, fields.at(0)) {
            if (fieldCard1 != fieldCard2)
                fieldCard2->setBrush(QBrush(window->colorChooseableCard2, Qt::Dense4Pattern));
            else
                fieldCard2->setBrush(QBrush(window->colorChooseableBoth, Qt::Dense4Pattern));
        }
    }
}

void Match::movePiece() {
    MainWindow *window = game->getWindow();
    if (game->getCardChoiceActive())
        game->setCardChoiceActive(false);
    else
        exchangeCards(chosenField->brush().color());
    if (!game->getCardChoiceActive()) {
        if (turns->size() > (game->getCurrentDisplayedMove()+1)) {
            QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Change Move", "Do you really want to enter this new move?<br>The old move and all following ones will be deleted.");
            if (reply == QMessageBox::No) {
                unmarkAllFields();
                game->setCardChoiceActive(false);
                pickedUpPiece = nullptr;
                fieldOfOrigin = nullptr;
                window->prepareGame();
                return;
            }
        }
        chosenField->linkPiece(pickedUpPiece);
        window->saveTurnInNotation();
        fieldOfOrigin->setPieceType(' ');
        QGraphicsScene *scene = window->getScene();
        if (game->getFirstPlayersTurn()) {
            scene->addItem(window->getTurnRed());
            scene->removeItem(window->getTurnBlue());
        }
        else {
            scene->addItem(window->getTurnBlue());
            scene->removeItem(window->getTurnRed());
        }
        unmarkAllFields();
        fieldOfOrigin = nullptr;
        pickedUpPiece = nullptr;
        if (result == 0) {
            if ((capturedRed->size() > 0 && capturedRed->last()->getType() == 'M') ||
                (board->at(0).at(2)->getPieceType() == 'm')) {
                setWinner(-1);
                window->notateVictory("0-1");
            }
            else {
                if ((capturedBlue->size() > 0 && capturedBlue->last()->getType() == 'm') ||
                    (board->at(4).at(2)->getPieceType() == 'M')) {
                    setWinner(1);
                    window->notateVictory("1-0");
                }
            }
        }
        if (window->getFlipEveryMove())
            QTimer::singleShot( 1, window, SLOT(on_actionFlipOnce_triggered()) );
    }
    QTimer::singleShot( 1, window, SLOT(refreshWindow()) );
}

void Match::pickUpPiece() {
    pickedUpPiece = fieldOfOrigin->getPiece();
    fieldOfOrigin->setBrush(QBrush(game->getWindow()->colorSelected, Qt::Dense4Pattern));
    markDestinationFields(getDestinationFields());
}

void Match::switchCards(CardSlot *usedCardSlot) {
    Card *temporary = usedCardSlot->getCard();
    usedCardSlot->setCard(slotsGrid->at(0).at(0)->getCard());
    slotsGrid->at(0).at(0)->setCard(temporary);
}

void Match::unmarkAllFields() {
    for (int column = 0; column < board->size(); column++) {
        for (int row = 0; row < board->at(column).size(); row++) {
            board->at(column).at(row)->setBrush(QBrush(Qt::NoBrush));
            board->at(column).at(row)->setCursor(Qt::ArrowCursor);
        }
    }
}

bool Match::operator==(const Match &other) const {
    if (playerNameBlue != other.playerNameBlue || playerNameRed != other.playerNameRed ||
        turns->size() != other.turns->size() || result != other.result)
        return false;
    for (int i = 0; i < turns->size(); i++)
        if (turns->at(i) != other.turns->at(i))
            return false;
    return true;
}
