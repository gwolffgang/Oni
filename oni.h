#ifndef ONI_H
#define ONI_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "table.h"
#include "card.h"

class Oni : public QGraphicsView {

private:
    //variables
    QGraphicsScene *scene;
    Table *table;
    QList<Piece*> *pieces;
    Piece *pickedUpPiece;
    int rows, cols;
    int borderX, borderY;

    int gameResult;        // game_result: 0 = ongoing game; 1 = player 1 has won; -1 = player 2 has won
    int studentsLeft[2];   // students_left: [0] = first player, [1] = second player
    bool firstPlayersTurn;  // firstPlayersTurn: true = turn of player 1, false = turn of player 2

public:
    //constructors
    Oni(QWidget *parent = NULL);

    // events
    void mouseMoveEvent(QMouseEvent *event);

    // getters
    inline int getBorderX() { return borderX; }
    inline int getBorderY() { return borderY; }
    inline bool getFirstPlayersTurn() { return firstPlayersTurn; }
    inline int getGameResult () { return gameResult; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline QGraphicsScene *getScene() { return scene; }
    int getStudentsLeft(int player);

    // setters
    inline void changePlayersTurn() { firstPlayersTurn = !firstPlayersTurn; }
    inline void setBorderX(int newX) { borderX = newX; }
    inline void setBorderY(int newY) { borderY = newY; }
    inline void setGameResult (int winner) { gameResult = winner; }
    inline void setPickedUpPiece(Piece *newPiece) { pickedUpPiece = newPiece; }

    //functions
    void start();

public slots:
    void load_game();
    void new_game();
    void save_game();
};

#endif // ONI_H
