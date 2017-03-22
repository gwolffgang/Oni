#ifndef ONI_H
#define ONI_H

#include <QApplication>
#include <QBrush>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QObject>
#include <QScreen>
#include <QTextStream>

#include "card.h"
#include "cardslot.h"
#include "field.h"
#include "mainwindow.h"
#include "piece.h"

class Oni {

public:
    //constructors
    Oni();

    // getters
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline QList<Piece*> *getCapturedPieces() { return capturedPieces; }
    inline QList<Card*> *getCards() { return cards; }
    inline int getCardsPerPlayer () { return cardsPerPlayer; }
    inline int getCols() { return cols; }
    inline Field *getFieldOfOrigin() { return fieldOfOrigin; }
    inline bool getFirstPlayersTurn() { return firstPlayersTurn; }
    inline bool getFlipBoard() { return flipedBoard; }
    inline int getGameResult() { return gameResult; }
    inline QString getOpenGameFileName() { return openGameFileName; }
    inline int getNeutralCardsPerPlayer() { return neutralCardsPerPlayer; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline int getRows() { return rows; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }
    inline MainWindow *getWindow() { return window; }

    // setters
    inline void changePlayersTurn() { firstPlayersTurn = !firstPlayersTurn; }
    inline void flipBoard() { flipedBoard = !flipedBoard; }
    inline void setCols(int newCols) { cols = newCols; }
    inline void setFieldOfOrigin(Field *origin) { fieldOfOrigin = origin; }
    inline void setGameResult(int winner) { gameResult = winner; }
    inline void setOpenGameFileName(QString newString) { openGameFileName = newString; }
    inline void setPickedUpPiece(Piece *newPiece) { pickedUpPiece = newPiece; }
    inline void setRows(int newRows) { rows = newRows; }

    //functions
    void exchangeCards(Card *card1, Card *card2);
    QList<int> identifyCards(int owner);
    void setUpWindowSize(double factor);

private:
    //variables
    MainWindow *window;
    QList<QList<Field*>> *board;
    QList<Piece*> *pieces, *capturedPieces;
    QList<QList<CardSlot*>> *slotsGrid;
    QList<Card*> *cards;
    QString openGameFileName;
    Piece *pickedUpPiece;
    Field *fieldOfOrigin;
    int rows, cols, cardsPerPlayer, neutralCardsPerPlayer;

    int gameResult;        // game_result: 0 = ongoing game; 1 = player 1 has won; -1 = player 2 has won
    bool firstPlayersTurn;  // firstPlayersTurn: true = turn of player 1, false = turn of player 2
    bool flipedBoard;
};

#endif // ONI_H
