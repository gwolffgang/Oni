#ifndef SENSEI_H
#define SENSEI_H

#include <QtCore>

class Sensei {

public:
    // constructor
    Sensei();

    // getter
    QList<QString> getPossibleMoves(QString position);

private:
    // methods
    double evaluatePosition(QString position);

};

#endif // SENSEI_H
