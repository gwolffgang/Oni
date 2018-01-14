#include "piece.h"
#include "oni.h"

extern Oni *game;

Piece::Piece(QGraphicsItem *parent) : QGraphicsPixmapItem(parent), row(-1), col(-1) {
}

int Piece::getOwner() {
    switch (type) {
    case 'M':
    case 'S':
        return 1;
    case 'm':
    case 's':
        return 2;
    }
    return 0;
}

void Piece::drawPiece() {
    // draw piece
    double pieceSize = game->getWindow()->getFieldSize();
    piece = QPixmap(pieceSize, pieceSize);
    piece.fill(Qt::transparent);
    QString name, color;
    switch (type) {
    case 'M':
        name = "master"; color = "red"; break;
    case 'S':
        name = "scolar"; color = "red"; break;
    case 'm':
        name = "master"; color = "blue"; break;
    case 's':
        name = "scolar"; color = "blue";
    }
    if (game->getPieceSet() == "Hanzi") {
        QFont namesFont = QFont("wts11");
        QPainter painter(&piece);
            if (color == "red") painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
            else painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.setPen(Qt::lightGray);
            painter.drawEllipse(0.15*pieceSize, 0.15*pieceSize, 0.7*pieceSize, 0.7*pieceSize);
            if (name == "master") {
                namesFont.setPixelSize(0.45*pieceSize);
                painter.setFont(namesFont);
                painter.drawText(QRectF(0.15*pieceSize, 0.15*pieceSize, 0.7*pieceSize, 0.7*pieceSize), Qt::AlignCenter, "師");
            }
            else {
                namesFont.setPixelSize(0.35*pieceSize);
                painter.setFont(namesFont);
                painter.drawText(QRectF(0.15*pieceSize, 0.15*pieceSize, 0.7*pieceSize, 0.7*pieceSize), Qt::AlignCenter, "徒");
            }
        painter.end();
        setPixmap(piece);
    } else if (game->getPieceSet() == "ComicStyle") {
        if (type != ' ') {
            QPixmap img(":/pics/pieces/" + name + "_" + color + ".png");
            piece = img.scaled(pieceSize, pieceSize);
            setPixmap(piece);
        }
    }
}
