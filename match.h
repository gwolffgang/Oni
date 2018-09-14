#ifndef MATCH_H
#define MATCH_H

#include <QtCore>

class Match {

public:
    // constructor
    Match();

    // getter
    inline QString getCity() { return city; }
    inline QDate getDate() { return date; }
    inline QString getEvent() { return event; }
    inline int getResult() { return result; }
    inline int getOpenDatabaseGameNumber() { return openDatabaseGameNumber; }
    inline QString getPlayerNameBlue() { return playerNameBlue; }
    inline QString getPlayerNameRed() { return playerNameRed; }
    inline double getRound() { return round; }
    inline QList<QString> *getTurns() { return turns; }

    // setter
    inline void setCity(QString newCity) { city = newCity; }
    inline void setDate(QDate newDate) { date = newDate; }
    inline void setDate(QString newDate) { date = QDate::fromString(newDate, Qt::ISODate); }
    inline void setEvent(QString newEvent) { event = newEvent; }
    inline void setResult(int newResult) { if (abs(newResult) == 1) result = newResult; else result = 0; }
    void setResult(QString newResult);
    inline void setOpenDatabaseGameNumber(int newNumber) { openDatabaseGameNumber = newNumber; }
    inline void setPlayerNameBlue(QString nameBlue = "Blue") { if (nameBlue != "") playerNameBlue = nameBlue; }
    inline void setPlayerNameRed(QString nameRed = "Red") { if (nameRed != "") playerNameRed = nameRed; }
    inline void setRound(double newRound) { round = newRound; }

private:
    QString playerNameBlue, playerNameRed, event, city;
    QDate date;
    QList<QString> *turns;
    int result, openDatabaseGameNumber;
    double round;

    bool operator==(const Match &other) const;
};

#endif // MATCH_H
