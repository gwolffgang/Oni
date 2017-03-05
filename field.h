#ifndef FIELD_H
#define FIELD_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "piece.h"

class Field : public QGraphicsRectItem {
private:
    // variables
    int col, row;
    char piecetype;
    Piece *piece;

public:
    // constructors
    Field(double size, QGraphicsItem *parent = NULL);

    // events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    // getters
    int getCol() { return col; }
    Piece* getPiece() { return piece; }
    char getPiecetype() { return piecetype; }
    int getRow() { return row; }

    // setters
    void setCol(int newCol) { if (newCol > -1) col = newCol; }
    void setPiece(Piece *newPiece) { piece = newPiece; }
    void setPiecetype(char newPiecetype) { piecetype = newPiecetype; }
    void setRow(int newRow) { if (newRow > -1) row = newRow; }

    // functions
    void addPiece(char type, double fieldSize);
    int identifiedPiece();

signals:
    void clicked() {}
};

#endif // FIELD_H
