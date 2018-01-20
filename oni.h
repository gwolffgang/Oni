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
    QString playerNameBlue, playerNameRed;
    QList<QString> *turns;
    int gameResult;

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
    // constructors
    Oni();

    // getters
    inline int getActuallyDisplayedMove() { return actuallyDisplayedMove; }
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline QList<Piece*> *getCapturedBlue() { return capturedBlue; }
    inline QList<Piece*> *getCapturedRed() { return capturedRed; }
    inline QList<Card*> *getCards() { return cards; }
    inline bool getCardChoiceActive() { return cardChoiceActive; }
    inline Field *getChosenField() { return chosenField; }
    inline Field *getFieldOfOrigin() { return fieldOfOrigin; }
    inline bool getFirstPlayersTurn() { return firstPlayersTurn; }
    inline bool getFlippedBoard() { return flippedBoard; }
    inline int getGameResult() { return match.gameResult; }
    inline QString getOpenGameFileName() { return openGameFileName; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline QString getPieceSet() { return piecesSet; }
    inline QString getPlayerNameBlue() { return match.playerNameBlue; }
    inline QString getPlayerNameRed() { return match.playerNameRed; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }
    inline QList<QString> *getTurns() { return match.turns; }
    inline MainWindow *getWindow() { return window; }

    // setters
    inline void setActuallyDisplayedMove(int newMove) { actuallyDisplayedMove = newMove; }
    inline void setCardChoiceActive(bool state) { cardChoiceActive = state; }
    inline void setChosenField(Field *target) { chosenField = target; }
    inline void setFlippedBoard(bool state) { flippedBoard = state; }
    inline void setFirstPlayersTurn(bool state) { firstPlayersTurn = state; }
    inline void setFieldOfOrigin(Field *origin) { fieldOfOrigin = origin; }
    inline void setGameResult(int winner) { match.gameResult = winner; }
    inline void setOpenGameFileName(QString newString) { openGameFileName = newString; }
    inline void setPickedUpPiece(Piece *newPiece) { pickedUpPiece = newPiece; }
    inline void setPiecesSet(QString newString) { piecesSet = newString; }
    inline void setPlayerNames(QString nameRed = "Red", QString nameBlue = "Blue") {if (nameBlue != "") match.playerNameBlue = nameBlue;
                                                                                    if (nameRed != "") match.playerNameRed = nameRed;}

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
    QString openGameFileName, piecesSet;
    Piece *pickedUpPiece;
    Field *fieldOfOrigin, *chosenField;
    int actuallyDisplayedMove;
    bool firstPlayersTurn, flippedBoard, cardChoiceActive;
    Match match;
};

#endif // ONI_H
