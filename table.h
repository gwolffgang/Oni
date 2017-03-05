#ifndef TABLE_H
#define TABLE_H

#include <QList>
#include "field.h"
#include "cardslot.h"

class Table {

private:
    // variables
    QList<QList<Field*>> *board;

public:
    // constructors
    Table() {}

    // getters
    inline QList<QList<Field*>> *getBoard() { return board; }

    // functions
    void drawBoard();
    void drawCardSlots(double border_x = 10, double border_y = 10);
};

#endif // TABLE_H
