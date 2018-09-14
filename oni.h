#ifndef ONI_H
#define ONI_H

#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include "card.h"
#include "cardslot.h"
#include "field.h"
#include "mainwindow.h"
#include "match.h"
#include "piece.h"
#include "windowdatabase.h"

class Oni : public QObject {
    Q_OBJECT

public:
    // variables
    QString backupFileName, configFileName, databaseFileName, tempDataFileName;

    // constructor
    Oni();

    // getter
    inline int getCurrentDisplayedMove() { return currentDisplayedMove; }
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline QList<Piece*> *getCapturedBlue() { return capturedBlue; }
    inline QList<Piece*> *getCapturedRed() { return capturedRed; }
    inline QList<Card*> *getCards() { return cards; }
    inline bool getCardChoiceActive() { return cardChoiceActive; }
    inline Field *getChosenField() { return chosenField; }
    inline Field *getFieldOfOrigin() { return fieldOfOrigin; }
    inline bool getFlippedBoard() { return flippedBoard; }
    inline Match *getMatch() { return match; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline QString getPieceSet() { return piecesSet; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }
    inline MainWindow *getWindow() { return window; }
    inline WindowDatabase *getWindowDatabase() { return windowDatabase; }

    // setter
    inline void setCardChoiceActive(bool state) { cardChoiceActive = state; }
    inline void setChosenField(Field *target) { chosenField = target; }
    inline void setCurrentDisplayedMove(int newMove) { currentDisplayedMove = newMove; }
    inline void setFlippedBoard(bool state) { flippedBoard = state; }
    inline void setFieldOfOrigin(Field *origin) { fieldOfOrigin = origin; }
    inline void setPickedUpPiece(Piece *newPiece) { pickedUpPiece = newPiece; }
    inline void setPiecesSet(QString newString) { piecesSet = newString; }

    // methods
    inline bool getFirstPlayersTurn() { if (currentDisplayedMove%2 == 0) return true; else return false; }
    bool readConfig();
    QList<Card*> identifyCards(int owner);
    void setupFoldersAndFiles();
    void switchCards(CardSlot *usedCardSlot);
    void winGame(int winner);
    bool writeConfig();

private:
    // variables
    MainWindow *window;
    WindowDatabase *windowDatabase;
    QList<QList<Field*>> *board;
    QList<Piece*> *pieces, *capturedBlue, *capturedRed;
    QList<QList<CardSlot*>> *slotsGrid;
    QList<Card*> *cards;
    QString piecesSet;
    Piece *pickedUpPiece;
    Field *fieldOfOrigin, *chosenField;
    int currentDisplayedMove;
    bool flippedBoard, cardChoiceActive;
    Match *match;
};

#endif // ONI_H
