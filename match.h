#ifndef MATCH_H
#define MATCH_H

#include <QtCore>

#include "card.h"
#include "cardslot.h"
#include "field.h"
#include "piece.h"

class Match {

public:
    // constructor
    Match();

    // getter
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline QList<Piece*> *getCapturedBlue() { return capturedBlue; }
    inline QList<Piece*> *getCapturedRed() { return capturedRed; }
    inline QList<Card*> *getCards() { return cards; }
    inline Field *getChosenField() { return chosenField; }
    inline QString getCity() { return city; }
    inline QDate getDate() { return date; }
    inline QString getEvent() { return event; }
    inline Field *getFieldOfOrigin() { return fieldOfOrigin; }
    inline Piece *getPickedUpPiece() { return pickedUpPiece; }
    inline QList<Piece*> *getPieces() { return pieces; }
    inline int getResult() { return result; }
    inline int getOpenDatabaseGameNumber() { return openDatabaseGameNumber; }
    inline QString getPlayerNameBlue() { return playerNameBlue; }
    inline QString getPlayerNameRed() { return playerNameRed; }
    inline double getRound() { return round; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }
    inline QList<QString> *getTurns() { return turns; }

    // setter
    inline void setChosenField(Field *target) { chosenField = target; }
    inline void setCity(QString newCity) { city = newCity; }
    inline void setDate(QDate newDate) { date = newDate; }
    inline void setDate(QString newDate) { date = QDate::fromString(newDate, Qt::ISODate); }
    inline void setEvent(QString newEvent) { event = newEvent; }
    inline void setFieldOfOrigin(Field *origin) { fieldOfOrigin = origin; }
    inline void setResult(int newResult) { if (abs(newResult) == 1) result = newResult; else result = 0; }
    void setResult(QString newResult);
    inline void setOpenDatabaseGameNumber(int newNumber) { openDatabaseGameNumber = newNumber; }
    inline void setPickedUpPiece(Piece *newPiece) { pickedUpPiece = newPiece; }
    inline void setPlayerNameBlue(QString nameBlue = "Blue") { if (nameBlue != "") playerNameBlue = nameBlue; }
    inline void setPlayerNameRed(QString nameRed = "Red") { if (nameRed != "") playerNameRed = nameRed; }
    inline void setRound(double newRound) { round = newRound; }

    // methods
    QList<Card*> identifyCards(int owner);
    void switchCards(CardSlot *usedCardSlot);

private:
    QString playerNameBlue, playerNameRed, event, city;
    QDate date;
    QList<QString> *turns;
    double round;
    int result, openDatabaseGameNumber;
    QList<QList<Field*>> *board;
    QList<Piece*> *pieces, *capturedBlue, *capturedRed;
    QList<QList<CardSlot*>> *slotsGrid;
    QList<Card*> *cards;
    Piece *pickedUpPiece;
    Field *fieldOfOrigin, *chosenField;

    bool operator==(const Match &other) const;
};

#endif // MATCH_H
