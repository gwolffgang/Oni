#include "piece.h"
#include "oni.h"

extern Oni *game;

Piece::Piece(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    // presettings
    row = col = -1;
}

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
        setPos(game->getBorderX(), game->getBorderY());
        img = img.scaled(fieldSize - 2*game->getBorderX(), fieldSize - 2*game->getBorderY());
        setPixmap(img);
    }
}
