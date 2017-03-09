#ifndef ONI_H
#define ONI_H

#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QObject>
#include "card.h"
#include "cardslot.h"
#include "field.h"
#include "mainwindow.h"
#include "piece.h"

class Oni {

public:
    //constructors
    Oni();

    // events
    void mouseMoveEvent(QMouseEvent *event);

    // getters
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline int getBorderX() { return borderX; }
    inline int getBorderY() { return borderY; }
    inline QList<Piece*> *getCapturedPieces() { return capturedPieces; }
    inline QList<Card*> *getCards() { return cards; }
    inline int getCardsPerPlayer () { return cardsPerPlayer; }
    inline int getCols() { return cols; }
    inline double getFieldHeight() { return fieldHeight; }
    inline bool getFirstPlayersTurn() { return firstPlayersTurn; }
    inline bool getFlipBoard() { return flipBoard; }
    inline int getGameResult() { return gameResult; }
    inline int getNeutralCardsPerPlayer() { return neutralCardsPerPlayer; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline Piece *getPieceToReposition() { return pieceToReposition; }
    inline int getRows() { return rows; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }
    inline MainWindow *getWindow() { return window; }
    inline double getWindowHeight() { return windowHeight; }
    inline double getWindowWidth() { return windowWidth; }

    // setters
    inline void changePlayersTurn() { firstPlayersTurn = !firstPlayersTurn; }
    inline void FlipBoard() { flipBoard = !flipBoard; }
    inline void setBorderX(int newX) { borderX = newX; }
    inline void setBorderY(int newY) { borderY = newY; }
    inline void setCols(int newCols) { cols = newCols; }
    inline void setFieldHeight(double newFieldHeight) { fieldHeight = newFieldHeight; }
    inline void setGameResult (int winner) { gameResult = winner; }
    void setPickedUpPiece(Piece *newPiece);
    inline void setPieceToReposition(Piece *origin) { pieceToReposition = origin; }
    inline void setRows(int newRows) { rows = newRows; }

    //functions
    void start();

public slots:
    void loadGame();
    void newGame();
    void saveGame();
    void saveGameAs();

private:
    //variables
    MainWindow *window;
    QList<QList<Field*>> *board;
    QList<Piece*> *pieces, *capturedPieces;
    QList<QList<CardSlot*>> *slotsGrid;
    QList<Card*> *cards;
    Piece *pickedUpPiece, *pieceToReposition;
    int rows, cols, cardsPerPlayer, neutralCardsPerPlayer;
    double windowHeight, windowWidth, borderX, borderY, fieldHeight;

    int gameResult;        // game_result: 0 = ongoing game; 1 = player 1 has won; -1 = player 2 has won
    bool firstPlayersTurn;  // firstPlayersTurn: true = turn of player 1, false = turn of player 2
    bool flipBoard;
};

#endif // ONI_H
