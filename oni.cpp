#include <QMessageBox>
#include <QTimer>

#include "oni.h"

extern Oni *game;

Oni::Oni() : window(new MainWindow), board(new QList<QList<Field*>>),
    pieces(new QList<Piece*>), capturedBlue(new QList<Piece*>), capturedRed(new QList<Piece*>),
    slotsGrid(new QList<QList<CardSlot*>>), cards(new QList<Card*>), turns(new QList<QString>),
    playerNameRed("Red"), playerNameBlue("Blue"), openGameFileName(""), pickedUpPiece(NULL), fieldOfOrigin(NULL), rows(5), cols(5),
    cardsPerPlayer(2), neutralCardsPerGame(1), actuallyDisplayedMove(0), gameResult(0),
    firstPlayersTurn(true), flippedBoard(false), cardChoiceActive(false) {

    QDir dir("saves");
    if (!dir.exists()) dir.mkpath(".");

    window->show();
}

void Oni::setPlayerNames(QString nameRed, QString nameBlue) {
    if (nameBlue != "") playerNameBlue = nameBlue;
    if (nameRed != "") playerNameRed = nameRed;
}

QList<Card*> Oni::identifyCards(int owner) {
    QList<Card*> list;
    int slotsSize = slotsGrid->at(owner).size();
    for (int k = 0; k < slotsSize; k++) {
        list.append(slotsGrid->at(owner).at(k)->getCard());
    }
    return list;
}

void Oni::switchCards(CardSlot *usedCardSlot) {
    Card *temporary = NULL;
    // switch cards
    temporary = usedCardSlot->getCard();
    usedCardSlot->setCard(game->getSlotsGrid()->at(0).at(0)->getCard());
    game->getSlotsGrid()->at(0).at(0)->setCard(temporary);
}

void Oni::winGame(int winner) {
    gameResult = winner;
    QString victor;
    if (winner == 1) victor = "Red";
    else victor = "Blue";
    QMessageBox::StandardButton reply = QMessageBox::information(NULL, "VICTORY!", victor + " has won the game. Congratulations!", QMessageBox::Ok, QMessageBox::Save);
    if (reply == QMessageBox::Save) QTimer::singleShot( 1, game->getWindow(), SLOT(on_actionSave_triggered()) );
}
