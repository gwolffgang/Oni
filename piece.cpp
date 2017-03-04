#include "piece.h"
#include "oni.h"

extern Oni *game;

Piece::Piece(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {}

void Piece::setPieceValues(char pieceType, double fieldSize) {
    // determinate card
    type = pieceType;
    QString piece, color;
    switch (type) {
    case 'M':
        piece = "master";
        color = "white";
        break;
    case 'S':
        piece = "scolar";
        color = "white";
        break;
    case 'm':
        piece = "master";
        color = "black";
        break;
    case 's':
        piece = "scolar";
        color = "black";
    }
    if (type != ' ') {
        QPixmap img(":/pics/pieces/" + piece + "_" + color + ".svg");
        setPos(game->border_x, game->border_y);
        img = img.scaled(fieldSize - 2*game->border_x, fieldSize - 2*game->border_y);
        setPixmap(img);
    }

}
