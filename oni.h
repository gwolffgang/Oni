#ifndef ONI_H
#define ONI_H

#include <QApplication>
#include <QBrush>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QMouseEvent>
#include <QListWidget>
#include <QObject>
#include <QScreen>
#include <QTextStream>
#include <QtPrintSupport/QPrinter>
#include "card.h"
#include "cardslot.h"
#include "field.h"
#include "mainwindow.h"
#include "piece.h"

class Oni : public QObject {
    Q_OBJECT

public:
    // constructors
    Oni();

    // getters
    inline int getActuallyDisplayedMove() { return actuallyDisplayedMove; }
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline QList<Piece*> *getCapturedBlue() { return capturedBlue; }
    inline QList<Piece*> *getCapturedRed() { return capturedRed; }
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
    inline QString getPieceSet() { return piecesSet; }
    inline QString getPlayerNameBlue() { return playerNameBlue; }
    inline QString getPlayerNameRed() { return playerNameRed; }
    inline int getRows() { return rows; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }
    inline QList<QString> *getTurns() { return turns; }
    inline MainWindow *getWindow() { return window; }

    // setters
    inline void setActuallyDisplayedMove(int newMove) { actuallyDisplayedMove = newMove; }
    inline void setCardChoiceActive(bool state) { cardChoiceActive = state; }
    inline void setChosenField(Field *target) { chosenField = target; }
    inline void setCols(int newCols) { cols = newCols; }
    inline void setFlippedBoard(bool state) { flippedBoard = state; }
    inline void setFirstPlayersTurn(bool state) { firstPlayersTurn = state; }
    inline void setFieldOfOrigin(Field *origin) { fieldOfOrigin = origin; }
    inline void setGameResult(int winner) { gameResult = winner; }
    inline void setOpenGameFileName(QString newString) { openGameFileName = newString; }
    inline void setPickedUpPiece(Piece *newPiece) { pickedUpPiece = newPiece; }
    inline void setPiecesSet(QString newString) { piecesSet = newString; }
    void setPlayerNames(QString nameRed = "Red", QString nameBlue = "Blue");
    inline void setRows(int newRows) { rows = newRows; }

    // methods
    bool readConfig();
    QList<Card*> identifyCards(int owner);
    void switchCards(CardSlot *usedCardSlot);
    void winGame(int winner);
    bool writeConfig() const;

private:
    // variables
    MainWindow *window;
    QList<QList<Field*>> *board;
    QList<Piece*> *pieces, *capturedBlue, *capturedRed;
    QList<QList<CardSlot*>> *slotsGrid;
    QList<Card*> *cards;
    QList<QString> *turns;
    QString playerNameRed, playerNameBlue, openGameFileName;
    Piece *pickedUpPiece;
    Field *fieldOfOrigin, *chosenField;
    int rows, cols, cardsPerPlayer, neutralCardsPerGame, actuallyDisplayedMove, gameResult;
    bool firstPlayersTurn, flippedBoard, cardChoiceActive;
    QString piecesSet;
    QFile configFile;
};

#endif // ONI_H
