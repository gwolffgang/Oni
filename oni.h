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
#include "windowdatabase.h"
#include "piece.h"

struct Match {
    QString playerNameBlue, playerNameRed, event, city;
    QDate date;
    QList<QString> *turns;
    int gameResult, openDatabaseGameNumber;
    double round;

    bool operator==(const Match &other) const {
        if (playerNameBlue != other.playerNameBlue || playerNameRed != other.playerNameRed) return false;
        if (turns->size() != other.turns->size() || gameResult != other.gameResult) return false;
        for (int i = 0; i < turns->size(); i++) if (turns->at(i) != other.turns->at(i)) return false;
        return true;
    }
};

class Oni : public QObject {
    Q_OBJECT

public:
    // variables
    QString backupFileName, configFileName, databaseFileName;

    // constructor
    Oni();

    // getter
    inline int getActuallyDisplayedMove() { return actuallyDisplayedMove; }
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline QList<Piece*> *getCapturedBlue() { return capturedBlue; }
    inline QList<Piece*> *getCapturedRed() { return capturedRed; }
    inline QList<Card*> *getCards() { return cards; }
    inline bool getCardChoiceActive() { return cardChoiceActive; }
    inline Field *getChosenField() { return chosenField; }
    inline QString getCity() { return match.city; }
    inline QDate getDate() { return match.date; }
    inline QString getEvent() { return match.event; }
    inline Field *getFieldOfOrigin() { return fieldOfOrigin; }
    inline bool getFlippedBoard() { return flippedBoard; }
    inline int getGameResult() { return match.gameResult; }
    inline int getOpenDatabaseGameNumber() { return match.openDatabaseGameNumber; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline QString getPieceSet() { return piecesSet; }
    inline QString getPlayerNameBlue() { return match.playerNameBlue; }
    inline QString getPlayerNameRed() { return match.playerNameRed; }
    inline double getRound() { return match.round; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }
    inline QList<QString> *getTurns() { return match.turns; }
    inline MainWindow *getWindow() { return window; }
    inline WindowDatabase *getWindowDatabase() { return windowDatabase; }

    // setter
    inline void setActuallyDisplayedMove(int newMove) { actuallyDisplayedMove = newMove; }
    inline void setCardChoiceActive(bool state) { cardChoiceActive = state; }
    inline void setChosenField(Field *target) { chosenField = target; }
    inline void setCity(QString newCity) { match.city = newCity; }
    inline void setDate(QDate newDate) { match.date = newDate; }
    inline void setDate(QString newDate) { match.date = QDate::fromString(newDate, Qt::ISODate); }
    inline void setEvent(QString newEvent) { match.event = newEvent; }
    inline void setFlippedBoard(bool state) { flippedBoard = state; }
    inline void setFieldOfOrigin(Field *origin) { fieldOfOrigin = origin; }
    inline void setGameResult(int newResult) { if (abs(newResult) == 1) match.gameResult = newResult; else match.gameResult = 0; }
    inline void setGameResult(QString newResult) { if (newResult == "1-0") match.gameResult = 1; else if (newResult == "0-1") match.gameResult = -1; else match.gameResult = 0; }
    inline void setOpenDatabaseGameNumber(int newNumber) { match.openDatabaseGameNumber = newNumber; }
    inline void setPickedUpPiece(Piece *newPiece) { pickedUpPiece = newPiece; }
    inline void setPiecesSet(QString newString) { piecesSet = newString; }
    inline void setPlayerNames(QString nameRed = "Red", QString nameBlue = "Blue") {if (nameBlue != "") match.playerNameBlue = nameBlue; if (nameRed != "") match.playerNameRed = nameRed;}
    inline void setRound(double newRound) { match.round = newRound; }

    // methods
    inline bool getFirstPlayersTurn() { if (match.turns->size()%2 == 1) return true; else return false; }
    bool readConfig();
    QList<Card*> identifyCards(int owner);
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
    int actuallyDisplayedMove;
    bool flippedBoard, cardChoiceActive;
    Match match;
};

#endif // ONI_H
