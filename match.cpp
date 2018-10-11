#include "match.h"
#include "oni.h"

extern Oni *game;

Match::Match() :
    playerNameBlue("Blue"), playerNameRed("Red"), event(""), city(""), date(QDate::currentDate()),
    turns(new QList<QString>), round(0.0), result(0), openDatabaseGameNumber(-1), board(new QList<QList<Field*>>),
    pieces(new QList<Piece*>), capturedBlue(new QList<Piece*>), capturedRed(new QList<Piece*>),
    slotsGrid(new QList<QList<CardSlot*>>), cards(new QList<Card*>), pickedUpPiece(nullptr), fieldOfOrigin(nullptr), chosenField(nullptr) {

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

void Match::capturePiece() {
    Piece *target = chosenField->identifyPiece();
    if (target->getOwner() == 1)
        capturedRed->append(target);
    else
        capturedBlue->append(target);
    pieces->removeAll(target);
    game->getWindow()->getScene()->removeItem(target);
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

    // switch used card and neutral card
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

void Match::movePiece() {
    MainWindow *window = game->getWindow();
    if (game->getCardChoiceActive())
        game->setCardChoiceActive(false);
    else {
        exchangeCards(chosenField->brush().color());
    }
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
                game->winGame(-1);
                window->notateVictory("0-1");
            }
            else {
                if ((capturedBlue->size() > 0 && capturedBlue->last()->getType() == 'm') ||
                    (board->at(4).at(2)->getPieceType() == 'M')) {
                    game->winGame(1);
                    window->notateVictory("1-0");
                }
            }
        }
        if (window->getFlipEveryMove())
            QTimer::singleShot( 1, window, SLOT(on_actionFlipOnce_triggered()) );
    }
    QTimer::singleShot( 1, window, SLOT(refreshWindow()) );
}

void Match::switchCards(CardSlot *usedCardSlot) {
    Card *temporary = usedCardSlot->getCard();
    usedCardSlot->setCard(slotsGrid->at(0).at(0)->getCard());
    slotsGrid->at(0).at(0)->setCard(temporary);
}

void Match::unmarkAllFields() {
    QBrush brush(Qt::NoBrush);
    for (int k = 0; k < board->size(); k++) {
        for (int l = 0; l < board->at(k).size(); l++) {
            board->at(k).at(l)->setBrush(brush);
            board->at(k).at(l)->setCursor(Qt::ArrowCursor);
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
