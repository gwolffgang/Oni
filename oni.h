#ifndef ONI_H
#define ONI_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>
#include "table.h"
#include "card.h"

class Oni : public QGraphicsView {
    Q_OBJECT

private:
    //variables
    QGraphicsScene *scene;
    Table *table;
    QList<Piece*> *pieces, *capturedPieces;
    QList<Card*> *cards;
    Piece *pickedUpPiece, *pieceToReposition;
    int rows, cols, cardsPerPlayer, neutralCardsPerPlayer;
    double borderX, borderY, fieldHeight;

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
    inline QList<Piece*> *getCapturedPieces() { return capturedPieces; }
    inline QList<Card*> *getCards() { return cards; }
    inline int getCardsPerPlayer () { return cardsPerPlayer; }
    inline int getCols() { return cols; }
    inline int getFieldHeight() { return fieldHeight; }
    inline bool getFirstPlayersTurn() { return firstPlayersTurn; }
    inline int getGameResult() { return gameResult; }
    inline int getNeutralCardsPerPlayer() { return neutralCardsPerPlayer; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline Piece *getPieceToReposition() { return pieceToReposition; }
    inline int getRows() { return rows; }
    inline QGraphicsScene *getScene() { return scene; }
    int getStudentsLeft(int player);
    inline Table *getTable() { return table; }

    // setters
    inline void changePlayersTurn() { firstPlayersTurn = !firstPlayersTurn; }
    inline void setBorderX(int newX) { borderX = newX; }
    inline void setBorderY(int newY) { borderY = newY; }
    inline void setCols(int newCols) { cols = newCols; }
    inline void setGameResult (int winner) { gameResult = winner; }
    void setPickedUpPiece(Piece *newPiece);
    inline void setPieceToReposition(Piece *origin) { pieceToReposition = origin; }
    inline void setRows(int newRows) { rows = newRows; }

    //functions
    void start();

public slots:
    void load_game();
    void new_game();
    void save_game();
};

#endif // ONI_H
