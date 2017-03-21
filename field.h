#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QMouseEvent>
#include "piece.h"

class Field : public QObject, public QGraphicsRectItem {
    Q_OBJECT

private:
    // variables
    int col, row;
    char pieceType;
    Piece *piece;

protected:
    // events
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
    // constructors
    Field(QGraphicsItem *parent = NULL);

    // getters
    inline int getCol() { return col; }
    inline Piece *getPiece() { return piece; }
    inline char getPieceType() { return pieceType; }
    inline int getRow() { return row; }

    // setters
    inline void setCol(int newCol) { if (newCol > -1) col = newCol; }
    inline void setPiece(Piece *newPiece) { piece = newPiece; }
    inline void setPieceType(char newPieceType) { pieceType = newPieceType; }
    inline void setRow(int newRow) { if (newRow > -1) row = newRow; }

    // functions
    void captureOrChangePiece(Piece *target);
    void dropPiece();
    int identifyPiece();
    void linkPiece(Piece *linkedPiece);
    void pickUpPiece(Piece *piece);
    void putBackPiece(Field *origin);
};

#endif // FIELD_H
