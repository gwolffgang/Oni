#ifndef FIELD_H
#define FIELD_H

#include "qobject.h" // #include <QObject>
#include "qgraphicsitem.h" // #include <QGraphicsRectItem>
#include "qgraphicssceneevent.h" // #include <QGraphicsSceneMouseEvent>
// #include <QGraphicsSceneDragDropEvent> ist bereits in Z.6 drin
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
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

public:
    // constructors
    Field(double size, QGraphicsItem *parent = NULL);

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
    void capturePiece(Piece *target);
    void dropPiece();
    int identifyPiece();
    void linkPiece(Piece *linkedPiece);
    void pickUpPiece(Field *field);
};

#endif // FIELD_H
