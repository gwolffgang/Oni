#include "match.h"
#include "oni.h"

Match::Match() :
    playerNameBlue("Blue"), playerNameRed("Red"), event(""), city(""), date(QDate::currentDate()),
    turns(new QList<QString>), round(0.0), result(0), openDatabaseGameNumber(-1), board(new QList<QList<Field*>>),
    pieces(new QList<Piece*>), capturedBlue(new QList<Piece*>), capturedRed(new QList<Piece*>),
    slotsGrid(new QList<QList<CardSlot*>>), cards(new QList<Card*>), pickedUpPiece(nullptr), fieldOfOrigin(nullptr) {

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

QList<Card *> Match::identifyCards(int owner) {
    QList<Card*> list;
    int slotsSize = slotsGrid->at(owner).size();
    for (int k = 0; k < slotsSize; k++) {
        list.append(slotsGrid->at(owner).at(k)->getCard());
    }
    return list;
}

void Match::switchCards(CardSlot *usedCardSlot) {
    Card *temporary = usedCardSlot->getCard();
    usedCardSlot->setCard(slotsGrid->at(0).at(0)->getCard());
    slotsGrid->at(0).at(0)->setCard(temporary);
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
