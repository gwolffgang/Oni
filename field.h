#ifndef FIELD_H
#define FIELD_H

#include "piece.h"
#include "card.h"
#include "cardslot.h"

class Field : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    // constructor
    Field(QGraphicsItem *parent = nullptr);

    // operator
    inline bool operator==(Field *field) { if ((field->row == row) && (field->col == col)) return true; return false; }

    // getter
    inline int getCol() { return col; }
    inline Piece *getPiece() { return piece; }
    inline char getPieceType() { return pieceType; }
    inline int getRow() { return row; }

    // setter
    inline void setCol(int newCol) { if (newCol > -1) col = newCol; }
    inline void setPiece(Piece *newPiece) { piece = newPiece; }
    inline void setPieceType(char newPieceType) { pieceType = newPieceType; }
    inline void setRow(int newRow) { if (newRow > -1) row = newRow; }

    // methods
    void captureOrChangePiece(Piece *target);
    void dropPiece();
    void exchangeCards();
    int identifyPiece();
    void linkPiece(Piece *linkedPiece);
    void pickUpPiece(Piece *piece);
    void unmarkAllFields();

protected:
    // events
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // variables
    int col, row;
    char pieceType;
    Piece *piece;
};

#endif // FIELD_H
