#include <QMessageBox>
#include <QTimer>

#include "oni.h"

extern Oni *game;

Oni::Oni() {
    // saves folder creation
    QDir dir("saves");
    if (!dir.exists()) dir.mkpath(".");

    // board settings
    board = new QList<QList<Field*>>;
    rows = 5;
    cols = 5;
    flippedBoard = false;

    // card settings
    slotsGrid = new QList<QList<CardSlot*>>;
    cardsPerPlayer = 2;
    neutralCardsPerGame = 1;

    // window settings
    window = new MainWindow;
    window->show();

    // game settings
    gameResult = 0;
    firstPlayersTurn = true;
    cardChoiceActive = false;
    pickedUpPiece = NULL;
    fieldOfOrigin = NULL;
    playerNameBlue = "Blue";
    playerNameRed = "Red";
    openGameFileName = "";
    actuallyDisplayedMove = 0;

    // initiate lists
    pieces = new QList<Piece*>;
    capturedBlue = new QList<Piece*>;
    capturedRed = new QList<Piece*>;
    cards = new QList<Card*>;
    turns = new QList<QString>;
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
