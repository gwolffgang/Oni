#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsPixmapItem>

class Piece : public QGraphicsPixmapItem {

public:
    // constructor
    Piece(QGraphicsItem *parent = nullptr);

    // getter
    inline int getCol() { return col; }
    inline int getOwner() { switch (type) { case 'M': case 'S': return 1; case 'm': case 's': return 2; } return 0; }
    inline QPixmap getPiece() { return piece; }
    inline int getRow() { return row; }
    inline char getType() { return type; }

    // setter
    inline void setCol(int newCol) { if (newCol > -1) col = newCol; }
    inline void setRow(int newRow) { if (newRow > -1) row = newRow; }
    inline void setType(char newType) { type = newType; }

    // methods
    void draw();

private:
    // variables
    int row, col;
    char type = ' ';
    QPixmap piece;
};

#endif // PIECE_H
