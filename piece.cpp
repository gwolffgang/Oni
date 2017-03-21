#include "piece.h"
#include "oni.h"

extern Oni *game;

Piece::Piece(QGraphicsItem *parent) : QGraphicsPixmapItem(parent) {
    // presettings
    row = col = -1;
}

void Piece::drawPiece() {
    // draw piece
    QString piece, color;
    switch (type) {
    case 'M':
        piece = "master";
        color = "red";
        break;
    case 'S':
        piece = "scolar";
        color = "red";
        break;
    case 'm':
        piece = "master";
        color = "blue";
        break;
    case 's':
        piece = "scolar";
        color = "blue";
    }
    if (type != ' ') {
        QPixmap img(":/pics/pieces/" + piece + "_" + color + ".png");
        img = img.scaled(game->getWindow()->getFieldHeight(), game->getWindow()->getFieldHeight());
        setPixmap(img);
    }
}
