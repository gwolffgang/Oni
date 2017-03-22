#include "oni.h"

extern Oni *game;

Oni::Oni() {
    // board settings
    board = new QList<QList<Field*>>;
    rows = 5;
    cols = 5;
    flipedBoard = false;

    // card settings
    slotsGrid = new QList<QList<CardSlot*>>;
    cardsPerPlayer = 2;
    neutralCardsPerPlayer = 1;

    // window settings
    window = new MainWindow;
    window->show();

    // game settings
    gameResult = 0;
    firstPlayersTurn = true;
    pickedUpPiece = NULL;
    fieldOfOrigin = NULL;
    openGameFileName = "";

    // initiate lists
    pieces = new QList<Piece*>;
    capturedPieces = new QList<Piece*>;
    cards = new QList<Card*>;
}

void Oni::exchangeCards(Card *card1, Card *card2) {
    Card *temporary = new Card;
    temporary->setOwner(card1->getOwner());
    card1->setOwner(card2->getOwner());
    card2->setOwner(temporary->getOwner());
    delete temporary;
}

QList<int> Oni::identifyCards(int owner) {
    QList<int> list;
    for (int i = 0; i < cards->count(); i++)
        if (cards->at(i)->getOwner() == owner) list.append(i);
    return list;
}
