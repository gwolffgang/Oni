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
        setPos(game->getWindow()->getBorderX(), game->getWindow()->getBorderY());
        img = img.scaled(game->getWindow()->getFieldHeight() - 2*game->getWindow()->getBorderX(), game->getWindow()->getFieldHeight() - 2*game->getWindow()->getBorderY());
        setPixmap(img);
    }
}
