#ifndef FIELD_H
#define FIELD_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "piece.h"

class Field : public QGraphicsRectItem {
private:
    // variables
    int col = -1, row = -1;
    char piece = ' ';

public:
    // constructors
    Field(double size, QGraphicsItem *parent = NULL);

    // events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    // getters
    int getCol() { return col; }
    char getPiece() { return piece; }
    int getRow() { return row; }

    // setters
    void setCol(int newCol) { if (newCol > -1) col = newCol; }
    void setPiece(char newPiece) { piece = newPiece; }
    void setRow(int newRow) { if (newRow > -1) row = newRow; }

    // functions
    void addPiece(char pieceType, double fieldSize);

signals:
    void clicked() {}
};

#endif // FIELD_H
