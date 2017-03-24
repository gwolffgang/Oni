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
#include <QtPrintSupport/QPrinter>

#include "card.h"
#include "cardslot.h"
#include "field.h"
#include "mainwindow.h"
#include "piece.h"

class Oni {

private:
    //variables
    MainWindow *window;
    QList<QList<Field*>> *board;
    QList<Piece*> *pieces, *capturedPieces;
    QList<QList<CardSlot*>> *slotsGrid;
    QList<Card*> *cards;
    QString openGameFileName;
    Piece *pickedUpPiece;
    Field *fieldOfOrigin, *chosenField;
    int rows, cols, cardsPerPlayer, neutralCardsPerGame;

    int gameResult;        // game_result: 0 = ongoing game; 1 = player 1 has won; -1 = player 2 has won
    bool firstPlayersTurn;  // firstPlayersTurn: true = turn of player 1, false = turn of player 2
    bool flippedBoard, cardChoiceActive;

public:
    //constructors
    Oni();

    // getters
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline QList<Piece*> *getCapturedPieces() { return capturedPieces; }
    inline QList<Card*> *getCards() { return cards; }
    inline bool getCardChoiceActive() { return cardChoiceActive; }
    inline int getCardsPerPlayer () { return cardsPerPlayer; }
    inline Field *getChosenField() { return chosenField; }
    inline int getCols() { return cols; }
    inline Field *getFieldOfOrigin() { return fieldOfOrigin; }
    inline bool getFirstPlayersTurn() { return firstPlayersTurn; }
    inline bool getFlippedBoard() { return flippedBoard; }
    inline int getGameResult() { return gameResult; }
    inline QString getOpenGameFileName() { return openGameFileName; }
    inline int getNeutralCardsPerGame() { return neutralCardsPerGame; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline int getRows() { return rows; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }
    inline MainWindow *getWindow() { return window; }

    // setters
    inline void setCardChoiceActive(bool state) { cardChoiceActive = state; }
    inline void setChosenField(Field *target) { chosenField = target; }
    inline void setCols(int newCols) { cols = newCols; }
    inline void setFlippedBoard(bool state) { flippedBoard = state; }
    inline void setFirstPlayersTurn(bool state) { firstPlayersTurn = state; }
    inline void setFieldOfOrigin(Field *origin) { fieldOfOrigin = origin; }
    inline void setGameResult(int winner) { gameResult = winner; }
    inline void setOpenGameFileName(QString newString) { openGameFileName = newString; }
    inline void setPickedUpPiece(Piece *newPiece) { pickedUpPiece = newPiece; }
    inline void setRows(int newRows) { rows = newRows; }

    //functions
    void switchCards(CardSlot *usedCardSlot);
    QList<Card*> identifyCards(int owner);
    void setUpWindowSize(double factor);
};

#endif // ONI_H
