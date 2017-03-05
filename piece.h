#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsPixmapItem>

class Piece : public QGraphicsPixmapItem {
private:
    // variables
    int row, col;
    char type = ' ';

public:
    // constructors
    Piece(QGraphicsItem *parent=NULL);

    // getters
    inline int getCol() { return col; }
    inline int getRow() { return row; }
    inline char getType() { return type; }

    // setters
    void setCol(int newCol) { if (newCol > -1) col = newCol; }
    void setRow(int newRow) { if (newRow > -1) row = newRow; }
    void setType(char newType) { type = newType; }

    //functions
    void setPieceValues(char pieceType, double fieldSize);
};

#endif // PIECE_H
