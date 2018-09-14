#include "match.h"
#include "oni.h"

Match::Match() :
    playerNameBlue("Blue"), playerNameRed("Red"), event(""), city(""), date(QDate::currentDate()),
    turns(new QList<QString>), result(0), openDatabaseGameNumber(-1), round(0) {

}

void Match::setResult(QString newResult) {
    if (newResult == "1-0")
        result = 1;
    else {
        if (newResult == "0-1")
            result = -1;
        else
            result = 0;
    }
}

bool Match::operator==(const Match &other) const {
    if (playerNameBlue != other.playerNameBlue || playerNameRed != other.playerNameRed ||
        turns->size() != other.turns->size() || result != other.result)
        return false;
    for (int i = 0; i < turns->size(); i++)
        if (turns->at(i) != other.turns->at(i))
            return false;
    return true;
}
