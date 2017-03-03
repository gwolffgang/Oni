#ifndef TABLE_H
#define TABLE_H

#include <QList>
#include "field.h"
#include "cardslot.h"


class Table {
private:
    //variables
    QList<Field*> board;
    QList<CardSlot*> cardGrid;

public:
    //constructors
    Table() {}

    //functions
    void drawBoard(double border_x = 10, double border_y = 10, int numOfCols = 5, int numOfRows = 5);
    void drawCardSlots(double border_x = 10, double border_y = 10);
};

#endif // TABLE_H
