#include "oni.h"
//#include "table.h"

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

    // initiate pieces lists
    pieces = new QList<Piece*>;
    capturedPieces = new QList<Piece*>;
}

void Oni::setPickedUpPiece(Piece *newPiece) {
    if (newPiece == NULL) {
        delete pickedUpPiece;
        pickedUpPiece = NULL;
    }
    else {
        if (pickedUpPiece) delete pickedUpPiece;
        pickedUpPiece = new Piece;
        pickedUpPiece->setCol(newPiece->getCol());
        pickedUpPiece->setRow(newPiece->getRow());
        pickedUpPiece->setType(newPiece->getType());
        pickedUpPiece->drawPiece();
        QPointF piecePos = game->getBoard()->at(newPiece->getRow()).at(newPiece->getCol())->pos() ;
        pickedUpPiece->setPos(piecePos + QPointF(game->window->getBorderX(), game->window->getBorderY()));
        window->getScene()->addItem(pickedUpPiece);
    }
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
