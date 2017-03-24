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
    neutralCardsPerGame = 1;

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

QList<Card*> Oni::identifyCards(int owner) {
    QList<Card*> list;
    for (int i = 0; i < cards->count(); i++)
        if (cards->at(i)->getOwner() == owner) list.append(cards->at(i));
    return list;
}

void Oni::switchCards(Card *usedCard) {
    Card temporary;
    Card *neutral = NULL;
    // get neutral card
    int i = 0;
    do {
        neutral = game->getCards()->at(i);
        i++;
    } while(neutral->getOwner() > 0);
    // switch cards
    temporary.setOwner(usedCard->getOwner());
    usedCard->setOwner(neutral->getOwner());
    neutral->setOwner(temporary.getOwner());
}
