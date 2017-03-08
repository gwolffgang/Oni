#ifndef TABLE_H
#define TABLE_H

#include <QList>
#include "field.h"
#include "cardslot.h"

class Table {

private:
    // variables
    QList<QList<Field*>> *board;
    QList<QList<CardSlot*>> *slotsGrid;

public:
    // constructors
    Table() {}

    // getters
    inline QList<QList<Field*>> *getBoard() { return board; }
    inline QList<QList<CardSlot*>> *getSlotsGrid() { return slotsGrid; }

    // functions
    void drawBoard();
    void drawCardSlots();
};

#endif // TABLE_H
