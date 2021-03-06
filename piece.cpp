#include "piece.h"
#include "oni.h"

extern Oni *game;

Piece::Piece(QGraphicsItem *parent) : QGraphicsPixmapItem(parent), row(-1), col(-1) {
}

void Piece::draw() {
    // draw piece
    int pieceSize = static_cast<int>(game->getWindow()->getFieldSize());
    piece = QPixmap(pieceSize, pieceSize);
    piece.fill(Qt::transparent);
    QString name, color;
    switch (type) {
    case 'M': name = "master"; color = "red"; break;
    case 'S': name = "scolar"; color = "red"; break;
    case 'm': name = "master"; color = "blue"; break;
    case 's': name = "scolar"; color = "blue";
    }
    if (game->getPieceSet() == "Hanzi") {
        QFont namesFont = QFont("wts11");
        QPainter painter(&piece);
            if (color == "red") painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
            else painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.setPen(Qt::lightGray);
            painter.drawEllipse(15*pieceSize/100, 15*pieceSize/100, 70*pieceSize/100, 70*pieceSize/100);
            if (name == "master") {
                namesFont.setPixelSize(45*pieceSize/100);
                painter.setFont(namesFont);
                painter.drawText(QRectF(0.15*pieceSize, 0.15*pieceSize, 0.7*pieceSize, 0.7*pieceSize), Qt::AlignCenter, "師");
            }
            else {
                namesFont.setPixelSize(35*pieceSize/100);
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
